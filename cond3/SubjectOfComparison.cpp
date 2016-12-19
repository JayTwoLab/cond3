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
// operator=
SubjectOfComparison& SubjectOfComparison::operator=(SubjectOfComparison& soc)
{
    this->key = soc.key;
    this->val = soc.val;

    return (*this);
}

//-----------------------------------------------------------------------------

