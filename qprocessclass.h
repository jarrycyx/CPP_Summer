#ifndef QPROCESSCLASS_H
#define QPROCESSCLASS_H

#include <QObject>

class QProcessClass : public QObject
{
    Q_OBJECT
public:
    explicit QProcessClass(QObject *parent = nullptr);
    Q_INVOKABLE int qmlToCppTest(int v);

signals:
    void signal1(QString qstr);

public slots:
    void slotTest1();
    void slotTest2(QString str);
};

#endif // QPROCESSCLASS_H
