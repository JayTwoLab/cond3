#ifndef CONDITIONEXPRESSION_H
#define CONDITIONEXPRESSION_H

#include "GlobalType.h"
#include "Value.h"

class ConditionExpression
{
public:
    ConditionExpression();
public:
    ConditionExpression& operator=(ConditionExpression op);
public:
    ConditionOperator op;
    std::string operand;
    Value val;
};

typedef ConditionExpression CE; // alias

#endif // CONDITIONEXPRESSION_H
