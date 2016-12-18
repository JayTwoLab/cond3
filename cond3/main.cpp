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

void TestCode1();
void TestCode2();

int main(int argc, char *argv[])
{ // QCoreApplication a(argc, argv);

    TestCode1();
    TestCode2();



    return 0; // return a.exec();
}

//---------------------------------------------
void TestCode2()
{

    // TODO:
    //  1) set condition to conditionExpression
    ConditionExpression ce;
    ce.op      = ConditionOperator::lessThan;
    ce.operand = "LATITUDE";
    ce.val     = double(42);

    // condition.setCE( ce )

    //  2) set soc to condition
    SubjectOfComparison soc;
    soc.key       = "LATITUDE";
    soc.val       = double(37.8);

    // condition.setSOC( soc )

    //  3) compare conditionExpression

    // bool ret = condition.compare()



}

//---------------------------------------------
void TestCode1()
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
