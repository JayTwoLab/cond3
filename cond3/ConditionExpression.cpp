
#include "ConditionExpression.h"

ConditionExpression::ConditionExpression()
{
}

ConditionExpression::ConditionExpression( ConditionOperator ceOperator, std::string ceOperand, Value ceValue )
{
    this->op      = ceOperator;
    this->operand = ceOperand;
    this->val     = ceValue;
}

ConditionExpression::~ConditionExpression()
{

}

ConditionExpression& ConditionExpression::operator=(ConditionExpression ce)
{
    this->op      = ce.op;
    this->operand = ce.operand;
    this->val     = ce.val;

    return (*this);
}


