#include "condition_filter.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>

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

void condition_filter::set_condition(std::uint64_t id, std::string operand, const std::string& op_str, value expected) {
    const auto op = parse_operator_string(op_str);
    if (op == condition_operator::in_list) {
        throw std::invalid_argument("operator 'IN' requires a value list overload");
    }
    condition_expression expr{op, std::move(operand), std::move(expected)};
    set_condition(id, std::move(expr));
}

void condition_filter::set_condition(std::uint64_t id, std::string operand, const std::string& op_str, std::vector<value> expected_list) {
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

// --- string parsing support for set_condition_string ---
namespace {

std::string trim_copy(const std::string& s) {
    std::size_t a = 0;
    while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    std::size_t b = s.size();
    while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b - a);
}

// parse quoted string (single or double quotes). pos must point at quote char.
std::string parse_quoted(const std::string& s, std::size_t& pos) {
    if (pos >= s.size()) throw std::invalid_argument("expected quoted string");
    char q = s[pos++];
    std::string out;
    while (pos < s.size()) {
        char c = s[pos++];
        if (c == q) return out;
        if (c == '\\' && pos < s.size()) {
            out.push_back(s[pos++]);
        } else {
            out.push_back(c);
        }
    }
    throw std::invalid_argument("unterminated quoted string");
}

// parse token until whitespace or one of stop chars
std::string parse_token(const std::string& s, std::size_t& pos) {
    std::size_t start = pos;
    while (pos < s.size()) {
        char c = s[pos];
        if (std::isspace(static_cast<unsigned char>(c)) || c == '[' || c == ']' || c == ',' ) break;
        ++pos;
    }
    return s.substr(start, pos - start);
}

value parse_value_token(const std::string& tok) {
    std::string t = trim_copy(tok);
    if (t.empty()) throw std::invalid_argument("empty value");
    bool has_dot = false;
    for (char c : t) {
        if (c == '.' || c == 'e' || c == 'E') { has_dot = true; break; }
    }
    if (!has_dot) {
        try {
            long long v = std::stoll(t);
            return value{static_cast<long long>(v)};
        } catch (...) {
            // fallthrough to double/string
        }
    }
    try {
        double d = std::stod(t);
        return value{d};
    } catch (...) {
        // fallback to string
    }
    return value{t};
}

condition_expression parse_condition_from_string(const std::string& src) {
    std::size_t pos = 0;
    auto s = src;
    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;

    // operand
    std::string operand;
    if (pos < s.size() && (s[pos] == '\'' || s[pos] == '"')) {
        operand = parse_quoted(s, pos);
    } else {
        operand = parse_token(s, pos);
    }
    operand = trim_copy(operand);

    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;

    if (pos >= s.size()) {
        throw std::invalid_argument("expected operator or value after operand");
    }

    // detect operator or decide it's omitted (shorthand equality)
    std::string op_tok;
    char c = s[pos];
    if (c == '<' || c == '>' || c == '=') {
        op_tok.push_back(c);
        ++pos;
        if (pos < s.size() && s[pos] == '=') {
            op_tok.push_back('=');
            ++pos;
        }
        while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
    } else if (std::isalpha(static_cast<unsigned char>(c))) {
        // e.g., IN
        std::size_t start = pos;
        while (pos < s.size() && std::isalpha(static_cast<unsigned char>(s[pos]))) ++pos;
        op_tok = s.substr(start, pos - start);
        while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
    } else {
        // No explicit operator; shorthand equality. Leave op_tok empty.
    }

    // If operator is IN, parse list
    std::string up_op = op_tok;
    for (auto& ch : up_op) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    if (!up_op.empty() && up_op == "IN") {
        if (pos >= s.size() || s[pos] != '[') throw std::invalid_argument("expected '[' for IN list");
        ++pos;
        std::vector<value> vals;
        while (true) {
            while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
            if (pos >= s.size()) throw std::invalid_argument("unterminated IN list");
            if (s[pos] == '\'' || s[pos] == '"') {
                std::string tok = parse_quoted(s, pos);
                vals.emplace_back(tok);
            } else {
                std::string tok = parse_token(s, pos);
                if (tok.empty()) throw std::invalid_argument("empty list element");
                vals.emplace_back(parse_value_token(tok));
            }
            while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
            if (pos >= s.size()) throw std::invalid_argument("unterminated IN list");
            if (s[pos] == ',') { ++pos; continue; }
            if (s[pos] == ']') { ++pos; break; }
            throw std::invalid_argument("expected ',' or ']' in IN list");
        }
        return condition_expression{std::move(operand), std::move(vals)};
    }

    // parse single value (operator may be empty => equality)
    value v;
    if (pos < s.size() && (s[pos] == '\'' || s[pos] == '"')) {
        std::string tok = parse_quoted(s, pos);
        v = value{tok};
    } else {
        std::string tok = parse_token(s, pos);
        if (tok.empty()) throw std::invalid_argument("expected value");
        v = parse_value_token(tok);
    }

    // if no operator specified, treat as equality
    if (op_tok.empty()) {
        return condition_expression{condition_operator::is_equal, std::move(operand), std::move(v)};
    }

    condition_operator op = parse_operator_string(op_tok);
    return condition_expression{op, std::move(operand), std::move(v)};
}

} // namespace

void condition_filter::set_condition_string(std::uint64_t id, const std::string& cond_str) {
    condition_expression expr = parse_condition_from_string(cond_str);
    set_condition(id, std::move(expr));
}

} // namespace cond3
