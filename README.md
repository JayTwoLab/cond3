# cond3_rule_tree

[Korean README](README.ko.md)

A C++17 library for building and evaluating complex rule trees using conditions and subjects. This project provides a flexible framework for defining conditions, grouping them into logical rule trees (AND, OR, NOT), and evaluating them against input data.

## Features

- **Condition Expressions**: Define conditions using operands, comparison operators, and expected values (single or list).
- **Rule Trees**: Compose conditions into rule trees using logical operations (AND, OR, NOT).
- **Evaluation Engine**: Evaluate rules against input subjects and get detailed results, including error reasons.
- **Extensible Types**: Supports numbers, real numbers, and strings as value types.
- **String-Operator Overloads**: Simplify condition definitions using string representations of operators.
- **Rule Parsing**: Parse rule trees from boolean expression strings for easier rule definition.

## Example Usage

### Using Condition Expressions

```cpp
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

// 
constexpr std::uint64_t RULE_ID = 1000;

// Type 2-1: define rule directly from string expression (parsing happens at set_rule time, and the parsed rule_node is stored internally for later evaluation)
{
	// const std::string RULE_EXPR = "(11 AND (41 OR 31) AND NOT 21)";
	// try {
	//  engine.set_rule(RULE_ID, RULE_EXPR);
	// } catch (const std::invalid_argument& ex) {
	//    std::cout << "failed to parse rule expression: " << ex.what() << "\n";
	//    return 1;
	// }
}

// type 2-2: load rule from external JSON file (parsing happens at load time, and the parsed rule_node is stored internally for later evaluation)
{
	// store rule by id (parses and keeps the rule_node internally)
	try {
		// Load rules from external JSON and register them in the engine.
		cond3::load_rules_from_file(engine, "rules.json");
	}
	catch (const std::invalid_argument& ex) {
		std::cout << "failed to parse rule expression: " << ex.what() << "\n";
		return 1;
	}
	catch (const std::exception& ex) {
		std::cout << "failed to load rules: " << ex.what() << "\n";
		return 1;
	}
}

// Part 3 : Define subjects (key-value pairs) to evaluate the rule against
rule_engine::subject_map subjects;

// Use helper from subject_utils.hpp so the key string is written only once
cond3::add_subject(subjects, "LATITUDE",       value{ 38.5 }); // double match
cond3::add_subject(subjects, "TEST INDICATOR", value{ std::int64_t{3} }); // integer match
cond3::add_subject(subjects, "HELLO",          value{ "hello" }); // string match

// Part 4: Evaluate rule against subjects and print result

// log callback for tracing
//  note: if you dont set a log callback, no logs will be emitted. This is optional and just for demonstration.
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
```

## Build Instructions

This project uses CMake (minimum version 3.16).

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## Requirements

- C++17 compatible compiler
- CMake 3.16 or higher

## License

[MIT License](LICENSE)
