#ifndef BOOLEANCONDITION_H
#define BOOLEANCONDITION_H

#include <cstdint>
#include <cstdio>

#include "GlobalType.h"

class BooleanCondition
{
public:
    BooleanCondition();
    virtual ~BooleanCondition();

public:
    void setCondition( uint64_t BCkey, BooleanOperator bo, bool isFinalCondition );

    // setKey( key )

    // setCondition1( SOCNumber soc, SOCNumber soc )

    // setCondition2( SOCNumber soc, BCNumber bc )

    // setCondition3( BCNumber bc1, BCNumber bc2 )


public:

    // bool evaluate( bool& result )

    bool evaluate(  bool first, bool second, bool& result );

    bool getResult( uint64_t& key, bool& out );

    // bool getConditionNumber( uint64_t& numberBC,  );

protected:
    uint64_t key;
    bool isSet;
    bool result;
    bool isFinal;
    BooleanOperator booleanOperator;
};

typedef BooleanCondition BC;

#endif // BOOLEANCONDITION_H
