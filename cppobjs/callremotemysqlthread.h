#ifndef CALLREMOTEMYSQLTHREAD_H
#define CALLREMOTEMYSQLTHREAD_H

#include <QObject>
#include <QThread>
#include <QSqlQuery>

class CallRemoteMySQLThread : public QThread{
    Q_OBJECT
    signals:
        void message(const QString& info);
        void progress(int present);
    public:
        CallRemoteMySQLThread(QObject* par=nullptr);
        ~CallRemoteMySQLThread();
        void run();


    private:
        QSqlQuery *query;
        int funcIdToRun=0;

};

#endif // CALLREMOTEMYSQLTHREAD_H
