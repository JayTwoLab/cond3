# cond3_rule_tree

[Korean README](README.ko.md)

A C++17 library for building and evaluating complex rule trees using conditions and subjects. This project provides a flexible framework for defining conditions, grouping them into logical rule trees (AND, OR, NOT), and evaluating them against input data.

## Features

- **Condition Expressions**: Define conditions using operands, comparison operators, and expected values (single or list).
- **Rule Trees**: Compose conditions into rule trees using logical operations (AND, OR, NOT).
- **Evaluation Engine**: Evaluate rules against input subjects and get detailed results, including error reasons.
- **Extensible Types**: Supports numbers, real numbers, and strings as value types.

## Project Structure

- `include/`: Public headers for the library.
  - `condition_expression.hpp`: Defines the structure of a condition expression.
  - `condition_filter.hpp`: Stores and evaluates multiple conditions.
  - `condition.hpp`: Represents a single condition.
  - `evaluator.hpp`: Evaluation logic for conditions.
  - `rule.hpp`: Rule tree node and engine definitions.
  - `subject.hpp`: Input data structure for evaluation.
  - `types.hpp`: Enums and type definitions.
  - `value.hpp`: Value container supporting multiple types.
- `src/`: Implementation files.
  - `main.cpp`: Example usage and demo.
  - Other `.cpp` files: Implement the core logic.

## Example Usage

```cpp
using cond3::condition_expression;
using cond3::condition_operator;
using cond3::rule_engine;
using cond3::rule_node;
using cond3::to_string;
using cond3::value;

int main() {
    rule_engine engine;

    // Define conditions
    // 11: LATITUDE < 42
    engine.set_condition(11, condition_expression{condition_operator::less_than, "LATITUDE", value{42.0}});

    // 21: TEST INDICATOR == 0
    engine.set_condition(21, condition_expression{condition_operator::is_equal, "TEST INDICATOR", value{std::uint64_t{0}}});

    // 31: HELLO == "hello"
    engine.set_condition(31, condition_expression{condition_operator::is_equal, "HELLO", value{"hello"}});

    // 41: TEST INDICATOR IN [2,3,5]
    engine.set_condition(41, condition_expression{
        "TEST INDICATOR",
        std::vector<value>{value{std::uint64_t{2}}, value{std::uint64_t{3}}, value{std::uint64_t{5}}}
    });

    // Rule tree:
    // RULE = (11 AND (41 OR 31) AND NOT 21)
    rule_node rule = rule_node::make_all_of({
        rule_node::make_leaf(11),
        rule_node::make_any_of({
            rule_node::make_leaf(41),
            rule_node::make_leaf(31),
        }),
        rule_node::make_not(rule_node::make_leaf(21)),
    });

    // Input (subjects)
    rule_engine::subject_map subjects;
    subjects.emplace("LATITUDE", cond3::subject{"LATITUDE", value{38.5}});
    subjects.emplace("TEST INDICATOR", cond3::subject{"TEST INDICATOR", value{std::uint64_t{3}}});
    subjects.emplace("HELLO", cond3::subject{"HELLO", value{"hello"}});

    auto r = engine.evaluate_rule(rule, subjects);

    if (!r.ok) {
        std::cout << "rule => error: " << to_string(r.error) << "\n";
        return 1;
    }

    std::cout << "rule => " << (r.value ? "true" : "false") << "\n";
    return 0;
}
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
