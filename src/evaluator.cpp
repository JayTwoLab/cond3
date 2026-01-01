#include "evaluator.hpp"

namespace cond3 {

static evaluate_result make_error(evaluate_error e) {
    return evaluate_result{false, false, e};
}

static bool compare_single(condition_operator op, const value& actual, const value& expected) {
    switch (op) {
        case condition_operator::is_equal: return actual == expected;
        case condition_operator::greater_than: return actual > expected;
        case condition_operator::greater_equal_than: return actual >= expected;
        case condition_operator::less_than: return actual < expected;
        case condition_operator::less_equal_than: return actual <= expected;
        default: return false;
    }
}

static bool compare_in_list(const value& actual, const std::vector<value>& list) {
    for (const auto& v : list) {
        if (actual == v) {
            return true;
        }
    }
    return false;
}

evaluate_result evaluate(const condition& cond, const subject& subj) {
    const auto& expr = cond.expression();

    if (!expr.is_set()) {
        if (expr.op == condition_operator::in_list && expr.expected_list.empty()) {
            return make_error(evaluate_error::empty_list);
        }
        return make_error(evaluate_error::expression_not_set);
    }

    if (subj.key != expr.operand) {
        return make_error(evaluate_error::key_mismatch);
    }

    if (expr.op == condition_operator::in_list) {
        if (expr.expected_list.empty()) {
            return make_error(evaluate_error::empty_list);
        }

        for (const auto& v : expr.expected_list) {
            if (v.type() != subj.actual.type()) {
                return make_error(evaluate_error::type_mismatch);
            }
        }

        return evaluate_result{true, compare_in_list(subj.actual, expr.expected_list), evaluate_error::none};
    }

    if (subj.actual.type() != expr.expected.type()) {
        return make_error(evaluate_error::type_mismatch);
    }

    return evaluate_result{true, compare_single(expr.op, subj.actual, expr.expected), evaluate_error::none};
}

} // namespace cond3
