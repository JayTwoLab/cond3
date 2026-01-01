#pragma once

#include "condition.hpp"
#include "subject.hpp"
#include "types.hpp"

namespace cond3 {

struct evaluate_result {
    bool ok{false};
    bool value{false};
    evaluate_error error{evaluate_error::none};
};

evaluate_result evaluate(const condition& cond, const subject& subj);

} // namespace cond3
