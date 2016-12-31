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

    uint64_t numberBC;

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
