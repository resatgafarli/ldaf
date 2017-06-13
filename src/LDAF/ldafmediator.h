/******************************************************
Author: resat.gafarli@gmail.com
File: ldafmediator.h
Date: 5/19/2017
License: GPL-3.0
*******************************************************/
#ifndef LDAFMEDIATOR_H
#define LDAFMEDIATOR_H

#include "ldafbase.h"

class LDAFMediator : public LDAFBase
{
    Q_OBJECT
public:
    explicit LDAFMediator(QObject *parent, const QJsonObject & jsonConf=QJsonObject());

private:
    QUrl m_baseUrl;
    void setURLMessage(QUrl, QObject *callBackObject, QString callBackJSFunc);
    void setJsonMessage(QJsonObject, QObject *callBackObject, QString callBackJSFunc);
};

#endif // LDAFMEDIATOR_H
