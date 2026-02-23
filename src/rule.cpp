#include "rule.hpp"

#include <sstream>
#include <string>

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

// helper: produce human-readable rule string (uses condition_filter for leaf text)
static std::string rule_node_to_string(const rule_node& node, const condition_filter& filter) {
    switch (node.op()) {
        case rule_op::leaf: {
            return filter.condition_to_string(node.leaf_condition_id());
        }
        case rule_op::not_: {
            const auto* ch = node.child();
            if (!ch) return "NOT <missing>";
            return std::string("NOT ") + "(" + rule_node_to_string(*ch, filter) + ")";
        }
        case rule_op::all_of: {
            std::ostringstream ss;
            ss << "(";
            for (std::size_t i = 0; i < node.children().size(); ++i) {
                if (i) ss << " AND ";
                ss << rule_node_to_string(node.children()[i], filter);
            }
            ss << ")";
            return ss.str();
        }
        case rule_op::any_of: {
            std::ostringstream ss;
            ss << "(";
            for (std::size_t i = 0; i < node.children().size(); ++i) {
                if (i) ss << " OR ";
                ss << rule_node_to_string(node.children()[i], filter);
            }
            ss << ")";
            return ss.str();
        }
        default:
            return "<unknown-op>";
    }
}

evaluate_result rule_engine::evaluate_rule(const rule_node& rule, const subject_map& subjects) const {
    std::unordered_map<std::uint64_t, evaluate_result> cache;

    auto log = [&](const std::string& s) {
        if (logger_) logger_(s);
    };

    // log human-readable rule expression
    {
        std::ostringstream ss;
        ss << "evaluate_rule: start";
        log(ss.str());
    }
    {
        std::ostringstream ss;
        ss << "rule_expr: " << rule_node_to_string(rule, filter_);
        log(ss.str());
    }

    auto res = eval_node(rule, subjects, cache, 0);
    {
        std::ostringstream ss;
        ss << "evaluate_rule: end -> ok=" << (res.ok ? "true" : "false")
           << " value=" << (res.value ? "true" : "false")
           << " error=" << to_string(res.error);
        log(ss.str());
    }
    return res;
}

evaluate_result rule_engine::eval_node(
    const rule_node& node,
    const subject_map& subjects,
    std::unordered_map<std::uint64_t, evaluate_result>& cache,
    std::size_t depth) const
{
    auto log = [&](const std::string& s) {
        if (logger_) logger_(s);
    };

    // indentation per depth (2 spaces per level)
    std::string pad(depth == 0 ? "" : std::string(depth * 2, ' '));

    // helper to show node type
    auto op_name = [&](rule_op op) {
        switch (op) {
            case rule_op::leaf: return "leaf";
            case rule_op::not_: return "not";
            case rule_op::all_of: return "all_of";
            case rule_op::any_of: return "any_of";
            default: return "unknown";
        }
    };

    {
        std::ostringstream ss;
        ss << pad << "eval_node: op=" << op_name(node.op());
        if (node.op() == rule_op::leaf) ss << " id=" << node.leaf_condition_id()
                                          << " (" << filter_.condition_to_string(node.leaf_condition_id()) << ")";
        log(ss.str());
    }

    switch (node.op()) {
        case rule_op::leaf: {
            const auto id = node.leaf_condition_id();

            auto it = cache.find(id);
            if (it != cache.end()) {
                std::ostringstream ss;
                ss << pad << "leaf[" << id << "] cache hit -> ok=" << (it->second.ok ? "true" : "false")
                   << " value=" << (it->second.value ? "true" : "false")
                   << " error=" << to_string(it->second.error)
                   << " (" << filter_.condition_to_string(id) << ")";
                log(ss.str());
                return it->second;
            }

            auto r = filter_.evaluate_one(id, subjects);
            {
                std::ostringstream ss;
                ss << pad << "leaf[" << id << "] evaluated -> ok=" << (r.ok ? "true" : "false")
                   << " value=" << (r.value ? "true" : "false")
                   << " error=" << to_string(r.error)
                   << " (" << filter_.condition_to_string(id) << ")";
                log(ss.str());
            }
            cache.emplace(id, r);
            return r;
        }

        case rule_op::not_: {
            const auto* ch = node.child();
            if (ch == nullptr) {
                log(pad + "not: missing child -> expression_not_set");
                return evaluate_result{false, false, evaluate_error::expression_not_set};
            }

            auto r = eval_node(*ch, subjects, cache, depth + 1);
            if (!r.ok) {
                std::ostringstream ss;
                ss << pad << "not: child error -> " << to_string(r.error);
                log(ss.str());
                return r;
            }

            std::ostringstream ss;
            ss << pad << "not: child value=" << (r.value ? "true" : "false") << " -> result=" << (!r.value ? "true" : "false");
            log(ss.str());
            return ok_bool(!r.value);
        }

        case rule_op::all_of: {
            bool saw_error = false;
            evaluate_error first_error = evaluate_error::none;

            {
                std::ostringstream ss;
                ss << pad << "all_of: children_count=" << node.children().size();
                log(ss.str());
            }

            // AND: if any child evaluates to false, return false immediately (takes precedence over errors)
            for (std::size_t i = 0; i < node.children().size(); ++i) {
                const auto& ch = node.children()[i];
                auto r = eval_node(ch, subjects, cache, depth + 1);

                {
                    std::ostringstream ss;
                    ss << pad << "all_of: child[" << i << "] -> ok=" << (r.ok ? "true" : "false")
                       << " value=" << (r.value ? "true" : "false")
                       << " error=" << to_string(r.error);
                    log(ss.str());
                }

                if (r.ok) {
                    if (!r.value) {
                        log(pad + "all_of: short-circuit -> false (child false)");
                        return ok_bool(false);
                    }
                } else {
                    if (!saw_error) {
                        saw_error = true;
                        first_error = r.error;
                    }
                }
            }

            if (saw_error) {
                std::ostringstream ss;
                ss << pad << "all_of: finished with error -> " << to_string(first_error);
                log(ss.str());
                return evaluate_result{false, false, first_error};
            }
            log(pad + "all_of: finished -> true");
            return ok_bool(true);
        }

        case rule_op::any_of: {
            bool saw_error = false;
            evaluate_error first_error = evaluate_error::none;

            {
                std::ostringstream ss;
                ss << pad << "any_of: children_count=" << node.children().size();
                log(ss.str());
            }

            // OR: if any child evaluates to true, return true immediately (takes precedence over errors)
            for (std::size_t i = 0; i < node.children().size(); ++i) {
                const auto& ch = node.children()[i];
                auto r = eval_node(ch, subjects, cache, depth + 1);

                {
                    std::ostringstream ss;
                    ss << pad << "any_of: child[" << i << "] -> ok=" << (r.ok ? "true" : "false")
                       << " value=" << (r.value ? "true" : "false")
                       << " error=" << to_string(r.error);
                    log(ss.str());
                }

                if (r.ok) {
                    if (r.value) {
                        log(pad + "any_of: short-circuit -> true (child true)");
                        return ok_bool(true);
                    }
                } else {
                    if (!saw_error) {
                        saw_error = true;
                        first_error = r.error;
                    }
                }
            }

            if (saw_error) {
                std::ostringstream ss;
                ss << pad << "any_of: finished with error -> " << to_string(first_error);
                log(ss.str());
                return evaluate_result{false, false, first_error};
            }
            log(pad + "any_of: finished -> false");
            return ok_bool(false);
        }

        default:
            log(pad + "eval_node: invalid operator");
            return evaluate_result{false, false, evaluate_error::invalid_operator};
    }
}

} // namespace cond3
