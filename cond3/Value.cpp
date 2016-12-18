//
// Value.cpp
//

#include "Value.h"

Value::Value()
{
    valueType = ValueType::NotSet;
}

// Value::~Value()
// {
// }


Value& Value::operator=(uint64_t num)
{
    this->numberValue = num;
    this->valueType = ValueType::Number;
    return (*this);
}

Value& Value::operator=(double realNum)
{
    this->realValue = realNum;
    this->valueType = ValueType::RealNumber;
    return (*this);
}

Value& Value::operator=(std::string str)
{
    this->stringValue = str;
    this->valueType = ValueType::String;
    return (*this);
}

ValueType Value::type()
{
    return this->valueType;
}

Value& Value::operator=(Value& other)
{
    ValueType vt = other.type();

    if ( vt == ValueType::NotSet )
    {
        this->valueType = ValueType::NotSet;
    }
    else if ( vt == ValueType::Number )
    {
        this->valueType   = ValueType::Number;
        this->numberValue = other.getNumber();
    }
    else if ( vt == ValueType::RealNumber )
    {
        this->valueType = ValueType::RealNumber;
        this->realValue = other.getRealNumber();
    }
    else if ( vt == ValueType::String )
    {
        this->valueType   = ValueType::String;
        this->stringValue = other.getString();
    }
    else
    {
        // undefined type
    }

    return (*this);
}

uint64_t Value::getNumber()
{
    if ( valueType == ValueType::Number )
        return numberValue;
    else
        return 0;
}

double Value::getRealNumber()
{
    if ( valueType == ValueType::RealNumber )
        return realValue;
    else
        return double(0);
}

std::string Value::getString()
{
    if ( valueType == ValueType::String )
    {
        return stringValue;
    }
    else
    {
        return (std::string(""));
    }
}

