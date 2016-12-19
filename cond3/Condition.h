#ifndef CONDITION_H
#define CONDITION_H

#include "GlobalType.h"
#include "SubjectOfComparison.h"
#include "ConditionExpression.h"

class Condition
{
public:
    Condition();
public:
    void setConditionExpression( uint64_t number, CE ce );
    void setCE( uint64_t number, CE ce );
public:
    bool compare( SOC soc );
    bool getResult(bool& conditionResult);
protected:
    ConditionExpression ce;
    uint64_t conditionNumber;
    bool isSetCE;
    bool result;
    bool isSetResult;
};

typedef Condition COND; // alias

#endif // CONDITION_H
