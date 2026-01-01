#include "rule.hpp"

namespace cond3 {

rule_node::rule_node(const rule_node& other)
    : op_(other.op_), condition_id_(other.condition_id_), children_(other.children_) {
    if (other.child_) {
        child_ = std::make_unique<rule_node>(*other.child_);
    }
}

rule_node& rule_node::operator=(const rule_node& other) {
    if (this == &other) {
        return *this;
    }

    op_ = other.op_;
    condition_id_ = other.condition_id_;
    children_ = other.children_;

    if (other.child_) {
        child_ = std::make_unique<rule_node>(*other.child_);
    }
    else {
        child_.reset();
    }

    return *this;
}


rule_node rule_node::make_leaf(std::uint64_t condition_id) {
    rule_node n;
    n.op_ = rule_op::leaf;
    n.condition_id_ = condition_id;
    return n;
}

rule_node rule_node::make_all_of(std::vector<rule_node> children) {
    rule_node n;
    n.op_ = rule_op::all_of;
    n.children_ = std::move(children);
    return n;
}

rule_node rule_node::make_any_of(std::vector<rule_node> children) {
    rule_node n;
    n.op_ = rule_op::any_of;
    n.children_ = std::move(children);
    return n;
}

rule_node rule_node::make_not(rule_node child) {
    rule_node n;
    n.op_ = rule_op::not_;
    n.child_ = std::make_unique<rule_node>(std::move(child));
    return n;
}

static evaluate_result ok_bool(bool v) {
    return evaluate_result{true, v, evaluate_error::none};
}

evaluate_result rule_engine::evaluate_rule(const rule_node& rule, const subject_map& subjects) const {
    std::unordered_map<std::uint64_t, evaluate_result> cache;
    return eval_node(rule, subjects, cache);
}

evaluate_result rule_engine::eval_node(
    const rule_node& node,
    const subject_map& subjects,
    std::unordered_map<std::uint64_t, evaluate_result>& cache) const
{
    switch (node.op()) {
        case rule_op::leaf: {
            const auto id = node.leaf_condition_id();

            auto it = cache.find(id);
            if (it != cache.end()) {
                return it->second;
            }

            auto r = filter_.evaluate_one(id, subjects);
            cache.emplace(id, r);
            return r;
        }

        case rule_op::not_: {
            const auto* ch = node.child();
            if (ch == nullptr) {
                return evaluate_result{false, false, evaluate_error::expression_not_set};
            }

            auto r = eval_node(*ch, subjects, cache);
            if (!r.ok) {
                return r;
            }

            return ok_bool(!r.value);
        }

        case rule_op::all_of: {
            bool saw_error = false;
            evaluate_error first_error = evaluate_error::none;

            // AND: if any child evaluates to false, return false immediately (takes precedence over errors)
            for (const auto& ch : node.children()) {
                auto r = eval_node(ch, subjects, cache);

                if (r.ok) {
                    if (!r.value) {
                        return ok_bool(false);
                    }
                } else {
                    // Even if an error occurs, continue because another child producing false can determine the final false
                    if (!saw_error) {
                        saw_error = true;
                        first_error = r.error;
                    }
                }
            }

            if (saw_error) {
                return evaluate_result{false, false, first_error};
            }
            return ok_bool(true);
        }

        case rule_op::any_of: {
            bool saw_error = false;
            evaluate_error first_error = evaluate_error::none;

            // OR: if any child evaluates to true, return true immediately (takes precedence over errors)
            for (const auto& ch : node.children()) {
                auto r = eval_node(ch, subjects, cache);

                if (r.ok) {
                    if (r.value) {
                        return ok_bool(true);
                    }
                } else {
                    // Even if an error occurs, continue because another child producing true can determine the final true
                    if (!saw_error) {
                        saw_error = true;
                        first_error = r.error;
                    }
                }
            }

            if (saw_error) {
                return evaluate_result{false, false, first_error};
            }
            return ok_bool(false);
        }

        default:
            return evaluate_result{false, false, evaluate_error::invalid_operator};
    }
}

} // namespace cond3
