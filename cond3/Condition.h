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
    bool setConditionExpression( ConditionExpression ce );
    bool setCE( ConditionExpression ce );

    // setSOC( soc )
    // setSubjectOfComparison( soc )

    // bool compare()
    // bool compare( soc )

protected:
    ConditionExpression ce;
    // soc

};

#endif // CONDITION_H
