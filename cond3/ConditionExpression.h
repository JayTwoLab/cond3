#ifndef CONDITIONEXPRESSION_H
#define CONDITIONEXPRESSION_H

#include "GlobalType.h"
#include "Value.h"

class ConditionExpression
{
public:
    ConditionExpression();
public:
    ConditionOperator op;
    std::string operand;
    Value val;


};

#endif // CONDITIONEXPRESSION_H
