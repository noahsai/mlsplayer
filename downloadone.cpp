#include "downloadone.h"


downloadone::downloadone(QObject *parent) :
    QObject(parent)
{
    U_A =  "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)";
    manager = new QNetworkAccessManager;
    tostop=false;
    got=0;
    all=0;
    havedone=0;
    ps=0;
    reply=NULL;
    trytimes=0;
}

downloadone::~downloadone()
{
    manager->deleteLater();
}

void downloadone::setnameurl(QString& name ,QString& url,QString& refurl,QString& ua)
{
    if(!ua.isEmpty()) U_A = ua;
    reply=NULL;
    trytimes=0;
    havedone=0;
    ps=0;
    got=0;
    filename=name;
    fileurl=url;
    ref=refurl;
    file.setFileName(filename);
    if(file.open(QIODevice::ReadWrite))
    {
        got=file.size();
        qDebug()<<"downloadone:got："<<got;
    }
    else {
        qDebug()<<"downloadone:file readwrite error";
        return;
    }

    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setRawHeader(QByteArray("User-Agent"), U_A.toLatin1());
    request.setRawHeader(QByteArray("Referer"), ref.toLatin1());
    request.setUrl(QUrl(url));
    qDebug() << "downloadone:Getting the file size...";
    QEventLoop loop;
    //发出请求，获取目标地址的头部信息
    QNetworkReply *repl = manager.head(request);
    QObject::connect(repl, SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    if(repl->hasRawHeader(QByteArray("Location")))
    {
        QString url = repl->header(QNetworkRequest::LocationHeader).toString();
        setnameurl(name,url,ref,U_A);
    }
    else{
        QVariant var = repl->header(QNetworkRequest::ContentLengthHeader);
        delete repl;
        all = var.toLongLong();
        qDebug() << fileurl<<endl<<"The file size is: " << all;
    }
}

void downloadone::start(){
    tostop=false;
    qDebug()<<"downloadone::start";
    request.setRawHeader("Range",tr("bytes=%1-").arg(got).toUtf8());
    request.setUrl(QUrl(fileurl));
    request.setRawHeader(QByteArray("User-Agent"), U_A.toLatin1());
    request.setRawHeader(QByteArray("Referer"), ref.toLatin1());
    reply = manager->get(request);

    connect(reply,SIGNAL(readyRead()),this,SLOT(datagot()));
    connect(reply,SIGNAL(finished()),this,SLOT(savefile()));
   // qDebug()<<"reply:"<<reply;
}

void downloadone::stop(){
   // dl->stop();
    tostop=true;
    if(reply!=NULL)
    {
        reply->abort();
    }
    else emit stoped();
    qDebug()<<"downloadone::stop()";
}

void downloadone::savefile(){
    qDebug()<<"downloadone::savefile()";
    file.flush();
    reply->deleteLater();
    if(trytimes==5) {
        emit error();
        return;
    }
    if(!tostop&&(reply->error()!=QNetworkReply::NoError || file.size()==0)&&trytimes<5)
    {
        got=0;
        trytimes++;
        start();
        return;
    }
    file.close();
    if(tostop){
        qDebug()<<"tostop_savefile";
        emit stoped();
    }
    else{
        qDebug()<<"fin_savefile";
        emit finished();
    }
    reply=NULL;
}

void downloadone::next(){
//    got+=512;
//    if(got>=all) {
//        emit finished();
//        return;
//    }
//    else start();
}

void downloadone::datagot()
{
        QNetworkReply *k=qobject_cast<QNetworkReply *>(sender());
        reply=k;//不这样写第一次之后reply就有可能为NULL
       // qDebug()<<"datagot"<<sender();
        //if(!file) return;
        QByteArray data = k->readAll();
        file.seek(got+havedone);
        file.write(data);
        havedone+=data.size();
       // reply->deleteLater();
        int tmp=ps;
        ps=(got+havedone)*100/all;
        if(ps!=tmp){
           qDebug()<<"progress:"<<ps<<"%";
            emit progress(ps);
        }
}
