#ifndef GETTIME_H
#define GETTIME_H

#include <QObject>
#include<QDebug>
#include<QEventLoop>
#include<QTimer>
#include<QtAV>
#include <QtAVWidgets>
using namespace QtAV ;

class gettime : public QObject
{
    Q_OBJECT
public:
    explicit gettime(QStringList&,QString&,QString&, QObject *parent = 0);
    int getnow();
signals:
    void gottime(qint64 ,int);

public slots:
    void got(qint64 );
    void toget();
    void error(QtAV::AVError);

private:
    QStringList list;
    AVPlayer *play;
    qint64 time;//记录总共huo取了多少时间
    int n,errtry;
    QString rf;
    QString U_A;

};

#endif // GETTIME_H
