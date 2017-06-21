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
   MockLDAFBase(QObject *parent=0, const QJsonObject & jsonConf=QJsonObject())
    :LDAFBase(parent,jsonConf){
    }
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

class LDAFCommonTestMessages{
  public:
    void setupMessageLists(){
        for (int i = 1;i<=11; ++i){
            urlList.push_back(QUrl(QString("%1%2").arg("Example/Resource/Path_").arg(i)));  
            QString jscript = QString("{\"par%1\":\"val%1\",\"par%2\":\"val%2\"}").arg(i).arg(i+1);
            QJsonDocument jdoc = QJsonDocument::fromJson(jscript.toUtf8());
            jsonList.push_back(jdoc.object());  
        }
    }
    
    QList<QUrl> urlList;
    QList<QJsonObject> jsonList;
    LDAFCallBackObject callBackObject;  
};

class LDAFCommandListProcessorTest: public ::testing::Test, public LDAFCommonTestMessages {
public:
    virtual void SetUp(){
        setupMessageLists();
    }
    void fillUrlQueue(){
        for (auto  url : urlList){
          commandListProcessor.addCommand(url,&mockLDAFBase,callBackObject);
        }
    }

    void fillJsonQueue(){
        for (auto  json : jsonList){
          commandListProcessor.addCommand(json,&mockLDAFBase,callBackObject);
        }

    }

    LDAFCommandListProcessor commandListProcessor;
    MockLDAFBase mockLDAFBase;
};

class LDAFBaseTest: public ::testing::Test, public LDAFCommonTestMessages {
public:
    virtual void SetUp(){
      setupMessageLists();
      setupConfiguration();
      firstLDAFBase = new MockLDAFBase(nullptr,fakeConfiguration);
      secondLDAFBase = new MockLDAFBase(nullptr,fakeConfiguration);
    }
    virtual void TearDown(){
      delete firstLDAFBase;
      delete secondLDAFBase;
    }

    virtual void connectLDAFBases(){
      firstLDAFBase->setReceiverObject(secondLDAFBase);
      secondLDAFBase->setReceiverObject(firstLDAFBase);
    }

    virtual void addUrlCommands(){
      for (auto url: urlList){
        firstLDAFBase->addCommand(url,callBackObject);
      }
    }
    
    virtual void addJsonCommands(){
      for (auto json: jsonList){
        firstLDAFBase->addCommand(json,callBackObject);
      }
    }

    virtual void setResponseUrlMessage(QUrl url, LDAFCallBackObject callBackObject){
        secondLDAFBase->addCommand(url,callBackObject);
        secondLDAFBase->processForwardByOne();
    }

    virtual void setResponseJsonMessage(QJsonObject json, LDAFCallBackObject callBackObject){
        secondLDAFBase->addCommand(json,callBackObject);
        secondLDAFBase->processForwardByOne();
    }

    virtual void setupConfiguration(){
      QString jscript = QString("{\"server_resource_root\":\"ServerResourcePath\",\"browser_home_page\":\"HomePage\"}");
      QJsonDocument jdoc = QJsonDocument::fromJson(jscript.toUtf8());
      fakeConfiguration = jdoc.object(); 
    }

  QJsonObject fakeConfiguration;
  MockLDAFBase * firstLDAFBase;
  MockLDAFBase * secondLDAFBase;

};

#endif // TST_LDAFBASE_H
