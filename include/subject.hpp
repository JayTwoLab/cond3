#pragma once

#include "value.hpp"

#include <string>
#include <utility>

namespace cond3 {

// Comparison target (key + actual value)
struct subject {
    std::string key;
    value actual;

    subject() = default;
    subject(std::string key_, value actual_) : key(std::move(key_)), actual(std::move(actual_)) {}
};

} // namespace cond3
