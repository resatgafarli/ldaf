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

class MockQObject:public QObject{
};


class MockLDAFBase:public LDAFBase{
public:
   
   MOCK_METHOD2(setURLMessage,void(QUrl, LDAFCallBackObject));
   MOCK_METHOD2(setJsonMessage,void(QJsonObject, LDAFCallBackObject));
};

class MockLDAFMessageType: public LDAFMessageType{
public:
  explicit MockLDAFMessageType(LDAFBase* base,LDAFCallBackObject callBackObject):
  LDAFMessageType(base,callBackObject)
  {}
  MOCK_METHOD0(setMessage,void());  
};

class MockLDAFUrl: public LDAFUrl{
public:
  explicit MockLDAFUrl(QUrl url, LDAFBase*base,LDAFCallBackObject callBackObject):
  LDAFUrl(url,base,callBackObject)
  {}
  MOCK_METHOD0(setMessage,void());  
};

class MockLDAFJson: public LDAFJson{
public:
  explicit MockLDAFJson(QJsonObject jsonObject, LDAFBase*base,LDAFCallBackObject callBackObject):
  LDAFJson(jsonObject,base,callBackObject)
  {}
  MOCK_METHOD0(setMessage,void());  
};

class MockLDAFCommand: public LDAFCommand{
public:
  explicit MockLDAFCommand(LDAFMessageType * object=nullptr, SetMessageMethod setMessageMethod=nullptr):
    LDAFCommand(object,setMessageMethod){}
};



#endif // TST_LDAFBASE_H
