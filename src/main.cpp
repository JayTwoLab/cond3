#include "rule.hpp"
#include "types.hpp"
#include "subject_utils.hpp"
#include "rule_parser.hpp"

#include <iostream>
#include <vector>

using cond3::condition_expression;
using cond3::condition_operator;
using cond3::rule_engine;
using cond3::rule_node;
using cond3::to_string;
using cond3::value;

int main() {
    rule_engine engine;

    // Define conditions

    // condition 11: LATITUDE < 42.0
    // engine.set_condition(11, "LATITUDE", "<", value{ 42.0 }); // double comparison
    engine.set_condition_string(11, "'LATITUDE' < 42.0"); // double comparison

    // condition 21: TEST INDICATOR == 0
    // engine.set_condition(21, "TEST INDICATOR", "=", value{ std::int64_t{0} }); // integer comparison
	engine.set_condition_string(21, "'TEST INDICATOR' = 0"); // integer comparison (shorthand equality)

    // condition 31: HELLO == 'hello'
	// engine.set_condition(31, "HELLO", "=", value{ "hello" }); // string comparison
    engine.set_condition_string(31, "'HELLO' = 'hello'"); // string comparison (single-quote form)

    // condition 41: TEST INDICATOR IN [2,3,5]
    // engine.set_condition<std::int64_t>(41, "TEST INDICATOR", "IN", { 2, 3, 5 }); // integer IN list
	engine.set_condition_string(41, "'TEST INDICATOR' IN [2,3,5]"); // integer IN list (string form)

    // Rule tree: parse from string
    // RULE = (11 AND (41 OR 31) AND NOT 21)
    rule_node rule = cond3::parse_rule("(11 AND (41 OR 31) AND NOT 21)");

    // Input (subjects)
    rule_engine::subject_map subjects;

    // Use helper from subject_utils.hpp so the key string is written only once
    cond3::add_subject(subjects, "LATITUDE", value{ 38.5 }); // double match
    cond3::add_subject(subjects, "TEST INDICATOR", value{ std::int64_t{3} }); // integer match
    cond3::add_subject(subjects, "HELLO", value{ "hello" }); // string match

    // Evaluate rule
    auto r = engine.evaluate_rule(rule, subjects);

    if (!r.ok) {
        // error can happen e.g. when a subject key is missing.
        std::cout << "rule => error: " << to_string(r.error) << "\n";
        return 1;
    }

    std::cout << "rule => " << (r.value ? "true" : "false") << "\n";
    return 0;
}
