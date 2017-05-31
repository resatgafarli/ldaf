/******************************************************
Author: resat.gafarli@gmail.com
File: ldafbasic
Date: 5/24/2017
License: GPL-3.0
*******************************************************/
#include "ldafbase.h"

/**/
LDAFBase::LDAFBase(QObject *parent,QPointer<LDAFCommandListProcessor>commandListProcessor):
    QObject(parent),
    m_commandListProcessor(commandListProcessor){
}

void LDAFBase::setReceiverObject(LDAFBase * object){
     m_object = object;
 }
void LDAFBase::addCommand(QUrl url, QString callBackJSFunc) {
    m_commandListProcessor->addCommand(url,m_object,callBackJSFunc);
 }

void LDAFBase::addCommand(QJsonObject jsonObject, QString callBackJSFunc){
    m_commandListProcessor->addCommand(jsonObject,m_object,callBackJSFunc);
}

void LDAFBase::processForwardByOne(){
     m_commandListProcessor->processForwardByOne();
}

void LDAFBase::processBackwardByOne(){
    m_commandListProcessor->processBackwardByOne();
}

void LDAFBase::processAllForward(){
    m_commandListProcessor->processAllForward();
}

void LDAFBase::processAllBackward(){
    m_commandListProcessor->processAllBackward();
}


/**/
LDAFMessageType::LDAFMessageType(LDAFBase * basicObject,QString callBackJSFunc):
    m_basicObject(basicObject),
    m_callBackJSFunc(callBackJSFunc){

}
LDAFMessageType::~LDAFMessageType(){

}

/**/
LDAFUrl::LDAFUrl(QUrl url, LDAFBase * basicObject,QString callBackJSFunc):
    LDAFMessageType(basicObject,callBackJSFunc),
    m_url(url){

}
LDAFUrl::~LDAFUrl(){

}

void LDAFUrl::setMessage(){
    if (m_basicObject!=nullptr) m_basicObject->setURLMessage(m_url,m_callBackJSFunc);
}

/**/

LDAFJson::LDAFJson(QJsonObject jsonobject, LDAFBase * basicObject,QString callBackJSFunc):
    LDAFMessageType(basicObject,callBackJSFunc),
    m_jsonobject(jsonobject){

}
LDAFJson::~LDAFJson(){

}

void LDAFJson::setMessage(){
    if (m_basicObject!=nullptr) m_basicObject->setJsonMessage(m_jsonobject,m_callBackJSFunc);
}

/**/

LDAFCommand::LDAFCommand(LDAFMessageType * object, Method method):
     m_object(object),
     m_method(method){

}

LDAFCommand::~LDAFCommand(){
     if (m_object != nullptr)
         delete m_object;
 }

void LDAFCommand::executeCommand(){
     if ((m_object!=nullptr)&&(m_method!=nullptr))
         (m_object->*m_method)();
 }

/**/
void LDAFCommandListProcessor::addCommand(QUrl message, LDAFBase * toObject, QString callBackJSFunc)
{
    addUrlMessage(message,toObject,callBackJSFunc);
}

void LDAFCommandListProcessor::addCommand(QJsonObject message, LDAFBase * toObject, QString callBackJSFunc)
{
    addJsonObjectMessage(message,toObject,callBackJSFunc);
}

void LDAFCommandListProcessor::processForwardByOne(){
    if (!m_activeQueue.isEmpty()){
        LDAFCommand * command  = m_activeQueue.dequeue();
        command->executeCommand();
        m_processedStack.push(command);
    }
}

void LDAFCommandListProcessor::processBackwardByOne(){
    if (!m_processedStack.isEmpty()){
        LDAFCommand * command  = m_processedStack.top();
        command->executeCommand();
        m_processedStack.pop();
        m_activeQueue.enqueue(command);
    }
}

void LDAFCommandListProcessor::processAllForward(){
    while (!m_activeQueue.isEmpty()){
        processForwardByOne();
    }
}

void LDAFCommandListProcessor::processAllBackward(){
    while (!m_processedStack.isEmpty()){
        processBackwardByOne();
    }
}

void LDAFCommandListProcessor::addUrlMessage(QUrl & message, LDAFBase * toObject, QString callBackJSFunc){
    m_activeQueue.enqueue(new LDAFCommand(new LDAFUrl(message,toObject,callBackJSFunc), &LDAFMessageType::setMessage));
}

void LDAFCommandListProcessor::addJsonObjectMessage(QJsonObject & message, LDAFBase * toObject, QString callBackJSFunc){
    m_activeQueue.enqueue(new LDAFCommand(new LDAFJson(message,toObject,callBackJSFunc), &LDAFMessageType::setMessage));
}


