#ifndef PROCESSINGMODEL_H
#define PROCESSINGMODEL_H

#include <QObject>
#include <QQuickView>

class ProcessingModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant thisModel READ thisModel NOTIFY thisModelChanged)
public:
    ProcessingModel(QObject *parent = nullptr);
    Q_INVOKABLE void itemMove(int idx);
    QVariant thisModel() const;// { return m_value1; }
    void setThisModel(const QVariant& v);

signals:
    void thisModelChanged(const QVariant&);

public slots:

private:
    QList<QObject*> myThisModel;
};

#endif // PROCESSINGMODEL_H
