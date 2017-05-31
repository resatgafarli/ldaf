/******************************************************
Author: resat.gafarli@gmail.com
File: tst_main.cpp
Date: 4/21/2017
License: GPL-3.0
*******************************************************/

#include <QString>
#include <QtTest>

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

//   { tst_ExampleUnit tc; QTest::qExec(&tc, argc, argv);}

   QMetaObject::invokeMethod(&a, "quit", Qt::QueuedConnection);
   return a.exec();

}



