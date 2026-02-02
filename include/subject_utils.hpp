#pragma once

#include "condition_filter.hpp"
#include "subject.hpp"

#include <string>

namespace cond3 {

// Add a subject to the subject_map using the same key for the map and the subject.
// This is a tiny helper to avoid repeating the key literal in client code.
inline void add_subject(condition_filter::subject_map& subjects, const std::string& key, value val) {
    subjects.emplace(key, subject{key, std::move(val)});
}

} // namespace cond3