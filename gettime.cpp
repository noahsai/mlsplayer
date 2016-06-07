#include "gettime.h"


gettime::gettime(QStringList& l, QString&ref, QString&ua,QObject *parent) :
    QObject(parent)
{
     U_A = "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)";
    if(!ua.isEmpty()) U_A = ua;
    list = l;
    time=0;
    n=0;
    errtry=0;
    rf=ref;
    play = new AVPlayer;
    connect(play,SIGNAL(durationChanged(qint64)),this,SLOT(got(qint64)));
    connect(play,SIGNAL(error(QtAV::AVError)),this,SLOT(error(QtAV::AVError)));
}
void gettime::toget()//不能将toget放在构造函数里，因为外面还要connection，怕toget执行完，外面没connection上
{
    qDebug()<<"总共："<<list.length()<<"现在："<<n;
    if(n<list.length())
    {
        QVariantHash opt;
        opt["referer"]= rf;
        opt["user-agent"]=U_A;
        play->setOptionsForFormat(opt);
        qDebug()<<"1";
        play->setFile(list.at(n));
        play->load();
    }
    else{
        qDebug()<<"gettime::toget:finished";
    }
}

void gettime::got(qint64 t)
{
    if(t>1)
    {
        time+=t;
        qDebug()<<"gottime::got:"<<t;
        emit gottime(time,n);
        errtry=0;
        n++;
    }
    else
    {
        errtry++;
        if(errtry>10) {
            emit gottime(-1,-1);
            return;
        }
    }
    QEventLoop playwait;
    QTimer::singleShot(600, &playwait, SLOT(quit()));
    playwait.exec();
    toget();

}

void gettime::error(QtAV::AVError err){
    qDebug()<<"Qmediaerror:"<<err;
    if(err==QtAV::AVError::NoError) return;
    if(errtry<10)
    {
        errtry++;
        QEventLoop playwait;
        QTimer::singleShot(400, &playwait, SLOT(quit()));
        playwait.exec();
        toget();
    }
    else emit gottime(-1,-1);
}

