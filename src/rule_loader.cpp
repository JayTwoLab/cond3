#include "rule_loader.hpp"

#include <fstream>
#include <stdexcept>

// nlohmann/json single header
#include <nlohmann/json.hpp>

namespace cond3 {

void load_rules_from_file(rule_engine& engine, const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) {
        throw std::runtime_error("failed to open rules file: " + path);
    }

    nlohmann::json j;
    ifs >> j;

    if (!j.is_array()) {
        throw std::invalid_argument("rules file must be a JSON array");
    }

    for (const auto& item : j) {
        // require fields: id (number) and expr (string)
        auto id = item.at("id").get<std::uint64_t>();
        auto expr = item.at("expr").get<std::string>();

        // parse and store rule (delegates to rule_engine::set_rule)
        engine.set_rule(id, expr);
    }
}

} // namespace cond3