#ifndef DOWNLOADONE_H
#define DOWNLOADONE_H

#include <QObject>
#include<QFile>
#include<QDataStream>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QEventLoop>
class downloadone : public QObject
{
    Q_OBJECT
public:
    explicit downloadone(QObject *parent = 0);
    void setnameurl(QString& name ,QString& url,QString& refurl,QString& ua);
    void start();
    void stop();
    ~downloadone();
signals:
    void finished();
    void stoped();
    void progress(int);
    void error();
public slots:
    void next();
    void savefile();
    void datagot();

private:
    QString U_A;
    qint64 got;
    qint64 all;

    QString filename;
    QString fileurl;
    QFile file;
    QString ref;

    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
    bool tostop;
    qint64 havedone;
    int ps;
    int trytimes;
};

#endif // DOWNLOADONE_H
