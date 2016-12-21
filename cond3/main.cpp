//
// main.cpp
//
// #include <QCoreApplication>

#include <list>
#include <vector>
#include <map>
#include <string>
#include <iostream>
using namespace std;

#include "Value.h"
#include "SubjectOfComparison.h"
#include "ConditionExpression.h"
#include "Condition.h"
#include "BooleanCondition.h"

void TestValue();
void TestCondition();
void TestBooleanCondition();

int main(int argc, char *argv[])
{ // QCoreApplication a(argc, argv);

    // TestValue();
    // TestCondition();
    TestBooleanCondition();

    return 0; // return a.exec();
}

//---------------------------------------------
void TestBooleanCondition()
{
    // TODO:

    // 0) define list of condition (LOC)

    // 1) define BooleanCondition (BC)
    BooleanCondition bc;
    bc.setCondition( 1, BooleanOperator::And );

    bool first  = true;
    bool second = false;

    bool result;
    if ( bc.evaluate( first, second, result ) )
    {

    }

    // 2) define list of BooleanCondition (BC)


}

//---------------------------------------------
void TestCondition()
{
    // usage of condition

    //  1) set condition to condition expression
    //   the condition means that latitude is less than 42.
    ConditionExpression ce;
    ce.op      = ConditionOperator::lessThan; // LATITUDE < 42
    ce.operand = "LATITUDE";
    ce.val     = double(42);

    //  2) set a subject of comparison to condition
    SubjectOfComparison soc;
    soc.key       = "LATITUDE";
    soc.val       = double(37.8);

    Condition cond;
    uint64_t numberOfCondition = 1;
    cond.setCE( numberOfCondition, ce );

    //  3) evaluate the condition

    bool retCmp = cond.evaluate( soc );
    // cond.getResult();
    if ( retCmp )
        cout << "true" << endl;
    else
        cout << "false" << endl;

}

//---------------------------------------------
void TestValue()
{
    Value v1;
    uint64_t code = 10;
    v1 = code;
    cout << "v1 =" << v1.getNumber() << endl;

    Value v2;
    double real = 12.34;
    v2 = real;
    cout << "v2 =" << v2.getRealNumber() << endl;
}
