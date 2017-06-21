/******************************************************
Author: resat.gafarli@gmail.com
File: ldafbase.cpp
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

void LDAFBase::callBackObjectFunction(LDAFCallBackObject callBackObject, QVariant arg){

    if (callBackObject.getObjectPointer() == nullptr){
        qDebug()<<"Callback object is invalid"<<endl;
        return;
    }

    QVariant returnedValue;
    QMetaObject::invokeMethod(callBackObject.getObjectPointer(), callBackObject.getFunctionName().toUtf8().data(),
              Q_RETURN_ARG(QVariant, returnedValue),
              Q_ARG(QVariant, arg));
}


void LDAFBase::setReceiverObject(LDAFBase * object){
     m_object = object;
 }

const QPointer<LDAFBase> & LDAFBase::getReceiverObject() const{
    return m_object;
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
void LDAFBase::reProcessCurrent() {
    m_commandListProcessor.reProcessCurrent();
}

bool LDAFBase::hasNext(){
    m_commandListProcessor.hasNext();
}

bool LDAFBase::hasPrev(){
    m_commandListProcessor.hasPrev();
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
   m_currentCommandId(-1),
   m_currentCommand(nullptr)
{
}

const   QList<LDAFCommand*> & LDAFCommandListProcessor::getCommandlist() const{
    return m_commandList;
}


const LDAFCommand * const LDAFCommandListProcessor::getCurrentCommand() const{
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

void LDAFCommandListProcessor::setCurrentCommand(){
    if ((m_currentCommandId>=0) && (m_currentCommandId<m_commandList.size())){
        m_currentCommand = m_commandList.at(m_currentCommandId);
    }else
        m_currentCommand=nullptr;
}

void LDAFCommandListProcessor::next(){
    if (hasNext()) m_currentCommandId++;
    setCurrentCommand();        
}

void LDAFCommandListProcessor::prev(){
    if (hasPrev()) m_currentCommandId--;
    setCurrentCommand();
}

void LDAFCommandListProcessor::processForwardByOne(){
    if (hasNext()){
        next();
        if (m_currentCommand!=nullptr) 
            m_currentCommand->executeCommand();
    }
}

void LDAFCommandListProcessor::processBackwardByOne(){
    if (hasPrev()){
        prev();
        if (m_currentCommand!=nullptr) 
            m_currentCommand->executeCommand();
    }
}

void LDAFCommandListProcessor::processAllForward(){
    while (hasNext()){
        processForwardByOne();
    }
}

void LDAFCommandListProcessor::processAllBackward(){
    while (hasPrev()){
        processBackwardByOne();
    }
}

void LDAFCommandListProcessor::addUrlMessage(QUrl & message, LDAFBase * toObject, LDAFCallBackObject callBackObject){
    m_commandList.push_back(new LDAFCommand(new LDAFUrl(message,toObject,callBackObject), &LDAFMessageType::setMessage));
}

void LDAFCommandListProcessor::addJsonObjectMessage(QJsonObject & message, LDAFBase * toObject, LDAFCallBackObject callBackObject){
    m_commandList.push_back(new LDAFCommand(new LDAFJson(message,toObject,callBackObject), &LDAFMessageType::setMessage));
}

bool LDAFCommandListProcessor::hasNext() {
    //Command pointer is adjusted in case of list size been changed.  
    if (m_currentCommandId+1 >= m_commandList.size())
        m_currentCommandId = m_commandList.size()-1;

    if (m_currentCommandId+1 < m_commandList.size())
        return true;
    else 
        return false;
}

bool LDAFCommandListProcessor::hasPrev() {
    //Command pointer is adjusted in case of list size been changed.  
    if (m_currentCommandId-1 >= m_commandList.size())
        m_currentCommandId = m_commandList.size()-1;
    
    if (m_currentCommandId-1 >= 0)
        return true;
    else 
        return false;
}

void LDAFCommandListProcessor::reProcessCurrent(){
    setCurrentCommand();
    if (m_currentCommand != nullptr)
        m_currentCommand->executeCommand();
}

