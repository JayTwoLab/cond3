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
#include "rule.hpp"
#include "types.hpp"

int main() {
    cond3::rule_engine engine;
    engine.set_condition(11, cond3::condition_expression{cond3::condition_operator::less_than, "LATITUDE", cond3::value{42.0}});
    // ... set more conditions ...

    // Build rule tree: (11 AND (41 OR 31) AND NOT 21)
    cond3::rule_node rule = cond3::rule_node::make_all_of({
        cond3::rule_node::make_leaf(11),
        // ...
    });

    // Prepare input subjects
    cond3::rule_engine::subject_map subjects;
    subjects.emplace("LATITUDE", cond3::subject{"LATITUDE", cond3::value{38.5}});
    // ...

    auto result = engine.evaluate_rule(rule, subjects);
    // result.value is true/false, result.ok indicates success, result.error gives error reason
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
