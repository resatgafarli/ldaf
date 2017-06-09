/******************************************************
Author: resat.gafarli@gmail.com
File: tst_ldafbase.h
Date: 6/7/2017
License: GPL-3.0
*******************************************************/
#ifndef TST_LDAFBASE_H
#define TST_LDAFBASE_H
#include "gmock/gmock.h"
#include "ldafbase.h"

class MockLDAFBase:public LDAFBase{
public:

   MOCK_METHOD3(setURLMessage,void(QUrl, QObject * , QString));
   MOCK_METHOD3(setJsonMessage,void(QJsonObject, QObject * , QString));

};

class MockQObject:public QObject{

};

class MockLDAFMessageType: public LDAFMessageType{
public:
  explicit MockLDAFMessageType(LDAFBase*base,QObject*funcobj,QString funcName):
  LDAFMessageType(base,funcobj,funcName)
  {}
  MOCK_METHOD0(setMessage,void());  
};

#endif // TST_LDAFBASE_H
