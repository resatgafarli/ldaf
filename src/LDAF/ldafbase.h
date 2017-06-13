/******************************************************
Author: resat.gafarli@gmail.com
File: ldafbasic
Date: 5/23/2017
License: GPL-3.0
*******************************************************/
#ifndef LDAFBASE_H
#define LDAFBASE_H
#include <QObject>
#include <QDebug>
#include <QQueue>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPointer>
#include <QStack>
#include <QJsonValue>

class LDAFBase; 
class LDAFCommand; 

class LDAFCommandListProcessor:public QObject {
public:
    LDAFCommandListProcessor(QObject * parent=nullptr);
    void addCommand(QUrl message, LDAFBase * toObject,QObject *callBackObject, QString callBackJSFunc);
    void addCommand(QJsonObject message, LDAFBase * toObject, QObject *callBackObject, QString callBackJSFunc);
    void processForwardByOne();
    void processBackwardByOne();
    void processAllForward();
    void processAllBackward();
    void reProcessCurrent() const;
    bool isActiveQueueEmpty() const;
    bool isProcessedStackEmpty() const;

private:
    void addUrlMessage(QUrl & message, LDAFBase * toObject, QObject *callBackObject, QString callBackJSFunc);
    void addJsonObjectMessage(QJsonObject & message, LDAFBase * toObject, QObject *callBackObject, QString callBackJSFunc);
    LDAFCommand* m_currentCommand;
    QQueue<LDAFCommand*> m_activeQueue;
    QStack<LDAFCommand*> m_processedStack;

};

class LDAFBase:public QObject{
    Q_OBJECT
public:

   explicit LDAFBase(QObject *parent=0, const QJsonObject & jsonConf=QJsonObject());
   void setReceiverObject(LDAFBase * object);

   virtual void setURLMessage(QUrl,QObject * callBackObject,QString) = 0;
   virtual void setJsonMessage(QJsonObject,QObject * callBackObject, QString) = 0;

   virtual void addCommand(QUrl url, QObject * callBackObject, QString callBackJSFunc);
   virtual void addCommand(QJsonObject jsonObject, QObject *callBackObject, QString callBackJSFunc);
   virtual void processForwardByOne();
   virtual void processBackwardByOne();
   virtual void processAllForward();
   virtual void processAllBackward();
   virtual void reProcessCurrent()const;
   Q_INVOKABLE bool hasNext() const;
   Q_INVOKABLE bool hasPrev() const;
   Q_INVOKABLE QString getHomePagePath() const;
   Q_INVOKABLE QString getServerResourcePath() const; 
   

private:
    const QJsonObject & m_jsonConf;
    LDAFCommandListProcessor m_commandListProcessor;
    QPointer<LDAFBase> m_object;
};


class LDAFMessageType{
public:
    explicit LDAFMessageType(LDAFBase * basicObject=nullptr, QObject * callBackObject=nullptr, QString callBackJSFunc="");
    virtual ~LDAFMessageType();
    virtual void setMessage()=0;
    const LDAFBase * const getBasicObject() const;
    const QObject  * const getCallBackObject() const;
    const QString & getCallBackJSFunc() const;
protected:
    LDAFBase * m_basicObject;
    QObject * m_callBackObject;
    QString m_callBackJSFunc;
};

class LDAFUrl:public LDAFMessageType{
public:
    explicit LDAFUrl(QUrl url, LDAFBase * basicObject,QObject* callBackObject, QString callBackJSFunc);
    virtual ~LDAFUrl();
    void setMessage();
    const QUrl & getUrl() const;
private:
    QUrl m_url;
};

class LDAFJson:public LDAFMessageType{
public:
    explicit LDAFJson(QJsonObject jsonobject, LDAFBase * basicObject,QObject * callBackObject, QString callBackJSFunc);
    virtual ~LDAFJson();
    void setMessage();
    const QJsonObject & getJsonObject() const;
private:
    QJsonObject m_jsonobject;
};


class LDAFCommand{
public:
    typedef void(LDAFMessageType:: *SetMessageMethod)();
    explicit LDAFCommand(LDAFMessageType * object=nullptr, SetMessageMethod setMessageMethod=nullptr);
    virtual ~LDAFCommand();
    void executeCommand();
    const LDAFMessageType * const getMessageObject() const;
    const LDAFCommand::SetMessageMethod getFunctionPointer() const; 
private:
    LDAFMessageType * m_object;
    SetMessageMethod m_setMessageMethod;
};




#endif // LDAFBASE_H

