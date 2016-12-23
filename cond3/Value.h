#ifndef VALUE_H
#define VALUE_H

#include "GlobalType.h"

class Value
{
public:
    Value();
    Value(uint64_t num);
    Value(double realNum);
    Value(std::string str);
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

#define VN(Num)         Value( uint64_t( #Num ) )
#define VS(Str)         Value( std::string( #Str ) )
// #define VRN(RealNum)    Value( double( #RealNum ) )

#endif // VALUE_H
