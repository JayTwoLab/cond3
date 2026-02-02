#include "condition_filter.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <stdexcept>

namespace cond3 {

void condition_filter::reset() {
    conditions_.clear();
}

void condition_filter::set_condition(std::uint64_t id, condition_expression expr) {
    conditions_[id] = condition{id, std::move(expr)};
}

// Helper to map operator string to enum
static condition_operator parse_operator_string(const std::string& s) {
    // trim + upper-case
    std::string t;
    t.reserve(s.size());
    std::copy_if(s.begin(), s.end(), std::back_inserter(t), [](unsigned char c){ return !std::isspace(c); });
    for (auto& ch : t) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));

    if (t == "<" || t == "LT" || t == "LESS" || t == "LESS_THAN") {
        return condition_operator::less_than;
    }
    if (t == ">" || t == "GT" || t == "GREATER" || t == "GREATER_THAN") {
        return condition_operator::greater_than;
    }
    if (t == "<=" || t == "LE" || t == "LESS_EQUAL" || t == "LESS_EQUAL_THAN") {
        return condition_operator::less_equal_than;
    }
    if (t == ">=" || t == "GE" || t == "GREATER_EQUAL" || t == "GREATER_EQUAL_THAN") {
        return condition_operator::greater_equal_than;
    }
    if (t == "=" || t == "==" || t == "EQ" || t == "IS" || t == "IS_EQUAL") {
        return condition_operator::is_equal;
    }
    if (t == "IN") {
        return condition_operator::in_list;
    }

    throw std::invalid_argument("unknown operator string: " + s);
}

void condition_filter::set_condition(std::uint64_t id, const std::string& op_str, std::string operand, value expected) {
    const auto op = parse_operator_string(op_str);
    if (op == condition_operator::in_list) {
        throw std::invalid_argument("operator 'IN' requires a value list overload");
    }
    condition_expression expr{op, std::move(operand), std::move(expected)};
    set_condition(id, std::move(expr));
}

void condition_filter::set_condition(std::uint64_t id, const std::string& op_str, std::string operand, std::vector<value> expected_list) {
    const auto op = parse_operator_string(op_str);
    if (op != condition_operator::in_list) {
        throw std::invalid_argument("only operator 'IN' accepts a value list");
    }
    condition_expression expr{std::move(operand), std::move(expected_list)};
    set_condition(id, std::move(expr));
}

std::map<std::uint64_t, evaluate_result> condition_filter::evaluate_all(const subject_map& subjects) const {
    std::map<std::uint64_t, evaluate_result> out;

    for (const auto& kv : conditions_) {
        const auto& cond = kv.second;
        const auto& expr = cond.expression();

        auto it = subjects.find(expr.operand);
        if (it == subjects.end()) {
            out.emplace(cond.id(), evaluate_result{false, false, evaluate_error::key_mismatch});
            continue;
        }

        out.emplace(cond.id(), cond3::evaluate(cond, it->second));
    }

    return out;
}

evaluate_result condition_filter::evaluate_one(std::uint64_t condition_id, const subject_map& subjects) const {
    auto it_cond = conditions_.find(condition_id);
    if (it_cond == conditions_.end()) {
        return evaluate_result{false, false, evaluate_error::expression_not_set};
    }

    const auto& cond = it_cond->second;
    const auto& expr = cond.expression();

    auto it_subj = subjects.find(expr.operand);
    if (it_subj == subjects.end()) {
        return evaluate_result{false, false, evaluate_error::key_mismatch};
    }

    return cond3::evaluate(cond, it_subj->second);
}

} // namespace cond3
