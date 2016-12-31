
#include "BooleanCondition.h"

//-----------------------------------------------------------------------
BooleanCondition::BooleanCondition()
{
    this->key = 0;
    this->isSet = false;
}

//-----------------------------------------------------------------------
BooleanCondition::~BooleanCondition()
{

}

//-----------------------------------------------------------------------
void BooleanCondition::setCondition( uint64_t BCkey, BooleanOperator bo, bool isFinalCondition )
{
    this->key          = BCkey;
    this->booleanOperator = bo;
    this->isFinal         = isFinalCondition;
}

//-----------------------------------------------------------------------
bool BooleanCondition::evaluate( bool first, bool second, bool& result )
{
    if ( 0 == this->key )
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
bool BooleanCondition::getResult(uint64_t& BCKey, bool& out)
{
    if ( !isSet )
        return false;

    out = result;
    BCKey = key;

    return true;
}

//-----------------------------------------------------------------------
