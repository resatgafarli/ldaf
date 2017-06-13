/******************************************************
Author: resat.gafarli@gmail.com
File: ldafmain.cpp
Date: 5/19/2017
License: GPL-3.0
*******************************************************/
#include "ldafmain.h"
#include "ldafbase.h"
#include "ldafbrowser.h"
#include "ldafmediator.h"
#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

LDAFMain::LDAFMain(QObject *parent) : QObject(parent)
{

}
void LDAFMain::generateConfigrationFile(){
    qDebug()<<"Configuration file has not provided."<<endl;


    QFile saveFile("ldaf_config.json");

    if (saveFile.exists()){
        qWarning("Please provide configration file in argument list.");
        return;
    }

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open ldaf_config.json to save.");
        return;
    }

    QFile jsonConfigTemp(":/jsons/config_temp.json");
    if (!jsonConfigTemp.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open :/jsons/config_temp.json to write.");
        return;
    }
    QByteArray jsonData = jsonConfigTemp.readAll(); 
    saveFile.write(jsonData);
}

bool LDAFMain::loadConfiguration(QString fileName){
    QFile configFile(fileName);
    if(!configFile.exists()){
        qWarning("Configuration file not found.");
        return false;
    }
    qDebug()<<"Configuration file openinig..."<<endl;
    if (!configFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open configuration file.");
        return false;
    }

    qDebug()<<"Configuration file loading..."<<endl;    
    QByteArray jsonData = configFile.readAll(); 
    QJsonParseError * errorPtr = nullptr;
    QJsonDocument m_configDoc = QJsonDocument::fromJson(jsonData,errorPtr); 
    if(errorPtr!=nullptr){
        qDebug()<<errorPtr->errorString()<<endl;
    }
    m_configuration = m_configDoc.object(); 
}


int LDAFMain::instantiateLdaf(int argc, char *argv[]){
    QGuiApplication a(argc, argv);
    if (a.arguments().size()<=1){
        generateConfigrationFile();
        return -1;
     }

    QStringList args = a.arguments();
    if (!loadConfiguration(args.at(1))){
        return -1;
    }

    m_ldafmediator = new LDAFMediator(this,m_configuration);
    m_ldafbrowser = new LDAFBrowser(this,m_configuration);
    m_ldafmediator->setReceiverObject(m_ldafbrowser);
    m_ldafbrowser->setReceiverObject(m_ldafmediator);
    m_ldafbrowser->loadApplicationWindow(args.at(1));

    return a.exec();
}
