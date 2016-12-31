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

// #define CONDNODE(CondNum, CondOp, CondKey, CondValue) \
//         CP( #CondNum, COND( #CondNum, CE( #CondOp, #CondKey, #CondValue ) ) )

//---------------------------------------------
void TestBooleanCondition()
{

    //-----------------------------------------------
    // part 1. define and evaluate condition

    // define map of condition
    ConditionMap mapCondition;
    mapCondition.insert( CP( 11, COND( 11, CE( CO::lessThan, "LATITUDE", Value(double(42)) ) ) ) );
    mapCondition.insert( CP( 21, COND( 21, CE( CO::isEqual, "EXERCISE INDICATOR", VN(0) ) ) ) );
    mapCondition.insert( CP( 31, COND( 31, CE( CO::isEqual, "HELLO", VS("hello") ) ) ) );

    // set map of SOC
    SOCMap mapSOC;
    mapSOC.insert( SOCMP( 101, SOC( 101, "LATITUDE", Value(double(38.5)) ) ) );
    mapSOC.insert( SOCMP( 111, SOC( 111, "EXERCISE INDICATOR", VN(1) ) ) );
    mapSOC.insert( SOCMP( 112, SOC( 112, "HELLO", VS("hello") ) ) );

    for ( ConditionMap::iterator itCond = mapCondition.begin() ; itCond != mapCondition.end() ; itCond ++ )
    {
        uint64_t condKey = itCond->first;
        Condition cond = itCond->second;
        std::string CEOPR = cond.getConditionExpressionOperand();

        for ( SOCMap::iterator itSOC = mapSOC.begin() ; itSOC != mapSOC.end() ; itSOC ++ )
        {
            uint64_t socKey = itSOC->first;
            SOC soc = itSOC->second;

            if( cond.getConditionExpressionOperand() == soc.key )
            {
                bool ret = cond.evaluate( soc );
                soc.setResult( ret ); // set result to condKey of listSOC

                // debug string
                string resultString = ret ? "true" : "false";
                cout << "condition number: " << cond.getConditionNumber()
                     << ", \tsoc key: '" << soc.key
                     << "', \t\tresult: " <<  resultString << endl;

            }
        }
    }

    /* console result belows:
    //---------------------------------------------------------
condition number: 11, 	soc key: 'LATITUDE', 		result: true
condition number: 21, 	soc key: 'EXERCISE INDICATOR', 		result: false
condition number: 31, 	soc key: 'HELLO', 		result: true
    //---------------------------------------------------------
    */

    //-----------------------------------------------
    // part 2. define and evaluate booelan condition

    // 1. make map of BC

    // 2. define BCs

    // 3. evaluate BCs

    // 4. get final answer of all condition


}

//---------------------------------------------
void TestCondition()
{
    // usage of condition

    //  1) set condition to condition expression
    //   the condition means that latitude is less than 42.

    ConditionExpression ce( ConditionOperator::lessThan, "LATITUDE", Value(double(42)) );

    //  2) set a subject of comparison to condition

    SubjectOfComparison soc( 501, "LATITUDE", Value(double(37.8)) );

    Condition cond;
    uint64_t numberOfCondition = 1;
    cond.setCE( numberOfCondition, ce );

    //  3) evaluate the condition

    bool retCmp = cond.evaluate( soc );
    if ( retCmp )
    {
        cout << "true" << endl;
    }
    else
    {
        cout << "false" << endl;
    }

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
