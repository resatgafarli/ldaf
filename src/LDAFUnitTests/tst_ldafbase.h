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
    MockLDAFBase mockLDAFBase;
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
};

class LDAFBaseTest: public ::testing::Test, public LDAFCommonTestMessages {
public:
    virtual void SetUp(){
      setupMessageLists();
      setupConfiguration();
      m_firstLDAFBase = new MockLDAFBase(nullptr,m_fakeConfiguration);
      m_secondLDAFBase = new MockLDAFBase(nullptr,m_fakeConfiguration);
      m_firstLDAFBase->setReceiverObject(m_secondLDAFBase);
      m_secondLDAFBase->setReceiverObject(m_firstLDAFBase);
    }

    virtual void setupConfiguration(){
      QString jscript = QString("{\"server_resource_root\":\"ServerRootPath\",\"browser_home_page\":\"HomePage\"}");
      QJsonDocument jdoc = QJsonDocument::fromJson(jscript.toUtf8());
      m_fakeConfiguration = jdoc.object(); 
    }

  QJsonObject m_fakeConfiguration;
  QPointer<MockLDAFBase> m_firstLDAFBase;
  QPointer<MockLDAFBase> m_secondLDAFBase;

};

#endif // TST_LDAFBASE_H
