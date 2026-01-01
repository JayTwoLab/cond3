#pragma once

#include "types.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace cond3 {

// Value container
class value {
public:
    using storage_t = std::variant<std::monostate, std::uint64_t, double, std::string>;

    value() = default;
    explicit value(std::uint64_t v) : data_(v) {}
    explicit value(double v) : data_(v) {}
    explicit value(std::string v) : data_(std::move(v)) {}
    explicit value(const char* v) : data_(std::string(v)) {}

    value_type type() const;

    std::optional<std::uint64_t> as_number() const;
    std::optional<double> as_real_number() const;
    std::optional<std::string> as_string() const;

    const storage_t& raw() const { return data_; }

    friend bool operator==(const value& a, const value& b);
    friend bool operator>(const value& a, const value& b);
    friend bool operator>=(const value& a, const value& b);
    friend bool operator<(const value& a, const value& b);
    friend bool operator<=(const value& a, const value& b);

private:
    storage_t data_{};
};

} // namespace cond3
