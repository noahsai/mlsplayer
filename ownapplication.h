#ifndef OWNAPPLICATION_H
#define OWNAPPLICATION_H

#include <QApplication>
#include<QLocalSocket>
#include<QTextStream>
#include<QLocalServer>
#include<QFile>
#include<player.h>
#include<QDebug>
#include<QIcon>
#include<QDesktopWidget>
class Ownapplication : public QApplication
{
    Q_OBJECT

public:
    Ownapplication(int argc, char *argv[]);
    bool istoquit();
signals:
    void toplay(QString&);
private slots:
    void newconnection();
private:
    QLocalServer *server;
    player *play;
    QString serverName;
    bool toquit;
};

#endif // OWNAPPLICATION_H
