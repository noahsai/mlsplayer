#ifndef HUANCUN_H
#define HUANCUN_H

#include <QObject>
#include<QFile>
#include<QDataStream>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<downloadmg.h>
#include<QRegularExpression>
#include<getufromc.h>
#include<QEventLoop>
#include<QProcess>
#include<QApplication>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

class huancun : public QObject
{
    Q_OBJECT
public:
    explicit huancun(QObject *parent = 0);
    ~huancun();
    void setnum(int n);
    void seturlpath(QString& url, QString& path);
    int getnum();
    void setformat(QString& fm);
    bool isdownloading();
    QString& geturl();
signals:
    void stoped();
    void finished();
    void progress(int);
    void onefinished(QString&);
    void error();
public slots:
    void start();
    void stop();
    void replyfinished();
    void setdling(bool);
    void getinfo(int );
    void geturls(int);
    void gotcstodownload(QString&);
private:
    QString encode(const QString& url);
    void work(const QString &edurl);
    bool fenxi1(QString& html);
    bool fenxi2(QString& html);
    void getsite(QString& html);
    void todownload(QString& cmd);
    void fenxi3(QString& html);
    void save();
    void fenxi();
    bool readcfg();
    bool you_get(QString );
    void you_geturls(QString&,QString&);

    bool downloading;
    int zhuanji;
    int num;
    int all;
    QString name;
    int goon;
    QString urls;
    QString playurl;
    QString dirpath;
    downloadmg *dlmg;
    QString format;
    QNetworkAccessManager *manager;
    QNetworkRequest reque;
    QNetworkReply *reply;
    QRegularExpression reg;
    QRegularExpressionMatch match;
    QRegularExpressionMatchIterator matchs;
    int getfiletype;
    bool isstop;
    QProcess *pro;
    QNetworkCookieJar *jar;
    QString playrf;
    QString U_A;
};

#endif // HUANCUN_H
