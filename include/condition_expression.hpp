#pragma once

#include "types.hpp"
#include "value.hpp"

#include <string>
#include <utility>
#include <vector>

namespace cond3 {

// Expression that groups an "operand" (key), a Comparison Operator, and the expected value (single or list)
struct condition_expression {
    condition_operator op{condition_operator::is_equal};
    std::string operand;

    value expected;
    std::vector<value> expected_list;

    condition_expression() = default;

    condition_expression(condition_operator op_, std::string operand_, value expected_)
        : op(op_), operand(std::move(operand_)), expected(std::move(expected_)) {}

    // Constructor for IN list
    condition_expression(std::string operand_, std::vector<value> list_)
        : op(condition_operator::in_list), operand(std::move(operand_)), expected_list(std::move(list_)) {}

    bool is_set() const {
        if (operand.empty()) {
            return false;
        }

        if (op == condition_operator::in_list) {
            return !expected_list.empty();
        }

        return expected.type() != value_type::not_set;
    }
};

} // namespace cond3
