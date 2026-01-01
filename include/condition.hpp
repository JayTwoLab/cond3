#pragma once

#include "condition_expression.hpp"

#include <cstdint>
#include <utility>

namespace cond3 {

// Condition = (id + expression)
class condition {
public:
    condition() = default;
    explicit condition(std::uint64_t id, condition_expression expr)
        : id_(id), expr_(std::move(expr)) {}

    std::uint64_t id() const { return id_; }
    const condition_expression& expression() const { return expr_; }

private:
    std::uint64_t id_{0};
    condition_expression expr_{};
};

} // namespace cond3
