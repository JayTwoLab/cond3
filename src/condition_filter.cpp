#include "condition_filter.hpp"

namespace cond3 {

void condition_filter::reset() {
    conditions_.clear();
}

void condition_filter::set_condition(std::uint64_t id, condition_expression expr) {
    conditions_[id] = condition{id, std::move(expr)};
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
