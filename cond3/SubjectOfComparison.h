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
    explicit SubjectOfComparison( uint64_t number, std::string socKey, Value socValue );
public:
    virtual ~SubjectOfComparison();
public:
    SubjectOfComparison& operator=(SubjectOfComparison& soc);
public:
    void setResult(bool res);
    bool getResult(bool& res);
    uint64_t getNumber();
public:
    std::string key;
    Value val;
protected:
    uint64_t numberSOC;
    bool isSet;
    bool resultSOC;
};

// alias

typedef SubjectOfComparison SOC;

typedef std::map<uint64_t, SOC> SOCMap;

typedef std::pair<uint64_t, SOC> SOCMapPair;

typedef SOCMapPair SOCMP;

#endif // SUBJECTOFCOMPARISON_H
