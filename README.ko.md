# cond3_rule_tree

[English README](README.md)

C++17 기반의 조건 및 대상(subject)을 활용한 복합 규칙 트리(rule tree) 생성 및 평가 라이브러리입니다. 이 프로젝트는 다양한 조건을 정의하고, 논리적 규칙 트리(AND, OR, NOT)로 그룹화하여 입력 데이터에 대해 평가할 수 있는 유연한 프레임워크를 제공합니다.

## 주요 특징

- **조건 표현식**: 피연산자, 비교 연산자, 기대값(단일 또는 리스트)을 사용하여 조건을 정의할 수 있습니다.
- **규칙 트리**: 논리 연산(AND, OR, NOT)을 활용해 조건들을 트리 구조로 조합할 수 있습니다.
- **평가 엔진**: 규칙을 입력 subject에 대해 평가하고, 오류 사유를 포함한 상세 결과를 제공합니다.
- **확장 가능한 타입**: 숫자, 실수, 문자열 타입을 지원합니다.

## 예제 코드

다음 예제는 현재 코드베이스의 API를 반영합니다:
- `set_condition`의 문자열 연산자 오버로드(`"<"`, `"="`, `"IN"`) 사용
- `IN` 리스트는 템플릿/initializer_list로 전달 가능 (정수/실수 계열 지정)
- 규칙 트리는 문자열 파서를 통해 생성 (`parse_rule`)
- `add_subject` 헬퍼로 키 중복 방지

```cpp
rule_engine engine;

// Define conditions

// condition 11: LATITUDE < 42.0
engine.set_condition(11, "LATITUDE", "<", value{ 42.0 }); // double comparison

// condition 21: TEST INDICATOR == 0
engine.set_condition(21, "TEST INDICATOR", "=", value{ std::int64_t{0} }); // integer comparison

// condition 31: HELLO == "hello"
engine.set_condition(31, "HELLO", "=", value{ "hello" }); // string comparison

// condition 41: TEST INDICATOR IN [2,3,5]
engine.set_condition<std::int64_t>(41, "TEST INDICATOR", "IN", { 2, 3, 5 }); // integer IN list

// Rule tree: parse from string
// RULE = (11 AND (41 OR 31) AND NOT 21)
rule_node rule = cond3::parse_rule("(11 AND (41 OR 31) AND NOT 21)");

// Input (subjects)
rule_engine::subject_map subjects;

// Use helper from subject_utils.hpp so the key string is written only once
cond3::add_subject(subjects, "LATITUDE", value{ 38.5 }); // double match
cond3::add_subject(subjects, "TEST INDICATOR", value{ std::int64_t{3} }); // integer match
cond3::add_subject(subjects, "HELLO", value{ "hello" }); // string match

// Evaluate rule
auto r = engine.evaluate_rule(rule, subjects);

if (!r.ok) {
    // error can happen e.g. when a subject key is missing.
    std::cout << "rule => error: " << to_string(r.error) << "\n";
    return 1;
}

std::cout << "rule => " << (r.value ? "true" : "false") << "\n";
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
