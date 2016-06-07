#include "shoucang.h"
#include "ui_shoucang.h"
extern QString cfgpath;

shoucang::shoucang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::shoucang)
{
    ui->setupUi(this);
    setLayout(ui->verticalLayout);
    readls();
    readsc();
}

shoucang::~shoucang()
{
    delete ui;
}

void shoucang::addshoucang(QStringList &info){
    int n=ui->shoucang_list->count();
    for(int i=0;i<n;i++)
    {
        if(ui->shoucang_list->item(i)->data(Qt::UserRole).toString()==info.at(1))  return;
    }

    item=new QListWidgetItem;
    item->setSizeHint(QSize(0,25));
    QString text=info.at(0);
    text.replace("\n"," ");
    item->setText(text);
    item->setData(Qt::UserRole,QVariant(info.at(1)));
    item->setToolTip(info.at(2));
    ui->shoucang_list->addItem(item);
    savesc();
}

void shoucang::addlishi(QStringList &info){
    int n=ui->lishi_list->count();
    for(int i=0;i<n;i++)
    {
        if(ui->lishi_list->item(i)->data(Qt::UserRole).toString()==info.at(1))
        {
            item=ui->lishi_list->takeItem(i);
            item->setToolTip(info.at(2));
            ui->lishi_list->insertItem(0,item);
            return;
        }
    }
    item=new QListWidgetItem;
    item->setSizeHint(QSize(0,25));
    QString text=info.at(0);
    text.replace("\n"," ");
    item->setText(text);
    item->setData(Qt::UserRole,QVariant(info.at(1)));
    item->setToolTip(info.at(2));
    //ui->lishi_list->addItem(item);
    ui->lishi_list->insertItem(0,item);
    savels();
}


void shoucang::on_shoucang_list_itemDoubleClicked(QListWidgetItem *item)
{
    QString url=item->data(Qt::UserRole).toString();
    emit toplay(url);
}

void shoucang::on_lishi_list_itemDoubleClicked(QListWidgetItem *item)
{
    QString url=item->data(Qt::UserRole).toString();
    emit toplay(url);
}

void shoucang::readls(){
    QFile file;
    QString filename=cfgpath+"lssave";
    file.setFileName(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"shoucang::readls";
        while(!file.atEnd())
        {
            QStringList info;
            QString t=file.readLine();
            t.remove("\n");
            if(t.isEmpty()) return;
            info=t.split("$$");
            item=new QListWidgetItem;
            //qDebug()<<info;
            item->setSizeHint(QSize(0,25));
            item->setText(info.at(0));
            item->setData(Qt::UserRole,QVariant(info.at(1)));
            item->setToolTip(info.at(2));
            ui->lishi_list->addItem(item);
        }
        qDebug()<<"shoucang::readls:lishi read:"<<ui->lishi_list->count();
    }
    file.close();

}

void shoucang::readsc(){
    QFile file;
    QString filename=cfgpath+"scsave";
    file.setFileName(filename);
    if(file.open(QIODevice::ReadOnly))
    {

        while(!file.atEnd())
        {
            QStringList info;
            QString t=file.readLine();
            t.remove("\n");
            if(t.isEmpty()) break;
            info=t.split("$$");
            item=new QListWidgetItem;
            item->setSizeHint(QSize(0,25));
            item->setText(info.at(0));
            item->setData(Qt::UserRole,QVariant(info.at(1)));
            item->setToolTip(info.at(2));
            ui->shoucang_list->addItem(item);

        }
    }
    file.close();
}

void shoucang::savels()
{
    QFile file;
    //QStringList list;
    QString filename=cfgpath+"lssave";
    qDebug()<<filename;
    file.setFileName(filename);
    QTextStream out(&file);
    if(file.open(QIODevice::WriteOnly))
    {
        QString info="";
        int i;
        int n=ui->lishi_list->count();
        for(i=0;i<n;i++)
        {
            info=ui->lishi_list->item(i)->text()+"$$"+ui->lishi_list->item(i)->data(Qt::UserRole).toString()+"$$"+ui->lishi_list->item(i)->toolTip();
            //list.append(info);
            out<<info<<"\n";
        }
        qDebug()<<filename<<"shoucang file saved";
    }
    file.close();

}

void shoucang::savesc(){
    QFile file;
    QString filename=cfgpath+"scsave";
    file.setFileName(filename);
    QTextStream out(&file);
    if(file.open(QIODevice::WriteOnly))
    {
        QString info="";
        int i;
        int n=ui->shoucang_list->count();
        for(i=0;i<n;i++)
        {
            info=ui->shoucang_list->item(i)->text()+"$$"+ui->shoucang_list->item(i)->data(Qt::UserRole).toString()+"$$"+ui->shoucang_list->item(i)->toolTip();
            //qDebug()<<info;
            out<<info<<"\n";
        }
    }
    file.close();

}

void shoucang::on_pushButton_2_clicked()
{
    int result= QMessageBox::question(NULL, "删除", "确定删除该收藏？","是","否",NULL,0);
    if(result==0)  ui->shoucang_list->takeItem(ui->shoucang_list->currentRow());
    savesc();
}

void shoucang::on_pushButton_3_clicked()
{
    int result= QMessageBox::question(NULL, "清空", "确定删除所有收藏？","是","否",NULL,0);
    if(result==0) ui->shoucang_list->clear();
    savesc();
}

void shoucang::on_pushButton_4_clicked()
{
    int result= QMessageBox::question(NULL, "删除", "确定删除该历史？","是","否",NULL,0);
    if(result==0)  ui->lishi_list->takeItem(ui->lishi_list->currentRow());
    savels();
}

void shoucang::on_pushButton_clicked()
{
    int result= QMessageBox::question(NULL, "清空", "确定删除所有历史？","是","否",NULL,0);
    if(result==0) ui->lishi_list->clear();
    savels();
}
