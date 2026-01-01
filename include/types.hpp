#pragma once

#include <cstdint>
#include <string>

namespace cond3 {

// Value type
enum class value_type : std::uint8_t {
    not_set = 0,
    number = 1,
    real_number = 2,
    string = 3,
};

// Condition operator
enum class condition_operator : std::uint8_t {
    is_equal = 1,
    greater_than = 2,
    greater_equal_than = 3,
    less_than = 4,
    less_equal_than = 5,

    // true when the actual value is contained in the list
    in_list = 6,
};

// Evaluation failure reasons (useful for debugging/logging)
enum class evaluate_error : std::uint8_t {
    none = 0,
    expression_not_set,
    key_mismatch,
    type_mismatch,
    invalid_operator,
    empty_list,
};

inline const char* to_string(value_type t) {
    switch (t) {
        case value_type::not_set: return "not_set";
        case value_type::number: return "number";
        case value_type::real_number: return "real_number";
        case value_type::string: return "string";
        default: return "unknown";
    }
}

inline const char* to_string(condition_operator op) {
    switch (op) {
        case condition_operator::is_equal: return "==";
        case condition_operator::greater_than: return ">";
        case condition_operator::greater_equal_than: return ">=";
        case condition_operator::less_than: return "<";
        case condition_operator::less_equal_than: return "<=";
        case condition_operator::in_list: return "IN";
        default: return "?";
    }
}

inline const char* to_string(evaluate_error e) {
    switch (e) {
        case evaluate_error::none: return "none";
        case evaluate_error::expression_not_set: return "expression_not_set";
        case evaluate_error::key_mismatch: return "key_mismatch";
        case evaluate_error::type_mismatch: return "type_mismatch";
        case evaluate_error::invalid_operator: return "invalid_operator";
        case evaluate_error::empty_list: return "empty_list";
        default: return "unknown";
    }
}

} // namespace cond3
