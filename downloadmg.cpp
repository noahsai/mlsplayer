#include "downloadmg.h"

downloadmg::downloadmg(QObject *parent) :
    QObject(parent)
{
    U_A ="Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)";
    one = new downloadone;
    connect(one,SIGNAL(stoped()),this,SLOT(save()));
    connect(one,SIGNAL(stoped()),this,SIGNAL(stoped()));
    connect(one,SIGNAL(finished()),SLOT(next()));
    connect(one,SIGNAL(progress(int)),this,SIGNAL(progress(int)));
    connect(one,SIGNAL(error()),this,SIGNAL(error()));
    filename="";
    dirpath="";
    now=0;
}
downloadmg::~downloadmg()
{
     one->deleteLater();
}

void downloadmg::seturlpath(QString& name,QString& urllist,QString& path,QString& refurl,QString& ua)
{
    files.clear();
    if(!ua.isEmpty()) U_A = ua;
    filename=name;
    urls=urllist.split("|");
    dirpath=path;
    ref=refurl;
    if(urls.last().isEmpty()) urls.removeLast();
    all=urls.length();
    QDir().mkpath(path);
    qDebug()<<urls;

}

void  downloadmg::start(int goon)
{
    qDebug()<<"downloadmg::start";
    QString name;
    for(now=0;now<=goon;now++)//先获取已下载的 和 这次的文件名
    {
        name=dirpath+"/"+filename+"_"+QString().setNum(now);
        files.append(name);
    }
    now=goon;//循环后now > goon

    QString part;
    part.sprintf("[%d/%d]",now+1,all);
    qDebug()<<part;
    emit onefinished(part);

    QString url=urls.at(now);
    one->setnameurl(name,url,ref,U_A);
    qDebug()<<"downloadmg::start now:"<<now;
    one->start();
}

void  downloadmg::next()
{
    now+=1;
    if(now>=all){
        save();
        emit finished();
        return;
    }
    else{
        start(now);
    }
}

void  downloadmg::stop()
{
    qDebug()<<"downloadmg::stop()";
    one->stop();
}


void  downloadmg::save()
{
    qDebug()<<"downloadmg:to save cfg";
    QFile file;
    QString cfg=dirpath+"/cfg";
    QDataStream out;
    out.setDevice(&file);
    file.setFileName(cfg);
    if(file.open(QIODevice::WriteOnly))
    {
        if(now>=all){//下载完毕，cfg将保存文件名列表，并生成concat.txt合并文件
            out<<files;
            if(all>1)
            {
                QFile cfile;
                QString concat;
                concat=dirpath+"/concat.txt";
                QTextStream o(&cfile);
                cfile.setFileName(concat);
                if(cfile.open(QIODevice::WriteOnly))
                {
                    for(int i=0;i<files.length();i++)
                    {
                        o<<"file \'"<<files.at(i)<<"\'\n";
                    }
                    o.flush();
                }
                else qDebug()<<"downloadmg:concat.txt生成失败";
                cfile.close();
            }
        }
        else {
        out<<(int)all;
        out<<filename;
        out<<(int)now;
        out<<ref;//将playurl设置成refer也没事！见huancun::seturlpath
        out<<urls.join("|");
        out<<ref;
        out<<U_A;
        }
    }
    else {
        qDebug()<<"downloadmg:cfg file open error!";
    }
    file.close();
    qDebug()<<"downloadmg:cfg saved";
    emit stoped();
}
