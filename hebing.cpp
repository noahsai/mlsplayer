#include "hebing.h"
#include "ui_hebing.h"

hebing::hebing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hebing)
{
    ui->setupUi(this);
    ing=false;
    now=0;
    all=0;
    tty = new QProcess;
    connect(tty,SIGNAL(finished(int)),this,SLOT(next()));
    connect(tty,SIGNAL(error(QProcess::ProcessError)),this,SLOT(error()));
}

hebing::~hebing()
{
    delete ui;
}

bool hebing::addtask(QString &title, QString &path)
{
    QListWidgetItem *item;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        if(ui->listWidget->item(i)->data(Qt::UserRole).toString()==path) return false;
    }
    item = new QListWidgetItem;
    item->setText(title);
    item->setData(Qt::UserRole,QVariant(path));
    ui->listWidget->addItem(item);
    all++;
    if(!ing) start();
    return true;
}

void hebing::next()
{
    QString text= ui->listWidget->item(now-1)->text();
    text="[完成]"+text;
    ui->listWidget->item(now-1)->setText(text);
    if(now<all) {
        start();
    }
    else{
        ing=false;
    }
}
void hebing::error()
{
    QString text= ui->listWidget->item(now-1)->text();
    text="[失败]"+text;
    ui->listWidget->item(now-1)->setText(text);
    if(now<all) {
        start();
    }
    else{
        ing=false;
    }
}

void hebing::start()
{
    ing=true;
    QString path=ui->listWidget->item(now)->data(Qt::UserRole).toString();
    QString cat=path+"/concat.txt";
     ffmpeg= QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/FFmpeg/ffmpeg";
   //  ffmpeg="/usr/local/ffmpeg/bin/ffmpeg";
    QString out=path+"/"+path.split("/").last()+".mp4";
    QString cmd=ffmpeg+" -f concat -i "+cat+" -c copy "+out+" -n";//默认不覆盖
    qDebug()<<cmd;
    tty->start(cmd);
    now++;
}

bool hebing::isrunning()
{
    return ing;
}

void hebing::on_pushButton_clicked()
{
    int r=ui->listWidget->currentRow();
    if(now==r) return;
    all--;
    if(r<now) now--;
    ui->listWidget->takeItem(r);

}
void hebing:: setffmpeg(QString& ff)
{
    ffmpeg=ff;
}
