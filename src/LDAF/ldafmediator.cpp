/******************************************************
Author: resat.gafarli@gmail.com
File: ldafmediator.cpp
Date: 5/19/2017
License: GPL-3.0
*******************************************************/
#include "ldafmediator.h"
#include <QDebug>
#include <QDir>

LDAFMediator::LDAFMediator(QObject *parent, QPointer<LDAFCommandListProcessor> commandListProcessor) :
    LDAFBase(parent,commandListProcessor),
    m_baseUrl(QDir::currentPath())
{
    //Convert local file based url to network based url
    m_baseUrl = m_baseUrl.fromLocalFile(m_baseUrl.path());
}

void LDAFMediator::setURLMessage(QUrl url,QString callBackJSFunc){
    QUrl resolvedUrl(m_baseUrl.resolved(url));
    addCommand(resolvedUrl,callBackJSFunc);
    processForwardByOne();
}

void LDAFMediator::setJsonMessage(QJsonObject jsonObject, QString callBackJSFunc){
    QJsonDocument doc(jsonObject);
    qDebug()<<"JSON received from browser"<<this<<doc.toJson()<<endl;

}


