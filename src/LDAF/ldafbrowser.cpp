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

LDAFBrowser::LDAFBrowser(QObject * parent, const QJsonObject & jsonConf) : LDAFBase(parent,jsonConf),
    m_engine(new QQmlEngine),
    m_component (new QQmlComponent(m_engine)),
    m_appWindowRoot(nullptr)
{
    m_engine->rootContext()->setContextProperty("ldafbrowser",this);
}

void LDAFBrowser::loadApplicationWindow(QString path){
    QUrl url("qrc:/ldafbrowser.qml");
    m_component->loadUrl(url);
    while (m_component->isLoading()){}
    m_appWindowRoot = m_component->create();
    for (auto e: m_component->errors()){
            qDebug()<<e.description()<<endl;
     }
}

void LDAFBrowser::setURLMessage(QUrl url, QObject * callBackObject, QString callBackJSFunc){

    if (callBackObject == nullptr){
        qDebug()<<"Callback object is invalid"<<endl;
        return;
    }

    QVariant returnedValue;
    QFile file (url.path());
    if (file.exists()){
            QMetaObject::invokeMethod(callBackObject, callBackJSFunc.toUtf8().data(),
              Q_RETURN_ARG(QVariant, returnedValue),
              Q_ARG(QVariant, url));
    }else {
        qDebug()<<"File not found:"<<url.path()<<endl;
    }

}

void LDAFBrowser::setJsonMessage(QJsonObject jsonObject, QObject * callBackObject, QString callBackJSFunc){
    QJsonDocument doc(jsonObject);
    qDebug()<<"JSON received from mediator"<<this<<doc.toJson()<<endl;
}

void LDAFBrowser::openPage(QString path, QObject * callBackObject, QString callBackJSFunc){
    QUrl url;
    url.setPath(path);
    addCommand(url,callBackObject,callBackJSFunc);
    processForwardByOne();
}

void LDAFBrowser::reloadCurrent(){
    reProcessCurrent();
}

void LDAFBrowser::nextPage(){
    processForwardByOne();
}

void LDAFBrowser::prevPage(){
    processBackwardByOne();
}

