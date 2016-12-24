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

    //-----------------------------------------------
    // part 1. define and evaluate condition

    // define list of condition
    ConditionMap mapCondition;
    mapCondition.insert( CP( 11, COND( 11, CE( CO::lessThan, "LATITUDE", Value(double(42))  ) ) ) );
    mapCondition.insert( CP( 21, COND( 21, CE( CO::isEqual, "EXERCISE INDICATOR", VN(0) ) ) ) );
    mapCondition.insert( CP( 31, COND( 31, CE( CO::isEqual, "HELLO", VS("hello") ) ) ) );

    // set list of SOC
    SOCList listSOC;
    listSOC.insert( listSOC.end(), SOC( "LATITUDE", Value(double(38.5)) ) );
    listSOC.insert( listSOC.end(), SOC( "EXERCISE INDICATOR", VN(1) ) );
    listSOC.insert( listSOC.end(), SOC( "HELLO", VS("hello") ) );

    for ( ConditionMap::iterator itCond = mapCondition.begin() ; itCond != mapCondition.end() ; itCond ++ )
    {
        // uint64_t condKey = itCond->first;
        Condition cond = itCond->second;
        for ( SOCList::iterator itSOC = listSOC.begin() ; itSOC != listSOC.end() ; itSOC ++ )
        {
            SOC soc = *itSOC;
            if( cond.getConditionExpressionOperand() == soc.key )
            {
                bool ret = cond.evaluate( soc );
                std::string resultString = ret ? "true" : "false";
                cout << "condition number: " << cond.getConditionNumber()
                     << ", soc key: " << soc.key
                     << ", result: " << resultString << endl;
            }
        }
    }

    /* console result belows:
    //---------------------------------------------------------
    condition number: 11, soc key: LATITUDE, result: true
    condition number: 21, soc key: EXERCISE INDICATOR, result: false
    condition number: 31, soc key: HELLO, result: true
    //---------------------------------------------------------
    */

    //-----------------------------------------------
    // part 2. define and evaluate booelan condition

    // 0) define list of condition (LOC)
    // std::map<uint64_t, bool> mapBC;

    // mapBC.insert( std::pair<uint64_t, bool>( 1, false ) );
    // mapBC.insert( std::pair<uint64_t, bool>( 2, true  ) );

    // 1) define first boolean condition
    // BooleanCondition bc;
    // bool isFinalCondition = false;
    // bc.setCondition( 1, BooleanOperator::And, isFinalCondition );
    // bc.setFirst ( Condition#1  )
    // bc.setSecond( false )

    // bool first  = true;
    // bool second = false;
    // bool result;
    // if ( bc.evaluate( first, second, result ) )
    // {
    // }

    // 2) define list of BooleanCondition (BC)


}

//---------------------------------------------
void TestCondition()
{
    // usage of condition

    //  1) set condition to condition expression
    //   the condition means that latitude is less than 42.
    // ConditionExpression ce;
    // ce.op      = ConditionOperator::lessThan; // LATITUDE < 42
    // ce.operand = "LATITUDE";
    // ce.val     = double(42);
    ConditionExpression ce( ConditionOperator::lessThan, "LATITUDE", Value(double(42)) );

    //  2) set a subject of comparison to condition
    // SubjectOfComparison soc;
    // soc.key       = "LATITUDE";
    // soc.val       = double(37.8);
    SubjectOfComparison soc( "LATITUDE", Value(double(37.8)) );

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
