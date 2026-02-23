#pragma once

#include "condition_filter.hpp"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <type_traits>
#include <initializer_list>
#include <functional>
#include <cstddef>

namespace cond3 {

// Rule tree node operations
enum class rule_op : std::uint8_t {
    leaf = 0,
    all_of,  // AND
    any_of,  // OR
    not_,    // NOT
};

// Node in the rule tree
class rule_node {
public:

    // Deep-copy support (required for std::initializer_list usage in MSVC)
    rule_node() = default;
    rule_node(const rule_node& other);
    rule_node& operator=(const rule_node& other);
    rule_node(rule_node&&) noexcept = default;
    rule_node& operator=(rule_node&&) noexcept = default;
    ~rule_node() = default;

    static rule_node make_leaf(std::uint64_t condition_id);
    static rule_node make_all_of(std::vector<rule_node> children);
    static rule_node make_any_of(std::vector<rule_node> children);
    static rule_node make_not(rule_node child);

    rule_op op() const { return op_; }
    std::uint64_t leaf_condition_id() const { return condition_id_; }
    const std::vector<rule_node>& children() const { return children_; }
    const rule_node* child() const { return child_.get(); }

private:
    rule_op op_{rule_op::leaf};
    std::uint64_t condition_id_{0};
    std::vector<rule_node> children_;
    std::unique_ptr<rule_node> child_;
};

// Rule engine
class rule_engine {
public:
    using subject_map = condition_filter::subject_map;

    void reset_conditions() { filter_.reset(); }

    // original form
    void set_condition(std::uint64_t id, condition_expression expr) { filter_.set_condition(id, std::move(expr)); }

    // Updated convenience overloads: operand comes before operator string
    void set_condition(std::uint64_t id, std::string operand, const std::string& op_str, value expected) {
        filter_.set_condition(id, std::move(operand), op_str, std::move(expected));
    }

    void set_condition(std::uint64_t id, std::string operand, const std::string& op_str, std::vector<value> expected_list) {
        filter_.set_condition(id, std::move(operand), op_str, std::move(expected_list));
    }

    // Template forwarder for initializer_list of arithmetic types (integral or floating).
    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    void set_condition(std::uint64_t id, std::string operand, const std::string& op_str, std::initializer_list<T> list) {
        filter_.set_condition(id, std::move(operand), op_str, list);
    }

    // String form convenience API
    void set_condition_string(std::uint64_t id, const std::string& cond_str) {
        filter_.set_condition_string(id, cond_str);
    }

    // Logging: set a callback to receive trace messages. If not set, no logs emitted.
    void set_log_callback(std::function<void(const std::string&)> cb) { logger_ = std::move(cb); }

    evaluate_result evaluate_rule(const rule_node& rule, const subject_map& subjects) const;

private:
    // depth: indentation level for tree-structured logs
    evaluate_result eval_node(
        const rule_node& node,
        const subject_map& subjects,
        std::unordered_map<std::uint64_t, evaluate_result>& cache,
        std::size_t depth = 0) const;

private:
    condition_filter filter_;

    // mutable so const evaluate functions can emit logs via callback
    mutable std::function<void(const std::string&)> logger_;
};

} // namespace cond3
