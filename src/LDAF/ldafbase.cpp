/******************************************************
Author: resat.gafarli@gmail.com
File: ldafbasic
Date: 5/24/2017
License: GPL-3.0
*******************************************************/
#include "ldafbase.h"

/*LDAFBase*/
LDAFBase::LDAFBase(QObject *parent, const QJsonObject & jsonConf):
    QObject(parent),
    m_jsonConf(jsonConf)
{
}

void LDAFBase::setReceiverObject(LDAFBase * object){
     m_object = object;
 }
void LDAFBase::addCommand(QUrl url, LDAFCallBackObject callBackObject) {
    m_commandListProcessor.addCommand(url,m_object,callBackObject);
 }

void LDAFBase::addCommand(QJsonObject jsonObject, LDAFCallBackObject callBackObject){
    m_commandListProcessor.addCommand(jsonObject,m_object,callBackObject);
}

void LDAFBase::processForwardByOne(){
     m_commandListProcessor.processForwardByOne();
}

void LDAFBase::processBackwardByOne(){
    m_commandListProcessor.processBackwardByOne();
}

void LDAFBase::processAllForward(){
    m_commandListProcessor.processAllForward();
}

void LDAFBase::processAllBackward(){
    m_commandListProcessor.processAllBackward();
}
void LDAFBase::reProcessCurrent()const{
    m_commandListProcessor.reProcessCurrent();
}

bool LDAFBase::hasNext() const{
    if (!m_commandListProcessor.isActiveQueueEmpty())
        return true;
    else
        return false;
}

bool LDAFBase::hasPrev() const{
    if (!m_commandListProcessor.isProcessedStackEmpty())
        return true;
    else
        return false;
}


QString LDAFBase::getServerResourcePath() const{
    return m_jsonConf["server_resource_root"].toString();
}

QString LDAFBase::getHomePagePath() const{
    return m_jsonConf["browser_home_page"].toString();
}


/*LDAFMessageType*/
LDAFMessageType::LDAFMessageType(LDAFBase * basicObject, LDAFCallBackObject callBackObject):
    m_basicObject(basicObject),
    m_callBackObject(callBackObject){

}

LDAFMessageType::~LDAFMessageType(){

}

const LDAFBase * const LDAFMessageType::getBasicObject() const {
    return m_basicObject;
}

const LDAFCallBackObject & LDAFMessageType::getCallBackObject() const{
    return m_callBackObject;
}


/*LDAFUrl*/
LDAFUrl::LDAFUrl(QUrl url, LDAFBase * basicObject, LDAFCallBackObject callBackObject):
    LDAFMessageType(basicObject,callBackObject),
    m_url(url){

}
LDAFUrl::~LDAFUrl(){

}
const QUrl & LDAFUrl::getUrl() const{
    return m_url;
}


void LDAFUrl::setMessage(){
    if (m_basicObject!=nullptr) m_basicObject->setURLMessage(m_url,m_callBackObject);
}

/*LDAFJson*/

LDAFJson::LDAFJson(QJsonObject jsonobject, LDAFBase * basicObject, LDAFCallBackObject callBackObject):
    LDAFMessageType(basicObject,callBackObject),
    m_jsonobject(jsonobject){

}
LDAFJson::~LDAFJson(){

}

const QJsonObject & LDAFJson::getJsonObject() const{
    return m_jsonobject;
}

void LDAFJson::setMessage(){
    if (m_basicObject!=nullptr) m_basicObject->setJsonMessage(m_jsonobject,m_callBackObject);
}

/*LDAFCommand*/
LDAFCommand::LDAFCommand(LDAFMessageType * object, SetMessageMethod setMessageMethod):
     m_object(object),
     m_setMessageMethod(setMessageMethod)
{
}

LDAFCommand::~LDAFCommand(){
     if (m_object != nullptr)
         delete m_object;
 }

void LDAFCommand::executeCommand(){
     if ((m_object!=nullptr)&&(m_setMessageMethod!=nullptr))
         (m_object->*m_setMessageMethod)();
 }

const LDAFMessageType * const LDAFCommand::getMessageObject() const{
    return m_object;
}

const LDAFCommand::SetMessageMethod LDAFCommand::getFunctionPointer() const{
    return m_setMessageMethod;
} 


/*LDAFCommandListProcessor*/
LDAFCommandListProcessor::LDAFCommandListProcessor(QObject * parent):
   QObject(parent),
   m_currentCommand(m_commandList)
{
    
}

const   QList<LDAFCommand*> & LDAFCommandListProcessor::getCommandlist() const{
    return m_commandList;
}


const QListIterator<LDAFCommand *> & LDAFCommandListProcessor::getCurrentCommand() const{
    return m_currentCommand;
}


void LDAFCommandListProcessor::addCommand(QUrl message, LDAFBase * toObject, LDAFCallBackObject callBackObject)
{
    addUrlMessage(message,toObject,callBackObject);
}

void LDAFCommandListProcessor::addCommand(QJsonObject message, LDAFBase * toObject, LDAFCallBackObject callBackObject)
{
    addJsonObjectMessage(message,toObject,callBackObject);
}

void LDAFCommandListProcessor::processForwardByOne(){
    if (m_currentCommand.hasNext()){
        m_currentCommand.next()->executeCommand();
    }
}

void LDAFCommandListProcessor::processBackwardByOne(){
    if (m_currentCommand.hasPrevious()){
        m_currentCommand.previous()->executeCommand();
    }
}

void LDAFCommandListProcessor::processAllForward(){
    while (!m_commandList.isEmpty()){
        processForwardByOne();
    }
}

void LDAFCommandListProcessor::processAllBackward(){
    while (!m_commandList.isEmpty()){
        processBackwardByOne();
    }
}

void LDAFCommandListProcessor::addUrlMessage(QUrl & message, LDAFBase * toObject, LDAFCallBackObject callBackObject){
    m_commandList.push_back(new LDAFCommand(new LDAFUrl(message,toObject,callBackObject), &LDAFMessageType::setMessage));
}

void LDAFCommandListProcessor::addJsonObjectMessage(QJsonObject & message, LDAFBase * toObject, LDAFCallBackObject callBackObject){
    m_commandList.push_back(new LDAFCommand(new LDAFJson(message,toObject,callBackObject), &LDAFMessageType::setMessage));
}

bool LDAFCommandListProcessor::isActiveQueueEmpty() const{
    return m_currentCommand.hasNext();
}

bool LDAFCommandListProcessor::isProcessedStackEmpty() const{
    return m_currentCommand.hasPrevious();
}

void LDAFCommandListProcessor::reProcessCurrent()const{
    if (m_currentCommand.hasNext()){
        m_currentCommand.peekNext()->executeCommand();
    }
}
