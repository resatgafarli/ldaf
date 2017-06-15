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

class LDAFCommandListProcessorTest: public ::testing::Test{

public:
    LDAFCommandListProcessorTest():
      receiverObject(new MockLDAFBase()),
      callBackObject(LDAFCallBackObject(new MockQObject,"TestFunction"))   
    {}
    virtual void SetUp(){
        for (int i = 1;i<=10; ++i){
            urlList.push_back(QUrl(QString("%1%2").arg("Example/Resource/Path_").arg(i)));  
            QString jscript = QString("{\"par%1\":\"val%1\",\"par%2\":\"val%2\"}").arg(i).arg(i++);
            QJsonDocument jdoc = QJsonDocument::fromJson(jscript.toUtf8());
            jsonList.push_back(jdoc.object());  
        }

    }

    
    MockLDAFBase * receiverObject;
    LDAFCallBackObject callBackObject;
    LDAFCommandListProcessor commandListProcessor;
    QList<QUrl> urlList;
    QList<QJsonObject> jsonList;
};



#endif // TST_LDAFBASE_H
