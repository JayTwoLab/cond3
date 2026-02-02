#pragma once

#include "types.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <type_traits>

namespace cond3 {

// Value container
class value {
public:
    using storage_t = std::variant<std::monostate, std::uint64_t, double, std::string>;

    value() = default;

    // Exact-match constructors
    explicit value(std::uint64_t v) : data_(v) {}
    explicit value(double v) : data_(v) {}
    explicit value(std::string v) : data_(std::move(v)) {}
    explicit value(const char* v) : data_(std::string(v)) {}

    // Single template constructor for other arithmetic types (int, int64_t, float, ...)
    // - integral types -> stored as uint64_t
    // - floating types  -> stored as double
    template<typename T, typename = std::enable_if_t<
        std::is_arithmetic<T>::value &&
        !std::is_same<T, std::uint64_t>::value &&
        !std::is_same<T, double>::value>>
    explicit value(T v) {
        if constexpr (std::is_integral_v<T>) {
            data_ = static_cast<std::uint64_t>(v);
        } else {
            data_ = static_cast<double>(v);
        }
    }

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
