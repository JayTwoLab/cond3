//
// Condition.cpp

#include "Condition.h"

#include <string>
#include <iostream>
using namespace std;

//----------------------------------------------------------------------
Condition::Condition()
{
    isSetCE = false;
    isSetResult = false;
}

//----------------------------------------------------------------------
void Condition::setConditionExpression( uint64_t number, CE conditionExpression )
{
    this->ce = conditionExpression;
    this->conditionNumber = number;
    isSetCE = true;
}

//----------------------------------------------------------------------
void Condition::setCE( uint64_t number, CE ce )
{
    setConditionExpression( number, ce );
}

//----------------------------------------------------------------------
bool Condition::compare( SOC soc )
{
    if ( !isSetCE )
    {
        return false; // condition expression is not set
    }

    ConditionOperator coOP  = ce.op;
    string strOperand       = ce.operand;
    Value valCE             = ce.val;

    string strKey = soc.key;
    Value  valSOC = soc.val;

    if ( strKey != strOperand )
    {
        return false; // different key
    }

    if ( valCE.type() != valSOC.type() )
    {
        return false; // different type
    }

    // valueSOC         Operator   valueCE

    switch ( coOP )
    {
    case ConditionOperator::isEqual : // ==
        {
            if ( valSOC == valCE )
            {
                result = true;
            }
            else
            {
                result = false;
            }
            isSetResult = true;
            return result;
        }
        break;

    case ConditionOperator::greaterThan : // >
        {
            if ( valSOC > valCE )
            {
                result = true;
            }
            else
            {
                result = false;
            }
            isSetResult = true;
            return result;
        }
        break;

    case ConditionOperator::greaterEqualThan : // >=
        {
            if ( valSOC >= valCE )
            {
                result = true;
            }
            else
            {
                result = false;
            }
            isSetResult = true;
            return result;
        }
        break;

    case ConditionOperator::lessThan : // <
        {
            if ( valSOC < valCE )
            {
                result = true;
            }
            else
            {
                result = false;
            }
            isSetResult = true;
            return result;
        }
        break;

    case ConditionOperator::lessEqualThan : // <=
        {
            if ( valSOC <= valCE )
            {
                result = true;
            }
            else
            {
                result = false;
            }
            isSetResult = true;
            return result;
        }
        break;

    default:
        {
            return false; // undefined operator
        }
    }

    return true;
}

//----------------------------------------------------------------------
bool Condition::getResult(bool& conditionResult)
{
    if ( !isSetCE )
    {
        return false; // condition expression is not set
    }

    if ( !isSetResult )
        return false;

    conditionResult = result;
    return true;
}

//----------------------------------------------------------------------
