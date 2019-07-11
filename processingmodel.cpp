#include "processingmodel.h"
#include <dataobject.h>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>

ProcessingModel::ProcessingModel(QObject *parent) : QObject(parent)
{

    myThisModel.append(new DataObject("Item 1", "red"));
    myThisModel.append(new DataObject("Item 2", "green"));
    myThisModel.append(new DataObject("Item 3", "blue"));
    myThisModel.append(new DataObject("Item 4", "yellow"));
    myThisModel.append(new DataObject("Item 5", "yellow"));
    myThisModel.append(new DataObject("Item 6", "yellow"));
    myThisModel.append(new DataObject("Item 7", "yellow"));
    myThisModel.append(new DataObject("Item 8", "yellow"));
    myThisModel.append(new DataObject("Item 9", "yellow"));
    //myThisModel.removeAt(1);
}

QVariant ProcessingModel::thisModel() const{
    return QVariant::fromValue(myThisModel);
}


Q_INVOKABLE void ProcessingModel::itemMove(int idx){
    myThisModel.removeAt(idx);
    qDebug()<<myThisModel.length();
}
