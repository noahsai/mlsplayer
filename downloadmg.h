#ifndef DOWNLOADMG_H
#define DOWNLOADMG_H

#include <QObject>
#include<QStringList>
#include<QFile>
#include<QDataStream>
#include<downloadone.h>
#include<QDir>
class downloadmg : public QObject
{
    Q_OBJECT
public:
    explicit downloadmg(QObject *parent = 0);
    void seturlpath(QString& name,QString& urllist,QString& path,QString& refurl,QString& ua);
    void start(int);
    ~downloadmg();
signals:
    void onefinished(QString&);
    void progress(int);
    void finished();
    void stoped();
    void error();
public slots:
    void next();
    void stop();
    void save();
private:
QString U_A;
    QString filename;
    QStringList urls;
    QString ref;
    QString dirpath;
    int now;
    int all;
    downloadone *one;
    QStringList files;
};

#endif // DOWNLOADMG_H
