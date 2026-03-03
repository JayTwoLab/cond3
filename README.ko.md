# cond3_rule_tree

[English README](README.md)

C++17 기반의 조건 및 대상(`subject`)을 활용한 복합 규칙 트리(`rule tree`) 생성 및 평가 라이브러리입니다. 이 프로젝트는 다양한 조건을 정의하고, 논리적 규칙 트리(AND, OR, NOT)로 그룹화하여 입력 데이터에 대해 평가할 수 있는 유연한 프레임워크를 제공합니다.

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

// Part 1: Define conditions
// 
// Define conditions
//  condition 11: LATITUDE < 42.0           // double comparison
//  condition 21: TEST INDICATOR == 0       // integer comparison
//  condition 31: HELLO == 'hello'          // string comparison
//  condition 41: TEST INDICATOR IN [2,3,5] // integer IN list

// Definition of condition type 1 : direct API with typed value
{
    // engine.set_condition(11, "LATITUDE",       "<", value{ 42.0 }); 
    // engine.set_condition(21, "TEST INDICATOR", "=", value{ std::int64_t{0} }); 
    // engine.set_condition(31, "HELLO",          "=", value{ "hello" }); 
    // engine.set_condition<std::int64_t>(41, "TEST INDICATOR", "IN", { 2, 3, 5 }); 
}

// Definition of condition type 2 : direct API with string form (operand/operator/value)
{
    // engine.set_condition_string(11, "'LATITUDE' < 42.0"); 
    // engine.set_condition_string(21, "'TEST INDICATOR' = 0");
    // engine.set_condition_string(31, "'HELLO' = 'hello'"); 
    // engine.set_condition_string(41, "'TEST INDICATOR' IN [2,3,5]"); 
}

// Definition of condition type 3 : JSON form, either directly from string or from file
{
    // load conditions from the file copied into the working directory / build dir
    // [
    //   { "id":11,  "operand":"LATITUDE",       "operator":"<",  "value":42.0      },
    //   { "id":21,  "operand":"TEST INDICATOR",                  "value":0         },
    //   { "id":31,  "operand":"HELLO",                           "value":"hello"   },
    //   { "id":41,  "operand":"TEST INDICATOR", "operator":"IN", "value":[2, 3, 5] },
    // ]
    engine.set_conditions_from_file("conditions.json");
}

// Part 2 : Define rule and evaluate

// 
constexpr std::uint64_t RULE_ID = 1000;

// Type 2-1: define rule directly from string expression (parsing happens at set_rule time, and the parsed rule_node is stored internally for later evaluation)
{
    // const std::string RULE_EXPR = "(11 AND (41 OR 31) AND NOT 21)";
    // try {
    //  engine.set_rule(RULE_ID, RULE_EXPR);
    // } catch (const std::invalid_argument& ex) {
    //    std::cout << "failed to parse rule expression: " << ex.what() << "\n";
    //    return 1;
    // }
}

// type 2-2: load rule from external JSON file (parsing happens at load time, and the parsed rule_node is stored internally for later evaluation)
{
    // store rule by id (parses and keeps the rule_node internally)
    try {
        // Load rules from external JSON and register them in the engine.
        cond3::load_rules_from_file(engine, "rules.json");
    }
    catch (const std::invalid_argument& ex) {
        std::cout << "failed to parse rule expression: " << ex.what() << "\n";
        return 1;
    }
    catch (const std::exception& ex) {
        std::cout << "failed to load rules: " << ex.what() << "\n";
        return 1;
    }
}

// Part 3 : Define subjects (key-value pairs) to evaluate the rule against
rule_engine::subject_map subjects;

// Use helper from subject_utils.hpp so the key string is written only once
cond3::add_subject(subjects, "LATITUDE",       value{ 38.5 }); // double match
cond3::add_subject(subjects, "TEST INDICATOR", value{ std::int64_t{3} }); // integer match
cond3::add_subject(subjects, "HELLO",          value{ "hello" }); // string match

// Part 4: Evaluate rule against subjects and print result

// log callback for tracing
//  note: if you dont set a log callback, no logs will be emitted. This is optional and just for demonstration.
engine.set_log_callback([](const std::string& msg) {
    std::cout << "[TRACE] " << msg << "\n";
    });

// evaluate by rule id (delegates to stored rule_node)
auto r = engine.evaluate_rule(RULE_ID, subjects);

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
