#include "getufromc.h"

getufromc::getufromc(const QStringList &list,QObject *parent)
    :QObject(parent)
{
    qDebug()<<list;
    result="";
    clist=list;
    fin=0;
    task=0;
    all=list.length();
}

getufromc::~getufromc()
{

}

QString getufromc::getresult()
{
    return result;
}

void getufromc::onegeted()
{
    qDebug()<<"getufromc::onegeted:fin:"<<fin<<"task:"<<task<<"all:"<<all;

    getone* download=qobject_cast< getone *>(sender());
    result = result+download->getresult()+"|";
    fin++;
    task++;

    if(fin==all){
        qDebug()<<"getuformc finished emit";
        emit finished(result);
        return ;
    }
    else if(task<all) {
        qDebug()<<"getufromc::onegeted:make a task!";
        download->start(QUrl(clist.at(task)));
        task++;
    }
}

void getufromc::start()
{
    qDebug()<<"getufromc:make a task!";
    download=new getone;
    connect(download,SIGNAL(finish()),this,SLOT(onegeted()));
    download->start(QUrl(clist.at(task)));


}
