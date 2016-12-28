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
    SubjectOfComparison& operator=(SubjectOfComparison& soc);
public:
    void setResult(bool res);
    bool getResult(bool& res);
public:
    std::string key;
    Value val;
protected:
    bool isSet;
    bool resultSOC;
};
typedef SubjectOfComparison SOC; // alias

typedef std::list<SubjectOfComparison> SOCList;

#endif // SUBJECTOFCOMPARISON_H
