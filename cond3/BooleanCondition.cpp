
#include "BooleanCondition.h"

//-----------------------------------------------------------------------
BooleanCondition::BooleanCondition()
{
    this->number = 0;
    this->isSet = false;
}

//-----------------------------------------------------------------------
BooleanCondition::~BooleanCondition()
{

}

//-----------------------------------------------------------------------
void BooleanCondition::setCondition( uint64_t numberBC, BooleanOperator bo, bool isFinalCondition )
{
    this->number          = numberBC;
    this->booleanOperator = bo;
    this->isFinal         = isFinalCondition;
}

//-----------------------------------------------------------------------
bool BooleanCondition::evaluate( bool first, bool second, bool& result )
{
    if ( 0 == this->number )
    {
        return false;
    }

    switch ( booleanOperator )
    {
        case BooleanOperator::And :
        {
            result = first && second;
        }
        break;

    case BooleanOperator::Or :
        {
            result = first || second;
        }
        break;

    default:
        {
            return false;
        }
    }

    isSet = true;
    return true;
}

//-----------------------------------------------------------------------
bool BooleanCondition::getResult(uint64_t& num, bool& out)
{
    if ( !isSet )
        return false;

    out = result;
    num = number;

    return true;
}
