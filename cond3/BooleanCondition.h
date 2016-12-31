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

    // void set( uint64_t number, uint64_t firstNO, BO bcOP, uint64_t secondNO );
    // bool getResult(bool& ret);

    uint64_t numberBC; // default is zero

    uint64_t firstNumber;
    bool isFirstCondType; // true: COND, false: BC

    uint64_t secondNumber;
    bool isSecondCondType; // true: COND, false: BC

    bool isFinal;

    BO Operator;

protected:
    bool result;

};

typedef BooleanCondition BC;

typedef std::list<BC> BCList;

#endif // BOOLEANCONDITION_H
