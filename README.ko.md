# cond3_rule_tree

[English README](README.md)

C++17 기반의 조건 및 대상(subject)을 활용한 복합 규칙 트리(rule tree) 생성 및 평가 라이브러리입니다. 이 프로젝트는 다양한 조건을 정의하고, 논리적 규칙 트리(AND, OR, NOT)로 그룹화하여 입력 데이터에 대해 평가할 수 있는 유연한 프레임워크를 제공합니다.

## 주요 특징

- **조건 표현식**: 피연산자, 비교 연산자, 기대값(단일 또는 리스트)을 사용하여 조건을 정의할 수 있습니다.
- **규칙 트리**: 논리 연산(AND, OR, NOT)을 활용해 조건들을 트리 구조로 조합할 수 있습니다.
- **평가 엔진**: 규칙을 입력 subject에 대해 평가하고, 오류 사유를 포함한 상세 결과를 제공합니다.
- **확장 가능한 타입**: 숫자, 실수, 문자열 타입을 지원합니다.

## 프로젝트 구조

- `include/`: 라이브러리의 공개 헤더 파일
  - `condition_expression.hpp`: 조건 표현식 구조 정의
  - `condition_filter.hpp`: 여러 조건 저장 및 평가
  - `condition.hpp`: 단일 조건 표현
  - `evaluator.hpp`: 조건 평가 로직
  - `rule.hpp`: 규칙 트리 노드 및 엔진 정의
  - `subject.hpp`: 평가 대상 데이터 구조
  - `types.hpp`: 열거형 및 타입 정의
  - `value.hpp`: 다양한 타입을 지원하는 값 컨테이너
- `src/`: 구현 파일
  - `main.cpp`: 예제 및 데모
  - 기타 `.cpp` 파일: 핵심 로직 구현

## 예제 코드

```cpp
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
```

## 빌드 방법

이 프로젝트는 CMake(최소 3.16 버전)를 사용합니다.

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## 요구 사항

- C++17 지원 컴파일러
- CMake 3.16 이상

## 라이선스

[MIT License](LICENSE)
