//
// SubjectOfComparison.h
//

#ifndef SUBJECTOFCOMPARISON_H
#define SUBJECTOFCOMPARISON_H

#include <list>
#include <map>

#include "GlobalType.h"
#include "Value.h"

class SubjectOfComparison
{
public:
    SubjectOfComparison();
    explicit SubjectOfComparison( std::string socKey, Value socValue );
public:
    virtual ~SubjectOfComparison();
public:
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

// alias

typedef SubjectOfComparison SOC;

typedef std::map<std::string, SOC> SOCMap;

typedef std::pair<std::string, SOC> SOCMapPair;

typedef SOCMapPair SOCMP;

#endif // SUBJECTOFCOMPARISON_H
