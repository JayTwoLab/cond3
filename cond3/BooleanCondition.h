#ifndef BOOLEANCONDITION_H
#define BOOLEANCONDITION_H

#include <cstdint>
#include <cstdio>

#include "GlobalType.h"

class BooleanCondition
{
public:
    BooleanCondition();
public:
    void setCondition( uint64_t numberBC, BooleanOperator bo );
public:
    bool evaluate(  bool first, bool second, bool& result );
    bool getResult( uint64_t& num, bool& out );
protected:
    uint64_t number;
    bool isSet;
    bool result;
    bool isFinal;
    BooleanOperator booleanOperator;
};

typedef BooleanCondition BC;

#endif // BOOLEANCONDITION_H
