#ifndef SUBJECTOFCOMPARISON_H
#define SUBJECTOFCOMPARISON_H

#include <cstdint>
#include <cstdio>
#include <string>

class SubjectOfComparison
{
public:
    SubjectOfComparison();

public:
    std::string key;

    enum ValueType {
        Number = 1,
        RealNumber = 2,
        String = 3,
    };
    SubjectOfComparison::ValueType valueType;

    uint64_t numberValue;
    double realValue;
    std::string stringValue;

protected:

};

#endif // SUBJECTOFCOMPARISON_H
