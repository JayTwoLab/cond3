#include "rule.hpp"
#include "types.hpp"

#include <iostream>
#include <vector>

using cond3::condition_expression;
using cond3::condition_operator;
using cond3::rule_engine;
using cond3::rule_node;
using cond3::to_string;
using cond3::value;

int main() {
    rule_engine engine;

    // Define conditions
    // 11: LATITUDE < 42
    engine.set_condition(11, condition_expression{condition_operator::less_than, "LATITUDE", value{42.0}});

    // 21: TEST INDICATOR == 0
    engine.set_condition(21, condition_expression{condition_operator::is_equal, "TEST INDICATOR", value{std::uint64_t{0}}});

    // 31: HELLO == "hello"
    engine.set_condition(31, condition_expression{condition_operator::is_equal, "HELLO", value{"hello"}});

    // 41: TEST INDICATOR IN [2,3,5]
    engine.set_condition(41, condition_expression{
        "TEST INDICATOR",
        std::vector<value>{value{std::uint64_t{2}}, value{std::uint64_t{3}}, value{std::uint64_t{5}}}
    });

    // Rule tree:
    // RULE = (11 AND (41 OR 31) AND NOT 21)
    rule_node rule = rule_node::make_all_of({
        rule_node::make_leaf(11),
        rule_node::make_any_of({
            rule_node::make_leaf(41),
            rule_node::make_leaf(31),
        }),
        rule_node::make_not(rule_node::make_leaf(21)),
    });

    // Input (subjects)
    rule_engine::subject_map subjects;
    subjects.emplace("LATITUDE", cond3::subject{"LATITUDE", value{38.5}});
    subjects.emplace("TEST INDICATOR", cond3::subject{"TEST INDICATOR", value{std::uint64_t{3}}});
    subjects.emplace("HELLO", cond3::subject{"HELLO", value{"hello"}});

    auto r = engine.evaluate_rule(rule, subjects);

    if (!r.ok) {
        std::cout << "rule => error: " << to_string(r.error) << "\n";
        return 1;
    }

    std::cout << "rule => " << (r.value ? "true" : "false") << "\n";
    return 0;
}
