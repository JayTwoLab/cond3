//
// main.cpp


// #include <QCoreApplication>

#include <list>
#include <vector>
#include <map>
#include <string>
#include <iostream>
using namespace std;

#include "SubjectOfComparison.h"

int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);


    SubjectOfComparison soc;
    soc.key = "LATITUDE";
    soc.valueType = SubjectOfComparison::RealNumber;
    soc.realValue = "37.8";



    return 0;
    // return a.exec();
}

