
#include "ConditionExpression.h"

ConditionExpression::ConditionExpression()
{

}

ConditionExpression& ConditionExpression::operator=(ConditionExpression ce)
{
    this->op      = ce.op;
    this->operand = ce.operand;
    this->val     = ce.val;

    return (*this);
}


