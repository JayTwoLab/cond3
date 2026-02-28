#pragma once

#include "rule.hpp"

#include <string>

namespace cond3 {

// Load rules from a JSON file and register them into the provided rule_engine.
// Expected JSON format: an array of objects with fields:
//   { "id": <number>, "expr": "<rule expression string>" }
void load_rules_from_file(rule_engine& engine, const std::string& path);

} // namespace cond3