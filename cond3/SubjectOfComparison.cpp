// SubjectOfComparison.cpp

#include "SubjectOfComparison.h"

#include <iostream>
#include <string>
using namespace std;

//-----------------------------------------------------------------------------
// constructor
SubjectOfComparison::SubjectOfComparison()
{
    isSet = false;

    numberSOC = 0;
    this->key.empty();
}

//-----------------------------------------------------------------------------
SubjectOfComparison::SubjectOfComparison( uint64_t number, std::string socKey, Value socValue )
{
    this->numberSOC = number;

    this->key = socKey;

    this->val = socValue;
}

//-----------------------------------------------------------------------------
SubjectOfComparison::~SubjectOfComparison()
{
}

//-----------------------------------------------------------------------------
// operator=
SubjectOfComparison& SubjectOfComparison::operator=( SubjectOfComparison& soc )
{
    this->key = soc.key;

    this->val = soc.val;

    this->numberSOC = soc.numberSOC;

    this->isSet = soc.isSet;

    this->resultSOC = soc.resultSOC;

    return (*this);
}

//-----------------------------------------------------------------------------
void SubjectOfComparison::setResult(bool res)
{
    isSet = true;
    this->resultSOC = res;
}

//-----------------------------------------------------------------------------
bool SubjectOfComparison::getResult(bool& res)
{
    if ( !this->isSet )
        return false;

    res = this->resultSOC;

    return true;
}

//-----------------------------------------------------------------------------
uint64_t SubjectOfComparison::getNumber()
{
    return (this->numberSOC);
}
