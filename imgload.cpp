#include "imgload.h"
#define U_A "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)"

imgload::imgload(int i,QString& imgurl,QObject *parent) :
    QObject(parent)
{
    QDir().mkpath("/tmp/td_img");
    mutex=new QMutex;
    url.setUrl(imgurl);
    got=false;
    index=i;
    reply=NULL;
    tostop=false;
    imgname=imgurl;
    imgname.replace("/","_");

    if(imgname.split("_").last().indexOf(".")==-1) imgname+=".jpg";
    imgname="/tmp/td_img/"+imgname;
    file.setFileName(imgname);
    reque.setUrl(url);
    reque.setRawHeader(QByteArray("User-Agent"), U_A);
    reque.setRawHeader(QByteArray("Referer"), "http://www.tudou.com");
    manager=new QNetworkAccessManager;

}

void imgload::start()
{
    mutex->lock();
    if(!file.exists())
    {
        qDebug()<<"imgload:img exists!";
        reply=manager->get(reque);
        connect(reply,SIGNAL(finished()),this,SLOT(finished()));
    }
    else if(file.size()<2048)
    {
        reply=manager->get(reque);
        connect(reply,SIGNAL(finished()),this,SLOT(finished()));
    }
    else emit loaded(index);
    mutex->unlock();

}
void imgload::stop()
{
    if(reply!=NULL&&!got)
    {
        tostop=true;
        reply->abort();
    }
}

void imgload::finished()
{
    if(tostop)
    {
        reply->deleteLater();
        manager->deleteLater();
        reply=NULL;
        manager=NULL;
        tostop=false;
        return;
    }
    got=true;
    file.open(QIODevice::WriteOnly);
   // qDebug()<<reply->rawHeaderPairs();
    QByteArray data=reply->readAll();
    reply->deleteLater();
    manager->deleteLater();
    file.write(data);
    file.flush();
    emit loaded(index);
    file.close();
    reply=NULL;
    manager=NULL;

}

bool imgload::isgoted()
{
    return got;
}

QString& imgload::getname()
{
    return imgname;
}
