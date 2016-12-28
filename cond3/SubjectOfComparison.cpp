//
// SubjectOfComparison.cpp
//

#include "SubjectOfComparison.h"

#include <iostream>
#include <string>
using namespace std;

//-----------------------------------------------------------------------------
// constructor
SubjectOfComparison::SubjectOfComparison()
{
    isSet = false;
}

//-----------------------------------------------------------------------------
SubjectOfComparison::SubjectOfComparison( std::string socKey, Value socValue )
{
    this->key = socKey;
    this->val = socValue;
}

//-----------------------------------------------------------------------------
SubjectOfComparison::~SubjectOfComparison()
{

}

//-----------------------------------------------------------------------------
// operator=
SubjectOfComparison& SubjectOfComparison::operator=(SubjectOfComparison& soc)
{
    this->key = soc.key;
    this->val = soc.val;

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
