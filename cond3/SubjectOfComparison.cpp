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

    return (*this);
}

//-----------------------------------------------------------------------------

