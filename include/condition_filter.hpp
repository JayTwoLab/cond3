#pragma once

#include "condition.hpp"
#include "evaluator.hpp"
#include "subject.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <type_traits>
#include <initializer_list>
#include <vector>
#include <optional>

namespace cond3 {

// Utility that stores Conditions and evaluates inputs (subjects)
class condition_filter {
public:
    using condition_map = std::map<std::uint64_t, condition>;
    using subject_map = std::map<std::string, subject>;

    void reset();
    void set_condition(std::uint64_t id, condition_expression expr);

    // Revised overloads: operand comes before operator string
    void set_condition(std::uint64_t id, std::string operand, const std::string& op_str, value expected);
    void set_condition(std::uint64_t id, std::string operand, const std::string& op_str, std::vector<value> expected_list);

    // Template overload: accept initializer_list of arithmetic types (integral or floating).
    // - Integral -> stored as uint64_t values
    // - Floating -> stored as double values
    // This is defined in the header because it's a template.
    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    void set_condition(std::uint64_t id, std::string operand, const std::string& op_str, std::initializer_list<T> list) {
        std::vector<value> vals;
        vals.reserve(list.size());
        if constexpr (std::is_integral_v<T>) {
            for (T v : list) {
                vals.emplace_back(static_cast<std::uint64_t>(v));
            }
        } else { // floating point
            for (T v : list) {
                vals.emplace_back(static_cast<double>(v));
            }
        }
        set_condition(id, std::move(operand), op_str, std::move(vals));
    }

    // Parse a condition from a simple string. Supported forms:
    //   "'KEY' < 42.0"
    //   "'KEY' = 'str'"
    //   "'KEY' IN [1,2,3]"
    //   "'KEY' 'str'"  // equality shorthand (no operator)
    void set_condition_string(std::uint64_t id, const std::string& cond_str);

    std::map<std::uint64_t, evaluate_result> evaluate_all(const subject_map& subjects) const;

    // Used for lazy evaluation in a rule tree
    evaluate_result evaluate_one(std::uint64_t condition_id, const subject_map& subjects) const;

    // Human readable representation of a condition (for logging)
    std::string condition_to_string(std::uint64_t condition_id) const;

private:
    condition_map conditions_;
};

} // namespace cond3
