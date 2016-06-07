#include "dllist.h"
#include "ui_dllist.h"

QString cfgpath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.config/mlsplayer/";

dllist::dllist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dllist)
{
    ui->setupUi(this);
    concat = new hebing(this);
    concat->setWindowFlags(Qt::Window);
    setLayout(ui->verticalLayout);
    ui->listWidget->setIconSize(QSize(25,25));
    tasks=0;
    max=2;
    format="super";
    readdlset();
    readlist();
    dlfilepath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/MLScache/";
    ffmpeg="";//开始是空->读取配置文件->还是空就自动检测->没有再设定为默认位置。
}

dllist::~dllist()
{
    delete ui;
}


void dllist::setformat(QString &f)
{
    format=f;
    QString text="当前优先清晰度："+format;
    ui->label->setToolTip(text);
}

void dllist::starttask(QListWidgetItem *item , int n)
{
    if(tasks<max){
        int result=freehc();
        qDebug()<<"dllist:start():find free hc num:"<<result;
        if(result==-1)
        {
            hc = new huancun;
            connect(hc,SIGNAL(progress(int)),this,SLOT(setprogress(int)));
            connect(hc,SIGNAL(onefinished(QString&)),this,SLOT(partfinished(QString&)));
            connect(hc,SIGNAL(finished()),this,SLOT(fin2nexttask()));
            connect(hc,SIGNAL(stoped()),this,SLOT(next()));
            connect(hc,SIGNAL(error()),this,SLOT(error()));
            mglist.append(hc);
        }
        else hc=mglist.at(result);
        QString url=item->data(Qt::UserRole).toString();
        QString path=item->toolTip();
        hc->setnum(n);
        hc->setformat(format);
        hc->seturlpath(url,path);
        hc->start();
        tasks++;
        item->setWhatsThis("dling");
        item->setIcon(QIcon(":/dl.png"));
    }
    else {
        item->setWhatsThis("ready");
        item->setIcon(QIcon(":/ready.png"));
    }
    qDebug()<<"dllist:start():task started!";
}
void dllist::setprogress(int ps){
    huancun *hc = qobject_cast<huancun *>(sender());
    int num=hc->getnum();
    if(num==-1){
        hc->stop();
        return;
    }
    QString t=ui->listWidget->item(num)->data(Qt::UserRole).toString();
    if(t==hc->geturl()){
        QStringList text=ui->listWidget->item(num)->text().split(" ");
        text.replace(text.length()-1,QString().setNum(ps)+"%");
        ui->listWidget->item(num)->setText(text.join(" "));
    }

    else{
        int i=0;
        for(i=0;i<ui->listWidget->count();i++)
        {
            t=ui->listWidget->item(i)->data(Qt::UserRole).toString();
            if(t==hc->geturl()){
                QStringList text=ui->listWidget->item(i)->text().split(" ");
                text.replace(text.length()-1,QString().setNum(ps)+"%");
                ui->listWidget->item(i)->setText(text.join(" "));
            }
        }
        if(i==ui->listWidget->count()) hc->stop();
    }
}

void dllist::partfinished(QString &part)
{
    huancun *hc = qobject_cast<huancun *>(sender());
    int num=hc->getnum();
    if(num==-1){
        hc->stop();
        return;
    }
    QString t=ui->listWidget->item(num)->data(Qt::UserRole).toString();
    if(t==hc->geturl()){
        QStringList text=ui->listWidget->item(num)->text().split(" ");
        text.replace(text.length()-2,part);//replace last
        ui->listWidget->item(num)->setText(text.join(" "));
    }
    else{
        int i=0;
        for(int i=0;i<ui->listWidget->count();i++)
        {
            t=ui->listWidget->item(i)->data(Qt::UserRole).toString();
            if(t==hc->geturl()){
                QStringList text=ui->listWidget->item(i)->text().split(" ");
                text.replace(text.length()-2,part);//replace last
                ui->listWidget->item(i)->setText(text.join(" "));
            }
        }
        if(i==ui->listWidget->count()) hc->stop();
    }
    savelist();
}

void dllist::addstart(QString& name,QString& url,QString& path)
{
    int all=ui->listWidget->count();
    QString t="";
    for(int i=0;i<all;i++)
    {
        t=ui->listWidget->item(i)->data(Qt::UserRole).toString();
        if(t==url) return;
    }

    QListWidgetItem *item=new QListWidgetItem;
    item->setSizeHint(QSize(0,60));
    item->setBackgroundColor(QColor(231, 243, 244));
    item->setIcon(QIcon(":/ready.png"));
    item->setText(name);
    item->setData(Qt::UserRole,QVariant(url));
    QString dirpath=dlfilepath+path;
    item->setToolTip(dirpath);
    ui->listWidget->addItem(item);
    savelist();
    starttask(item,ui->listWidget->count()-1);
}

int dllist::freehc()
{
    if(mglist.length()==0) return -1;
    int working=0;
    for(int i=0;i<mglist.length();i++)
    {
        if(mglist.at(i)->isdownloading())  working++;
    }
    qDebug()<<working<<"dllist:hc is working!"<<"there is "<<mglist.length()<<"hc";
    if(working<max&&working<mglist.length())
    {
        for(int i=0;i<mglist.length();i++)
            if(!mglist.at(i)->isdownloading()) return i;
    }
    else if(working<max&&working==mglist.length()) return -1;
    else if(working==max) return -1;

}

bool dllist::isdownloading()
{
    int i=0;
    for(i=0;i<mglist.length();i++)
    {
        if(mglist.at(i)->isdownloading()) return true;
    }
    return false;
}


void dllist::rmtask(QListWidgetItem* item)
{
    QString cmd=item->toolTip();
    cmd="rm -rf \""+cmd+"\"";
    qDebug()<<cmd;
    system(cmd.toStdString().c_str());
}

void dllist::on_del_pushButton_clicked()
{
    on_stop_pushButton_2_clicked();
    QList<QListWidgetItem*> list=ui->listWidget->selectedItems();
    if(list.length()>0)
    {
        int result= QMessageBox::question(NULL, "删除任务", "确定删除所选任务？","是，并删除文件","是","否",2);
        if(result<2)
        {
            qDebug()<<"dllist:del_clicked";
            for(int i=0;i<list.length();i++)
            {
                int n=ui->listWidget->row(list.at(i));
                //stop(list.at(i),n);//stop() will isdling();
                rm2changedling(n);
                if(result==0)  rmtask(ui->listWidget->item(n));
                ui->listWidget->takeItem(n);
                qDebug()<<"to remove";
            }
        }
    }
    savelist();

}

void dllist::rm2changedling(int r)
{
    for(int i=0;i<mglist.length();i++)
    {
        int num=mglist.at(i)->getnum();
        if(num>r) {
            qDebug()<<"dllist:change dling";
            mglist.at(i)->setnum(num-1);
        }
    }
}
int dllist::isdling(int r)
{
    for(int i=0;i<mglist.length();i++)
    {
        if(mglist.at(i)->getnum()==r) {
            qDebug()<<"dllist:task"<<r<<"isdling at mglist"<<i;
            return i;
        }
    }
    return -1;
}

void dllist::on_start_pushButton_clicked()
{
    QList<QListWidgetItem*> list=ui->listWidget->selectedItems();
    for(int i=0;i<list.length();i++)
    {
        int n=ui->listWidget->row(list.at(i));
        if(isdling(n)!=-1) continue;
        if(list.at(i)->whatsThis()=="fin") continue;
        starttask(ui->listWidget->item(n),n);
    }
    savelist();

}

void dllist::fin2nexttask()
{
    tasks--;
    huancun *hc = qobject_cast<huancun *>(sender());
    hc->setdling(false);
    int n=hc->getnum();
    ui->listWidget->item(n)->setWhatsThis("fin");
    ui->listWidget->item(n)->setIcon(QIcon(":/fin.png"));
    hc->setnum(-1);//reset
    next();//里面有savelist();

}
void dllist::next()
{
    qDebug()<<"dllist::stop2next";
    for(int i=0;i<ui->listWidget->count();i++)
    {
        if(ui->listWidget->item(i)->whatsThis()=="ready"){
            qDebug()<<"find the next task & start";
            starttask(ui->listWidget->item(i),i);
        }
    }
    savelist();
}

void dllist::on_stop_pushButton_2_clicked()
{

    QList<QListWidgetItem*> list=ui->listWidget->selectedItems();
    for(int i=0;i<list.length();i++)
    {
        QListWidgetItem* t=list.at(i);
        if(t->whatsThis()=="ready"||t->whatsThis()=="err"){
            t->setWhatsThis("stop");
            t->setIcon(QIcon(":/stop.png"));
        }
    }
    //
    for(int i=0;i<list.length();i++)
    {
        QListWidgetItem* t=list.at(i);
        int n=ui->listWidget->row(list.at(i));
        stop(t,n);
    }
    savelist();

}
void dllist::stop(QListWidgetItem *item , int n)
{
    int result=isdling(n);
    if(result!=-1) {
        tasks--;
        qDebug()<<"selected isdiling";
        mglist.at(result)->setdling(false);
        mglist.at(result)->setnum(-1);
        ui->listWidget->item(n)->setWhatsThis("stop");
        item->setIcon(QIcon(":/stop.png"));
        qDebug()<<"selected stopedd";
        mglist.at(result)->stop();
    }
}

void dllist::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QString t=ui->listWidget->item(index.row())->whatsThis();
    if(t=="stop")
        starttask(ui->listWidget->item(index.row()),index.row());
    else if(t=="ready"||t=="err"||t=="dling") on_stop_pushButton_2_clicked();
    else {
        QString dirpath=ui->listWidget->item(index.row())->toolTip();
        QStringList filelist;
        QFile file;
        QString cfg=dirpath+"/cfg";
        file.setFileName(cfg);
        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream in;
            in.setDevice(&file);
            in>>filelist;
            emit playfile(filelist);
        }
        file.close();
    }
}

void dllist::error(){
    tasks--;
    huancun *hc = qobject_cast<huancun *>(sender());
    hc->setdling(false);
    int n=hc->getnum();
    ui->listWidget->item(n)->setWhatsThis("err");
    ui->listWidget->item(n)->setIcon(QIcon(":/err.png"));
    hc->setnum(-1);//reset
    next();
}

void dllist::savelist()
{
    qDebug()<<"dllist::savelist()";
    QFile file;
    QString filename=cfgpath+"downloadsave";
    file.setFileName(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        for(int i=0;i<ui->listWidget->count();i++)
        {
            QListWidgetItem *item=ui->listWidget->item(i);
            QString data="";
            if(item->whatsThis()=="fin")data="fin|";
            else data="stop|";
            QString text=item->text();
            text.replace("\n","$$");
            data=data+text+"|"+item->data(Qt::UserRole).toString()+"|"+item->toolTip()+"\n";
            out<<data;
        }
        out.flush();
        qDebug()<<"file saved";

    }
    qDebug()<<"dllist::savelist() saved";
    file.close();
}
void dllist::readlist()
{
    QFile file;
    QString filename=cfgpath+"downloadsave";
    file.setFileName(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QListWidgetItem *item;
        while(!in.atEnd())
        {
            item=new QListWidgetItem;
            QString text=in.readLine();
            QStringList list=text.split("|");
            text=list.at(0);
            if(text=="fin") {
                item->setWhatsThis("fin");
                item->setIcon(QIcon(":/fin.png"));
            }
            else {
                item->setIcon(QIcon(":/stop.png"));
                item->setWhatsThis("stop");
            }
            text=list.at(1);
            text.replace("$$","\n");
            item->setText(text);
            item->setData(Qt::UserRole,QVariant(list.at(2)));
            item->setToolTip(list.at(3));
            item->setSizeHint(QSize(0,60));
            item->setBackgroundColor(QColor(231, 243, 244));
            ui->listWidget->addItem(item);
        }
    }
    file.close();
}

void dllist::readdlset()
{
    QString filepath=cfgpath+"dlset";
    qDebug()<<"dllist:to read dlset";
    QFile file;
    QDataStream out;
    out.setDevice(&file);
    file.setFileName(filepath);
    if(file.open(QIODevice::ReadOnly))
    {
        out>>max;
        out>>dlfilepath;
        out>>ffmpeg;
        if(dlfilepath.isEmpty()) dlfilepath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/MLScache/";
        if(ffmpeg.isEmpty()){
            ffmpeg= QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/FFmpeg/ffmpeg";
            QProcess tty  ;
            tty.start("/bin/which ffmpeg");
            if(tty.waitForStarted(3000))
            {
                if(tty.waitForFinished(3000))
                {
                    QString result=tty.readAllStandardOutput();
                    if(!result.isEmpty()) {
                        ffmpeg=result.remove("\n");
                        qDebug()<<result;
                    }
                    else qDebug()<<"dllist::readdlset:ffmpeg not found in your system.";
                }
            }
            else qDebug()<<tty.readAll();
        }
    }
    else {
        qDebug()<<"dlset file open error!";
    }
    concat->setffmpeg(ffmpeg);
    file.close();
}

void dllist::on_pushButton_clicked()
{
    dlset *set=new dlset;
    connect(set,SIGNAL(setfilepath(QString&,QString&)),this,SLOT(setdlfilepath(QString&,QString&)));
    connect(set,SIGNAL(setmaxtask(int)),this,SLOT(setmaxtasks(int)));
    set->show();
}

void dllist::setdlfilepath(QString &path,QString& ff){
    if(!path.isEmpty()) dlfilepath=path;
    if(!ff.isEmpty()) {
        ffmpeg=ff;
        concat->setffmpeg(ffmpeg);
    }
    else{
        ffmpeg= QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/FFmpeg/ffmpeg";
        QProcess tty  ;
        tty.start("/bin/which ffmpeg");
        if(tty.waitForStarted(3000))
        {
            if(tty.waitForFinished(3000))
            {
                QString result=tty.readAllStandardOutput();
                if(!result.isEmpty()) {
                    ffmpeg=result.remove("\n");
                    qDebug()<<result;
                }
                else qDebug()<<"dllist::setdlfilepath:ffmpeg not found in your system.";
            }
        }
        else qDebug()<<tty.readAll();
        concat->setffmpeg(ffmpeg);
    }
}

void dllist::setmaxtasks(int m){
    max=m;
    int now=0;
    int all=ui->listWidget->count();
    for(int i=0;i<all;i++)
    {
        if(ui->listWidget->item(i)->whatsThis()=="dling") now++;
    }

    if(now<max)
    {
        int n=0;
        for(int i=0;i<all;i++)
        {
            if(ui->listWidget->item(i)->whatsThis()=="ready"&&n<(max-now))
            {
                starttask(ui->listWidget->item(i),i);
                n++;
            }
        }
    }
    else if(now>max)
    {
        int n=0;
        for(int i=0;i<all;i++)
        {
            if(ui->listWidget->item(i)->whatsThis()=="dling"&&n<max)
            {
                n++;
            }
            else if(ui->listWidget->item(i)->whatsThis()=="dling"&&n>=max)
            {
                stop(ui->listWidget->item(i),i);
                ui->listWidget->item(i)->setWhatsThis("ready");
                ui->listWidget->item(i)->setIcon(QIcon(":/ready.png"));
                n++;
            }
        }
    }
}

void dllist::stopall()
{
    for(int i=0;i<mglist.length();i++)
    {
        if(mglist.at(i)->isdownloading()) mglist.at(i)->stop();
    }
    savelist();

}

void dllist::on_pushButton_2_clicked()
{
    QList<QListWidgetItem*> list=ui->listWidget->selectedItems();
    for(int i=0;i<list.length();i++)
    {
        QListWidgetItem* t=list.at(i);
        QString text=t->text();
        text.remove(QRegularExpression("\\n[\\s\\S]*"));
        QString data=t->toolTip();
        concat->addtask(text,data);
    }
    concat->show();
}
