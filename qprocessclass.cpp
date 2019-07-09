#include "qprocessclass.h"
#include <QDebug>
#include <QString>

QProcessClass::QProcessClass(QObject *parent) : QObject(parent)
{

}

void QProcessClass::slotTest1(){
    qDebug()<<"QProcessClass receive signal, slot 1 activated.";
    emit signal1(QString::fromUtf8("QProcessClass signal 1 sent"));
}

void QProcessClass::slotTest2(QString str){
    qDebug()<<"QProcessClass receive signal, slot 2 activated."<<str;
}

Q_INVOKABLE int QProcessClass::qmlToCppTest(int v){
    emit signal1(QString::fromUtf8("QProcessClass signal 1 sent"));
    return v+1;
}
