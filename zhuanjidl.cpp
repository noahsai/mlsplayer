#include "zhuanjidl.h"
#include "ui_zhuanjidl.h"
#define U_A "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)"

zhuanjidl::zhuanjidl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zhuanjidl)
{
    ui->setupUi(this);
    setWindowTitle("专辑列表");
    setAttribute(Qt::WA_DeleteOnClose);
    setLayout(ui->verticalLayout);
    manager =  new QNetworkAccessManager;

}

zhuanjidl::~zhuanjidl()
{
    delete ui;
}

void zhuanjidl::on_pushButton_clicked()
{
    QList<QListWidgetItem*> tmplist=ui->listWidget->selectedItems();
    if(tmplist.isEmpty()) return;
    for(int i=0;i<tmplist.length();i++)
    {
        QString data=tmplist.at(i)->data(Qt::UserRole).toString()+"|"+tmplist.at(i)->text();
        list.append(data);
    }
    emit finished(list);
    close();
}

void zhuanjidl::getdata(QString &url)
{
    url="http://www.tudou.com/crp/getAlbumvoInfo.action?charset=utf-8&areaCode=&acode="+url;
    QString refer="http://www.tudou.com";
    reque.setRawHeader(QByteArray("Referer"), refer.toLatin1());
    reque.setRawHeader(QByteArray("User-Agent"), U_A);
    reque.setUrl(QUrl(url));
    reply=manager->get(reque);
    connect(reply,SIGNAL(finished()),this,SLOT(getlist()));
}

void zhuanjidl::getlist()
{
    qDebug()<<"zhuanjidl::getlist().";
    QString all =reply->readAll();
    reply->deleteLater();
    reg.setPattern("itemPlayUrl.+?\\.html");
    //qDebug()<<all;
    matchs=reg.globalMatch(all);
    int ji=1;
    QListWidgetItem *item;
    while(matchs.hasNext())
    {
        QString t=matchs.next().captured(0);
        t=t.remove("itemPlayUrl\":\"");
        qDebug()<<"has next"<<t;
        item=new QListWidgetItem;
        item->setSizeHint(QSize(0,23));
        item->setText("第"+QString().setNum(ji)+"集");
        item->setData(Qt::UserRole,QVariant(t));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->listWidget->addItem(item);
        ji++;
    }
    qDebug()<<"zhuanjidl::getlist finished";
}

void zhuanjidl::on_pushButton_2_clicked()
{
    close();
}
