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
