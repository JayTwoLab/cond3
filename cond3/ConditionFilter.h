#ifndef CONDITIONFILTER_H
#define CONDITIONFILTER_H

#include "GlobalType.h"
#include "Value.h"
#include "SubjectOfComparison.h"
#include "ConditionExpression.h"
#include "Condition.h"
#include "BooleanCondition.h"

class ConditionFilter
{
public:
    ConditionFilter();

public:
    void resetCondition();

    void setNumberCondition( uint64_t numberCOND, CO condOp, std::string condKey, double condValue );
    void setRealNumberCondition( uint64_t numberCOND, CO condOp, std::string condKey, uint64_t condValue );
    void setStringCondition( uint64_t numberCOND, CO condOp, std::string condKey, std::string condValue );

    // insertBC( uint64_t numberBC, first, BO, second );

    bool evaluate( SOCMap mapSOC );

protected:
    ConditionMap mapCondition;

    // BCList

};

#endif // CONDITIONFILTER_H
