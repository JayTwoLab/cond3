#ifndef VALUE_H
#define VALUE_H

#include "GlobalType.h"

class Value
{
public:
    Value();
    // virtual ~Value();
public:
    ValueType type();
public:
    Value& operator=(Value& val);
    Value& operator=(uint64_t num);
    Value& operator=(double realNum);
    Value& operator=(std::string str);
public:
    uint64_t getNumber();
    double getRealNumber();
    std::string getString();

protected:
    ValueType valueType;
protected:
    uint64_t numberValue;
    double realValue;
    std::string stringValue;
};

#endif // VALUE_H
