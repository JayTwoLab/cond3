#pragma once

#include "rule.hpp"

#include <string>

namespace cond3 {

// Parse a boolean rule expression and return the corresponding rule_node.
//
// Supported tokens:
// - unsigned integers (condition ids), e.g. 11
// - AND, OR, NOT (case-insensitive)
// - parentheses: ( )
//
// Operator precedence: NOT (highest, unary), AND, OR (lowest).
//
// Throws std::invalid_argument on parse error.
rule_node parse_rule(const std::string& expr);

} // namespace cond3