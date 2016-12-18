#ifndef GLOBALTYPE_H
#define GLOBALTYPE_H

#include <cstdint>
#include <cstdio>
#include <string>

enum ValueType {
    NotSet     = 0,
    Number     = 1,
    RealNumber = 2,
    String     = 3,
};

enum ConditionOperator {
    equal            = 1,
    greaterThan      = 2,
    greaterEqualThan = 3,
    lessThan         = 4,
    lessEqualThan    = 5,
};

#endif // GLOBALTYPE_H
