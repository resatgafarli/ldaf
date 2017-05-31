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


LDAFMain::LDAFMain(QObject *parent) : QObject(parent)
{

}
int LDAFMain::instantiateLdaf(int argc, char *argv[]){
    QGuiApplication a(argc, argv);
    m_browserCLP = new LDAFCommandListProcessor;
    m_mediatorCLP = new LDAFCommandListProcessor;
    m_ldafmediator = new LDAFMediator(this,m_mediatorCLP);
    m_ldafbrowser = new LDAFBrowser(this,m_browserCLP);

    m_ldafmediator->setReceiverObject(m_ldafbrowser);
    m_ldafbrowser->setReceiverObject(m_ldafmediator);
    m_ldafbrowser->loadHomePage();

    return a.exec();
}
