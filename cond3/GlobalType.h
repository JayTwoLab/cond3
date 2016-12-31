#ifndef GLOBALTYPE_H
#define GLOBALTYPE_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <string>
#include <map>
#include <list>
#include <vector>
#include <iostream>

enum ValueType {
    NotSet     = 0,
    Number     = 1,
    RealNumber = 2,
    String     = 3,
};
typedef ValueType VT;

enum ConditionOperator {
    isEqual          = 1,
    greaterThan      = 2,
    greaterEqualThan = 3,
    lessThan         = 4,
    lessEqualThan    = 5,
};
typedef ConditionOperator CO;

enum BooleanOperator {
    And = 1,
    Or = 2,
};
typedef BooleanOperator BO;

#endif // GLOBALTYPE_H
