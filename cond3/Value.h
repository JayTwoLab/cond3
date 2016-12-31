#ifndef VALUE_H
#define VALUE_H

#include "GlobalType.h"

class Value
{
public:
    Value();
    explicit Value(uint64_t num);
    explicit Value(double realNum);
    explicit Value(std::string str);
    virtual ~Value();
public:
    ValueType type();
public:
    uint64_t getNumber();
    double getRealNumber();
    std::string getString();
public:
    Value& operator=(Value& val);
    Value& operator=(uint64_t num);
    Value& operator=(double realNum);
    Value& operator=(std::string str);
public:
    bool operator==(Value& val);
    bool operator>(Value& val);
    bool operator>=(Value& val);
    bool operator<(Value& val);
    bool operator<=(Value& val);

protected:
    ValueType valueType;
protected:
    uint64_t numberValue;
    double realValue;
    std::string stringValue;
};
typedef Value VAL;

// #define VDN(DoubleNum)    Value( double(#DoubleNum) )

#define VN(Num)         Value( uint64_t( #Num ) )

#define VS(Str)         Value( std::string( #Str ) )

#endif // VALUE_H
