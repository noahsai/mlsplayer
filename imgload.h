#ifndef IMGLOAD_H
#define IMGLOAD_H

#include <QObject>
#include<QDebug>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QFile>
#include<QApplication>
#include<QDir>
#include<QMutex>

class imgload : public QObject
{
    Q_OBJECT
public:
    explicit imgload(int,QString& imgurl,QObject *parent = 0);

signals:
    void loaded(int);
public slots:
    bool isgoted();
    void finished();
    void start();
    QString& getname();
    void stop();

private:
    QMutex *mutex;
    bool got;
    QUrl url;
    int index;
    QString imgname;
    QNetworkAccessManager *manager;
    QNetworkRequest reque;
    QNetworkReply *reply;
    QFile file;
    bool tostop;


};

#endif // IMGLOAD_H
