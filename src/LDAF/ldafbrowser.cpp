/******************************************************
Author: resat.gafarli@gmail.com
File: ldafbrowser.cpp
Date: 5/19/2017
License: GPL-3.0
*******************************************************/
#include "ldafbrowser.h"
#include "ldafmediator.h"
#include <QDebug>
#include <QFile>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QFileInfo>

LDAFBrowser::LDAFBrowser(QObject * parent, QPointer<LDAFCommandListProcessor> commandListProcessor) :
    LDAFBase(parent,commandListProcessor),
    m_engine(new QQmlEngine),
    m_component (new QQmlComponent(m_engine)),
    m_appWindowRoot(nullptr)
{
    m_engine->rootContext()->setContextProperty("ldafbrowser",this);
    loadApplicationWindow();
}

void LDAFBrowser::loadApplicationWindow(){
    QUrl url("qrc:/ldafbrowser.qml");
    m_component->loadUrl(url);
    while (m_component->isLoading()){}
    m_appWindowRoot = m_component->create();
    for (auto e: m_component->errors()){
            qDebug()<<e.description()<<endl;
     }
}
void LDAFBrowser::loadHomePage(){
if (!m_appWindowRoot.isNull())
    QMetaObject::invokeMethod(m_appWindowRoot, "loadHomePage");
}

void LDAFBrowser::setURLMessage(QUrl url, QString callBackJSFunc){
    QVariant returnedValue;
    QFile file (url.path());
    if (file.exists()){
        if (!m_appWindowRoot.isNull())
            QMetaObject::invokeMethod(m_appWindowRoot, callBackJSFunc.toUtf8().data(),
              Q_RETURN_ARG(QVariant, returnedValue),
              Q_ARG(QVariant, url));
    }else {
        qDebug()<<"File not found:"<<url.path()<<endl;
    }

}

void LDAFBrowser::setJsonMessage(QJsonObject jsonObject, QString callBackJSFunc){
    QJsonDocument doc(jsonObject);
    qDebug()<<"JSON received from mediator"<<this<<doc.toJson()<<endl;
}

void LDAFBrowser::openPage(QString path,QString callBackJSFunc){
    QUrl url;
    url.setPath(path);
    addCommand(url,callBackJSFunc);
    processForwardByOne();
}

void LDAFBrowser::nextPage(){
    processForwardByOne();
}

void LDAFBrowser::prevPage(){
    processBackwardByOne();
}

