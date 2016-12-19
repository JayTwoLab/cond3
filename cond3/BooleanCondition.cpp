
#include "BooleanCondition.h"

//-----------------------------------------------------------------------
BooleanCondition::BooleanCondition()
{
    this->number = 0;
    this->isSet = false;

}

//-----------------------------------------------------------------------
void BooleanCondition::setCondition( uint64_t numberBC, BooleanOperator bo )
{
    this->number = numberBC;
    this->booleanOperator = bo;

}

//-----------------------------------------------------------------------
bool BooleanCondition::evaluate( bool first, bool second, bool& result )
{

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
