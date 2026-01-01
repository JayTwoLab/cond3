cmake_minimum_required(VERSION 3.16)
project(cond3_rule_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cond3
    src/value.cpp
    src/evaluator.cpp
    src/condition_filter.cpp
    src/rule.cpp
)

target_include_directories(cond3 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(cond3 PUBLIC cxx_std_17)

add_executable(cond3_demo
    src/main.cpp
)

target_link_libraries(cond3_demo PRIVATE cond3)
