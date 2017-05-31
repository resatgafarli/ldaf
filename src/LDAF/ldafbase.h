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

class LDAFCommandListProcessor;

class LDAFBase:public QObject{
public:

   explicit LDAFBase(QObject *parent=0,QPointer<LDAFCommandListProcessor>commandListProcessor=nullptr);
   void setReceiverObject(LDAFBase * object);

   virtual void setURLMessage(QUrl,QString) = 0;
   virtual void setJsonMessage(QJsonObject,QString) = 0;

   virtual void addCommand(QUrl url, QString callBackJSFunc);
   virtual void addCommand(QJsonObject jsonObject, QString callBackJSFunc);
   virtual void processForwardByOne();
   virtual void processBackwardByOne();
   virtual void processAllForward();
   virtual void processAllBackward();

private:
    QPointer<LDAFCommandListProcessor> m_commandListProcessor;
    QPointer<LDAFBase> m_object;
};


class LDAFMessageType{
public:
    explicit LDAFMessageType(LDAFBase * basicObject=nullptr,QString callBackJSFunc="");
    virtual ~LDAFMessageType();
    virtual void setMessage()=0;

protected:
    LDAFBase * m_basicObject;
    QString m_callBackJSFunc;
};

class LDAFUrl:public LDAFMessageType{
public:
    explicit LDAFUrl(QUrl url, LDAFBase * basicObject,QString callBackJSFunc);
    virtual ~LDAFUrl();
    void setMessage();

private:
    QUrl m_url;
};

class LDAFJson:public LDAFMessageType{
public:
    explicit LDAFJson(QJsonObject jsonobject, LDAFBase * basicObject, QString callBackJSFunc);
    virtual ~LDAFJson();
    void setMessage();

private:
    QJsonObject m_jsonobject;
};


class LDAFCommand{
    typedef void(LDAFMessageType:: *Method)();
public:
    explicit LDAFCommand(LDAFMessageType * object=nullptr, Method method=nullptr);
    virtual ~LDAFCommand();
    void executeCommand();
private:
    LDAFMessageType * m_object;
    Method m_method;
};


class LDAFCommandListProcessor:public QObject {
public:
    void addCommand(QUrl message, LDAFBase * toObject, QString callBackJSFunc);
    void addCommand(QJsonObject message, LDAFBase * toObject, QString callBackJSFunc);
    void processForwardByOne();
    void processBackwardByOne();
    void processAllForward();
    void processAllBackward();

private:
    void addUrlMessage(QUrl & message, LDAFBase * toObject, QString callBackJSFunc);
    void addJsonObjectMessage(QJsonObject & message, LDAFBase * toObject, QString callBackJSFunc);

    QQueue<LDAFCommand*> m_activeQueue;
    QStack<LDAFCommand*> m_processedStack;

};

#endif // LDAFBASE_H

