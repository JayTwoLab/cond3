#include "value.hpp"

namespace cond3 {

value_type value::type() const {
    if (std::holds_alternative<std::monostate>(data_)) {
        return value_type::not_set;
    }
    if (std::holds_alternative<std::uint64_t>(data_)) {
        return value_type::number;
    }
    if (std::holds_alternative<double>(data_)) {
        return value_type::real_number;
    }
    if (std::holds_alternative<std::string>(data_)) {
        return value_type::string;
    }
    return value_type::not_set;
}

std::optional<std::uint64_t> value::as_number() const {
    if (auto p = std::get_if<std::uint64_t>(&data_)) {
        return *p;
    }
    return std::nullopt;
}

std::optional<double> value::as_real_number() const {
    if (auto p = std::get_if<double>(&data_)) {
        return *p;
    }
    return std::nullopt;
}

std::optional<std::string> value::as_string() const {
    if (auto p = std::get_if<std::string>(&data_)) {
        return *p;
    }
    return std::nullopt;
}

static bool same_type(const value& a, const value& b) {
    return a.type() == b.type();
}

bool operator==(const value& a, const value& b) {
    if (!same_type(a, b)) {
        return false;
    }
    return a.data_ == b.data_;
}

bool operator>(const value& a, const value& b) {
    if (!same_type(a, b)) {
        return false;
    }

    switch (a.type()) {
        case value_type::number:
            return std::get<std::uint64_t>(a.data_) > std::get<std::uint64_t>(b.data_);
        case value_type::real_number:
            return std::get<double>(a.data_) > std::get<double>(b.data_);
        default:
            return false;
    }
}

bool operator>=(const value& a, const value& b) {
    if (!same_type(a, b)) {
        return false;
    }

    switch (a.type()) {
        case value_type::number:
            return std::get<std::uint64_t>(a.data_) >= std::get<std::uint64_t>(b.data_);
        case value_type::real_number:
            return std::get<double>(a.data_) >= std::get<double>(b.data_);
        default:
            return false;
    }
}

bool operator<(const value& a, const value& b) {
    if (!same_type(a, b)) {
        return false;
    }

    switch (a.type()) {
        case value_type::number:
            return std::get<std::uint64_t>(a.data_) < std::get<std::uint64_t>(b.data_);
        case value_type::real_number:
            return std::get<double>(a.data_) < std::get<double>(b.data_);
        default:
            return false;
    }
}

bool operator<=(const value& a, const value& b) {
    if (!same_type(a, b)) {
        return false;
    }

    switch (a.type()) {
        case value_type::number:
            return std::get<std::uint64_t>(a.data_) <= std::get<std::uint64_t>(b.data_);
        case value_type::real_number:
            return std::get<double>(a.data_) <= std::get<double>(b.data_);
        default:
            return false;
    }
}

} // namespace cond3
