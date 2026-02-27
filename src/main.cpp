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

	// Part 1: Define conditions
    // 
    // Define conditions
    //  condition 11: LATITUDE < 42.0           // double comparison
    //  condition 21: TEST INDICATOR == 0       // integer comparison
    //  condition 31: HELLO == 'hello'          // string comparison
    //  condition 41: TEST INDICATOR IN [2,3,5] // integer IN list

	// Definition of condition type 1 : direct API with typed value
    {
        // engine.set_condition(11, "LATITUDE",       "<", value{ 42.0 }); 
        // engine.set_condition(21, "TEST INDICATOR", "=", value{ std::int64_t{0} }); 
        // engine.set_condition(31, "HELLO",          "=", value{ "hello" }); 
        // engine.set_condition<std::int64_t>(41, "TEST INDICATOR", "IN", { 2, 3, 5 }); 
    }

	// Definition of condition type 2 : direct API with string form (operand/operator/value)
    {
        // engine.set_condition_string(11, "'LATITUDE' < 42.0"); 
        // engine.set_condition_string(21, "'TEST INDICATOR' = 0");
        // engine.set_condition_string(31, "'HELLO' = 'hello'"); 
        // engine.set_condition_string(41, "'TEST INDICATOR' IN [2,3,5]"); 
    }

	// Definition of condition type 3 : JSON form, either directly from string or from file
    {
        // load conditions from the file copied into the working directory / build dir
        // [
        //   { "id":11,  "operand":"LATITUDE",       "operator":"<",  "value":42.0      },
        //   { "id":21,  "operand":"TEST INDICATOR",                  "value":0         },
        //   { "id":31,  "operand":"HELLO",                           "value":"hello"   },
        //   { "id":41,  "operand":"TEST INDICATOR", "operator":"IN", "value":[2, 3, 5] },
        // ]
        engine.set_conditions_from_file("conditions.json");
	}

	// Part 2 : Define rule and evaluate
    //  RULE_ID = 1000
    //  RULE_EXPR = (11 AND (41 OR 31) AND NOT 21)
    constexpr std::uint64_t RULE_ID = 1000;
    const std::string RULE_EXPR = "(11 AND (41 OR 31) AND NOT 21)";

    // store rule by id (parses and keeps the rule_node internally)
    try {
        engine.set_rule(RULE_ID, RULE_EXPR);
    } catch (const std::invalid_argument& ex) {
        std::cout << "failed to parse rule expression: " << ex.what() << "\n";
        return 1;
    }

	// Part 3 : Define subjects (key-value pairs) to evaluate the rule against
    rule_engine::subject_map subjects;

    // Use helper from subject_utils.hpp so the key string is written only once
    cond3::add_subject(subjects, "LATITUDE", value{ 38.5 }); // double match
    cond3::add_subject(subjects, "TEST INDICATOR", value{ std::int64_t{3} }); // integer match
    cond3::add_subject(subjects, "HELLO", value{ "hello" }); // string match

	// Part 4: Evaluate rule against subjects and print result

    // log callback for tracing
    engine.set_log_callback([](const std::string& msg) {
        std::cout << "[TRACE] " << msg << "\n";
        });

    // evaluate by rule id (delegates to stored rule_node)
    auto r = engine.evaluate_rule(RULE_ID, subjects);

    if (!r.ok) {
        // error can happen e.g. when a subject key is missing.
        std::cout << "rule => error: " << to_string(r.error) << "\n";
        return 1;
    }

    std::cout << "rule => " << (r.value ? "true" : "false") << "\n";
    return 0;
}
