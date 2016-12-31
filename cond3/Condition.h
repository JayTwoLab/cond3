#ifndef CONDITION_H
#define CONDITION_H

#include <list>
#include <map>
#include <vector>

#include "GlobalType.h"
#include "SubjectOfComparison.h"
#include "ConditionExpression.h"


class Condition
{
public:
    Condition();
    Condition( uint64_t number, CE ce );
    virtual ~Condition();
public:
    void setConditionExpression( uint64_t number, CE ce );
    void setCE( uint64_t number, CE ce );
public:
    bool evaluate( SOC soc );
    bool getResult(bool& conditionResult);
    uint64_t getConditionNumber();
    std::string getConditionExpressionOperand();
protected:
    ConditionExpression ce;
    uint64_t conditionNumber;
    bool isSetCE;
    bool result;
    bool isSetResult;
};

typedef Condition COND; // alias

typedef std::map<uint64_t, Condition> ConditionMap;
typedef ConditionMap CM; // alias

typedef std::pair<uint64_t, Condition> ConditionPair;
typedef ConditionPair CP; // alias

#endif // CONDITION_H
