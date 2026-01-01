#pragma once

#include "condition.hpp"
#include "evaluator.hpp"
#include "subject.hpp"

#include <cstdint>
#include <map>
#include <string>

namespace cond3 {

// Utility that stores Conditions and evaluates inputs (subjects)
class condition_filter {
public:
    using condition_map = std::map<std::uint64_t, condition>;
    using subject_map = std::map<std::string, subject>;

    void reset();
    void set_condition(std::uint64_t id, condition_expression expr);

    std::map<std::uint64_t, evaluate_result> evaluate_all(const subject_map& subjects) const;

    // Used for lazy evaluation in a rule tree
    evaluate_result evaluate_one(std::uint64_t condition_id, const subject_map& subjects) const;

private:
    condition_map conditions_;
};

} // namespace cond3
