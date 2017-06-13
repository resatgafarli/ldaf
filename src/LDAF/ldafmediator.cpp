/******************************************************
Author: resat.gafarli@gmail.com
File: ldafmediator.cpp
Date: 5/19/2017
License: GPL-3.0
*******************************************************/
#include "ldafmediator.h"
#include <QDebug>
#include <QDir>

LDAFMediator::LDAFMediator(QObject *parent, const QJsonObject & jsonConf) :  LDAFBase(parent,jsonConf)
{
    //Convert local file based url to network based url
    m_baseUrl.setPath(getServerResourcePath());
    m_baseUrl = m_baseUrl.fromLocalFile(m_baseUrl.path());
}

void LDAFMediator::setURLMessage(QUrl url,LDAFCallBackObject callBackObject){
    QUrl resolvedUrl(m_baseUrl.resolved(url));
    addCommand(resolvedUrl,callBackObject);
    processForwardByOne();
}

void LDAFMediator::setJsonMessage(QJsonObject jsonObject, LDAFCallBackObject callBackObject){
    QJsonDocument doc(jsonObject);
    qDebug()<<"JSON received from browser"<<this<<doc.toJson()<<endl;

}


