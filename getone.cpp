#include "getone.h"
#define U_A "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)"
getone::getone(QObject *parent)
    :QObject(parent)
{
    result="";
}

getone::~getone()
{

}

QString getone::getresult()
{
    return result;
}

void getone::replyfinished(){
//    QString l=reply->rawHeader("Content-Length");
//    int len=l.toInt();
    QString h=reply->readAll();
    reply->deleteLater();
    //qWarning()<<reply->rawHeaderPairs();
    reg.setPattern("http:.+?(?=<)");
    match=reg.match(h);
    result=match.captured(0);
    result.remove("amp;");
    if(result.isEmpty())
    {
        reg.setPattern("http:.");
        match=reg.match(h);
        result=match.captured(0);
    }
    qDebug()<<"getone:"<<h.size()<<result;

    emit finish();
}

void getone::start(const QUrl &url)
{
    QString u;
    QNetworkAccessManager manager;
    QEventLoop loop;
    //发出请求，获取目标地址的头部信息
    QNetworkRequest requ;
    requ.setRawHeader(QByteArray("User-Agent"), U_A);
    QNetworkReply *rep = manager.get(QNetworkRequest(url));
    QObject::connect(rep, SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    qDebug()<<rep->rawHeaderPairs();
    if(rep->hasRawHeader("Location"))
    {
        u=rep->rawHeader("Location");
        qDebug()<<"getone:"<<u;
    }
    else{
        u=url.toString();
        QString h=rep->readAll();
        //qWarning()<<reply->rawHeaderPairs();
        reg.setPattern("http:.+?(?=<)");
        match=reg.match(h);
        result=match.captured(0);
        result.remove("amp;");
//        if(result.isEmpty())
//        {
//            reg.setPattern("http:.");
//            match=reg.match(h);
//            result=match.captured(0);
//        }
        qDebug()<<"getone::start:"<<result;
        emit finish();
        //rep->deleteLater();
        return ;
    }
    //rep->deleteLater();
   // qDebug()<<u;
    QTimer::singleShot(100, &loop, SLOT(quit()));
    loop.exec();//延迟100ms，尝试避免第二次请求返回空数据。
    result="";
    QNetworkRequest reque;
    reque.setRawHeader(QByteArray("User-Agent"), U_A);
    reque.setUrl(QUrl(u));
    reply=man.get(reque);
    connect(reply,SIGNAL(finished()),this,SLOT(replyfinished()));
}
