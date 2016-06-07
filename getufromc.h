#ifndef GETUFROMC_H
#define GETUFROMC_H
#include<QTextStream>
#include<QIODevice>
#include<QDebug>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QListWidget>
#include<QTextCodec>
#include<getone.h>

class getufromc: public QObject
{
    Q_OBJECT
public:
    getufromc(const QStringList &list,QObject* parent = 0);
    ~getufromc();
    QString getresult();
    void start();
signals:
    void finished(QString&);

public slots:
    void onegeted();

private:
    int all,fin,task;
    QStringList clist;
    QString result;
    getone *download;

};

#endif // GETUFROMC_H
