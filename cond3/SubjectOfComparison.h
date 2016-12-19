//
// SubjectOfComparison.h
//

#ifndef SUBJECTOFCOMPARISON_H
#define SUBJECTOFCOMPARISON_H

#include "GlobalType.h"
#include "Value.h"

class SubjectOfComparison
{
public:
    SubjectOfComparison();
public:
    SubjectOfComparison& operator=(SubjectOfComparison& soc);
public:
    std::string key;
    Value val;
};

typedef SubjectOfComparison SOC; // alias

#endif // SUBJECTOFCOMPARISON_H
