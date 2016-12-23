//
// SubjectOfComparison.h
//

#ifndef SUBJECTOFCOMPARISON_H
#define SUBJECTOFCOMPARISON_H

#include <list>

#include "GlobalType.h"
#include "Value.h"

class SubjectOfComparison
{
public:
    SubjectOfComparison();
    SubjectOfComparison( std::string socKey, Value socValue );
    virtual ~SubjectOfComparison();
public:
    SubjectOfComparison& operator=(SubjectOfComparison& soc);
public:
    std::string key;
    Value val;
};
typedef SubjectOfComparison SOC; // alias

typedef std::list<SubjectOfComparison> SOCList;

#endif // SUBJECTOFCOMPARISON_H
