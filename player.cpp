#include "player.h"
#include "ui_player.h"

#include<QDebug>
#include<QMessageBox>

extern QString cfgpath;//in dllsit.cpp


player::player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::player)
{
    ui->setupUi(this);
    U_A ="Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)";
    QtAV::setFFmpegLogLevel("debug");
    QDir().mkpath(cfgpath);
    whatstime = new QLabel(ui->widget_2);
    whatstime->setBackgroundRole(QPalette::Dark);
    whatstime->setAutoFillBackground(true);
    whatstime->setFixedSize(50,20);
    whatstime->setAlignment(Qt::AlignCenter);
    whatstime->hide();

    workerThread = new QThread(this) ;

    mywebpage *page=new mywebpage();
    webview=new QWebView(this);
    webview->setPage(page);
    webview->hide();
    webview->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    webview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(webview,SIGNAL(linkClicked(QUrl)),this,SLOT(webplay(QUrl)));
    ui->horizontalLayout->addWidget(webview);
    qDebug()<<"start1";

    ku=new pianku;
//    qDebug()<<"start2";
    connect(ku,SIGNAL(toplay(QString&)),this,SLOT(toplay(QString&)));
    connect(ku,SIGNAL(setbase(QString&)),this,SLOT(setbase(QString&)));
    connect(ku,SIGNAL(adddownload(QString&,QString&)),this,SLOT(adddownload(QString&,QString&)));
    //ku->hide();start at ku
    ui->widget_3->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    ui->horizontalLayout->addWidget(ku);
    ku->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    myshoucang=new shoucang(this);
    myshoucang->hide();
    connect(ku,SIGNAL(addshoucang(QStringList&)),myshoucang,SLOT(addshoucang(QStringList&)));
    connect(myshoucang,SIGNAL(toplay(QString&)),this,SLOT(toplay(QString&)));
    ui->horizontalLayout->addWidget(myshoucang);
    myshoucang->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
//    qDebug()<<"start3";

    download=new dllist(this);
    download->hide();
    ui->horizontalLayout->addWidget(download);
    download->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    connect(download,SIGNAL(playfile(QStringList&)),this,SLOT(openfile(QStringList&)));

    ui->widget_2->setLayout(ui->horizontalLayout_4);
    ui->widget_3->setLayout(ui->verticalLayout_4);
    setLayout(ui->horizontalLayout);
    manager=new QNetworkAccessManager(this);
    jar =new QNetworkCookieJar(this);
    manager->setCookieJar(jar);
    ui->widget->setLayout(ui->verticalLayout_2);


    //===============================================

    aplayer = new AVPlayer;
    connect(aplayer,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
   // connect(aplayer,SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),this,SLOT(mediaStatusChanged(QtAV::MediaStatus)));
    connect(aplayer,SIGNAL(stopped()),this,SLOT(playstoped()));
    tmplayer=NULL;
    renderer =new GLWidgetRenderer2();
   // renderer->show();
    aplayer->setRenderer(renderer);
    ui->mplayerlayout->addWidget(renderer);

    renderer->installEventFilter(this);
    ui->horizontalSlider->installEventFilter(this);
    ui->listWidget->installEventFilter(this);
    ui->video->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->pushButton_9->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    ui->pushButton_3->installEventFilter(this);
    ui->pushButton_12->installEventFilter(this);
    ui->pushButton_5->installEventFilter(this);
    ui->pushButton_4->installEventFilter(this);
    ui->yuyan->installEventFilter(this);
    ui->qingxidu->installEventFilter(this);
    ui->widget_2->installEventFilter(this);

    playrf="";
    getduration = NULL;
    base="";
    online=false;
    process = NULL;
    notlistcheck=false;
    autochange=true;
    now_ji=-1;
    zhuanji = 0;
    getfiletype=0;
    mousex=-1,mousey=-1;
    curplay=0;
    init=false;
    ui->listWidget->hide();
    ui->tishi->hide();
    ui->widget->hide();
    ui->yuyan->hide();
    format="high";
    ui->youxian->setCurrentIndex(1);
    readset();
    download->setformat(format);
    ui->widget_2->hide();

    playtimer = new QTimer(this);
    connect(playtimer,SIGNAL(timeout()),this,SLOT(playtimerout()));
    timer = new QTimer(this);
    timer->start(350);
    connect(timer,SIGNAL(timeout()),this,SLOT(inittimeout()));
  //  webview->load(QUrl("http://www.soku.com"));
}

player::~player()
{
    if(workerThread->isRunning())
    {
        workerThread->quit();
        workerThread->wait();
        getduration=NULL;
    }
    delete ui;
}

void player::toplay(QString & url)
{
    if(QFileInfo(url).isFile())
    {
        QStringList tmp(url);
        openfile(tmp);
        return;
    }

    online=true;
    qDebug()<<"player::toplay:"<<url;
    zhuanji=0;
    setWindowTitle("马铃薯在线视频");
    //to play new video with the youxian fomat

    switch(ui->youxian->currentIndex())
    {
    case 0:format="normal";break;
    case 1:format="high";break;
    case 2:format="super";break;
    case 3:format="real";break;
    default:break;//默认高清；
    }
    qDebug()<<"player::toplay():format="<<format;
    //----------------------------------
    on_pushButton_7_clicked();
    reset4other();
    if(url.indexOf("http")==-1)
    {
        qDebug()<<"地址是专辑";
        now_ji=0;
        url="http://www.tudou.com/crp/getAlbumvoInfo.action?charset=utf-8&areaCode=&acode="+url;
        QString refer="http://www.tudou.com";
        reque.setRawHeader(QByteArray("Referer"), refer.toLatin1());
        reque.setRawHeader(QByteArray("User-Agent"), U_A.toLatin1());
        reque.setUrl(QUrl(url));
        reply=manager->get(reque);
        connect(reply,SIGNAL(finished()),this,SLOT(getlist()));
    }
    else
    {
        qDebug()<<"地址是视频";
        now_ji=-1;
        work(encode(url));
    }
}

bool player::you_get(QString url)//查看you-get是否支持
{
    int which=0;
    QProcess pro;
    QString yougetpath=QApplication::applicationDirPath()+"/you-get-develop/you-get";

    QString cmd="python3 "+yougetpath+" -i "+url;
    QProcess tty  ;
    tty.start("/bin/which you-get");
    if(tty.waitForStarted(3000))
    {
        if(tty.waitForFinished(3000))
        {
            QString result=tty.readAllStandardOutput();
            if(!result.isEmpty()) {
                result=result.remove("\n");
                qDebug()<<result;
                cmd = result + " -i "+url;
            }
            else qDebug()<<"player::you_get:you-get not found in your system,use the default.";
        }
    }
    else qDebug()<<tty.readAll();

    qDebug()<<cmd;
    pro.start(cmd);

    if(pro.waitForStarted())
    {
        if(pro.waitForFinished())
        {
            QString result=pro.readAllStandardOutput();
            qDebug()<<result;
            if(result.indexOf("title",0,Qt::CaseInsensitive)!=-1)
            {
                pro.kill();
                if(name=="noname")
                {
                    reg.setPattern("(?<=[Tt][iI][tT][lL][eE]:).+");
                    name=reg.match(result).captured(0);
                    name.remove(" ");
                    qDebug()<<"name"<<name;
                    setWindowTitle(name);
                }
                QStringList cmds;
                QStringList formats;
                if(ui->qingxidu->count()==0)
                {
                    ui->qingxidu->clear();
                    reg.setPattern(" --.+ ");
                    matchs=reg.globalMatch(result);
                    if(!matchs.hasNext())//证明只有一个清晰度
                    {
                        ui->qingxidu->addItem("默认");
                        QString f="";
                        QString urls=you_geturls(url,f);
                        ui->qingxidu->setItemData(0,QVariant(urls));
                        ui->qingxidu->setEnabled(true);
                        play4auto(0);
                        return true;
                    }
                    else
                    {
                        while(matchs.hasNext())
                        {
                            cmds.append(matchs.next().captured(0));
                        }
                        reg.setPattern("(?<=profile: ).+");
                        matchs=reg.globalMatch(result);
                        while(matchs.hasNext())
                        {
                            QString t=matchs.next().captured(0);
                            qDebug()<<t;
                            if(t.indexOf("标清")!=-1||t.indexOf("360")!=-1) formats.append("标清");
                            else if(t.indexOf("高清")!=-1||t.indexOf("480")!=-1) formats.append("高清");
                            else if(t.indexOf("超清")!=-1||t.indexOf("720")!=-1) formats.append("超清");
                            else if(t.indexOf("原画")!=-1||t.indexOf("1080")!=-1) formats.append("原画");
                            else formats.append(t);
                        }
                        ui->qingxidu->addItems(formats);
                        int n=ui->youxian->currentIndex();
                        for(;n>=0;n--)
                        {
                            QString tmp=ui->youxian->itemText(n);
                            for(int i=0;i<formats.length();i++)
                            {
                                if(formats.at(i).indexOf(tmp)!=-1)
                                {
                                    which=i;
                                    n=-1;//用于跳出最外层的循环；
                                }
                            }
                        }
                        for(int i=0;i<cmds.length();i++)
                        {
                            ui->qingxidu->setItemData(i,QVariant(cmds.at(i)));
                        }
                        if(result.indexOf("粤语")!=-1)
                        {
                            reg.setPattern("lang:[\\s\\S]+?\n\n");
                            matchs=reg.globalMatch(result);
                            while(matchs.hasNext())
                            {
                                QString p=matchs.next().captured(0);
                                qDebug()<<p;
                                if(p.indexOf("国语")!=-1)
                                {
                                    reg.setPattern("http:.+");
                                    QString url=reg.match(p).captured(0);
                                    ui->yuyan->setItemData(0,QVariant(url));
                                }
                                if(p.indexOf("粤语")!=-1)
                                {
                                    reg.setPattern("http:.+");
                                    QString url=reg.match(p).captured(0);
                                    ui->yuyan->setItemData(1,QVariant(url));
                                }
                            }
                            ui->yuyan->setVisible(true);
                            ui->yuyan->setEnabled(true);
                        }


                        QString fm=cmds.at(which);
                        QString urls=you_geturls(url,fm);
                        qDebug()<<urls<<endl<<which;
                        ui->qingxidu->setCurrentIndex(which);
                        ui->qingxidu->setItemData(which,QVariant(urls));
                        ui->qingxidu->setEnabled(true);
                        play4auto(which);
                        return true;
                    }
                }
            }
            else {
                qDebug()<<pro.readAll();
                pro.kill();

            }
        }
        else {
            setWindowTitle("解析超时，请重试");
            pro.kill();
        }
    }
    return false;

}

QString player::you_geturls(QString& url,QString& format)
{
    U_A = "Python-urllib/3.5";//用you-get获取的地址需要用这个ua
    youget_format = format;
    QProcess pro;
    QString yougetpath=QApplication::applicationDirPath()+"/you-get-develop/you-get";
    QString cmd="python3 "+yougetpath+" -u "+format+url;
    qDebug()<<cmd;
    pro.start(cmd);
    if(pro.waitForStarted())
    {
        if(pro.waitForFinished())
        {
            QString result=pro.readAllStandardOutput();
//            qDebug()<<result;
            if(result.indexOf("urls",0,Qt::CaseInsensitive)!=-1)
            {
                reg.setPattern("[uU][rR][lL][sS]:[\\s\\S]+");
                QString data=reg.match(result).captured(0);
                reg.setPattern("http.+");
                qDebug()<<data;
                matchs=reg.globalMatch(data);
                QString urls;
                while(matchs.hasNext())
                {
                    urls+=matchs.next().captured(0)+"|";
                }
                qDebug()<<"urls found!"<<urls;
                return urls;
            }
            qDebug()<<"urls not found!";
            return QString("");
        }
        qDebug()<<"you-get waitfinished error!";
        return QString("");
    }
    qDebug()<<"you-get waitfinished error!";
    return QString("");
}


void player::getlist()
{
    qDebug()<<"in getlist().";
    QString all =reply->readAll();
    reply->deleteLater();
    if(theurl.indexOf("listplay")==-1)
    {
        reg.setPattern("itemPlayUrl.+?\\.html");
       // qDebug()<<all;
        matchs=reg.globalMatch(all);
        int ji=1;
        QListWidgetItem *item;
        while(matchs.hasNext())
        {
            QString t=matchs.next().captured(0);
            t=t.remove("itemPlayUrl\":\"");
            //qDebug()<<"player::getlist():has next"<<t;
            item=new QListWidgetItem(ui->listWidget);
            item->setSizeHint(QSize(0,23));
            item->setText("第"+QString().setNum(ji)+"集");
            item->setData(Qt::UserRole,QVariant(t));
            ui->listWidget->addItem(item);
            ji++;
        }
        reg.setPattern("(?<=fjTitle\":\").*?(?=\")");
        matchs=reg.globalMatch(all);
        int n=0;
        int c=ji-1;
        while(matchs.hasNext()&&n<c)
        {
            QString t=matchs.next().captured(0);
            ui->listWidget->item(n)->setToolTip(t);
            n++;
        }
    }
    else {
        reg.setPattern("(?<=\"itemCode\":\").*?(?=\")");

        qDebug()<<all;
        matchs=reg.globalMatch(all);
        QListWidgetItem *item;
        while(matchs.hasNext())
        {
            QString t=matchs.next().captured(0);
            //qDebug()<<"player::getlist():has next"<<t;
            reg.setPattern("(?<=play/)[^./]+");
            QString code=reg.match(theurl).captured(0);
            QString url="http://www.tudou.com/listplay/"+code+"/"+t+".html";
            item=new QListWidgetItem(ui->listWidget);
            item->setSizeHint(QSize(0,23));
            item->setData(Qt::UserRole,QVariant(url));
            ui->listWidget->addItem(item);

        }
        reg.setPattern("(?<=\"title\":\").*?(?=\",)");
        matchs=reg.globalMatch(all);
        int n=0;
        int c=ui->listWidget->count();
        while(matchs.hasNext()&&n<c)
        {
            QString t=matchs.next().captured(0);
            ui->listWidget->item(n)->setText(t);
            ui->listWidget->item(n)->setToolTip(t);
            n++;
        }
    }
    //
    int i=0;
    QString wtitle=this->windowTitle();
    QString data;
    for(i=0;i<ui->listWidget->count();i++)
    {
        data=ui->listWidget->item(i)->data(Qt::UserRole).toString();
        if(data==theurl){
            qDebug()<<"第"<<i<<"找到相同的url";
            ui->listWidget->setCurrentRow(i);
            now_ji=i;
            break;
        }
        else
        {
           data=ui->listWidget->item(i)->toolTip();
           data.trimmed();
           if((!data.isEmpty())&&wtitle.indexOf(data)!=-1)
           {
               qDebug()<<"第"<<i<<"找到相似的标题";
               ui->listWidget->setCurrentRow(i);
               now_ji=i;
               break;
           }
        }
    }
    if(i==ui->listWidget->count())
    {
        now_ji=i;//由于无法判断哪一集，设为最大，默认不自动切换
        if(theurl.indexOf("listplay")!=-1) ui->listWidget->setCurrentRow(0);
    }
    if(ui->listWidget->count()==1&&(aplayer->state()==AVPlayer::StoppedState||aplayer->state()==AVPlayer::PausedState))
    {
        ui->listWidget->setCurrentRow(0);
        on_listWidget_itemDoubleClicked(ui->listWidget->item(0));
        ui->widget->hide();
        ui->listWidget->hide();
    }
    else
    {
        ui->pushButton_5->setChecked(true);
        ui->widget->show();
        ui->listWidget->show();
    }

}

void player::work(const QString &edurl)
{
    U_A ="Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)";

    qDebug()<<"player::work()";
    //ui->huanchong->show();
    url.setUrl(edurl);
    QString refer="http://www.flvcd.com";
    reque.setRawHeader(QByteArray("Referer"), refer.toLatin1());
    reque.setRawHeader(QByteArray("User-Agent"), U_A.toLatin1());
    reque.setUrl(url);

    reply=manager->get(reque);
    connect(reply,SIGNAL(finished()),this,SLOT(getfile()));
}

QString player::encode(const QString& url)
{
    reset4self();
    ui->qingxidu->clear();
    //---------reset end--------
    ui->tishi->show(); //显示正在解析的提示
    QString a;
    QUrl u(url);
    theurl=url;
    a=u.toEncoded().toPercentEncoding();
    a="http://www.flvcd.com/parse.php?kw="+a+"&format="+format+"&go=1";//加上清晰度
    qDebug()<<"player::encode():"<<a;
    return a;
}
void player::getfile()
{
    qDebug()<<"player::getfile():reply finished:"<<getfiletype;
   // qDebug()<<reply->rawHeaderPairs();
    if(reply->error()==QNetworkReply::NoError)
    {
        QByteArray bytes =reply->readAll();
        reply->deleteLater();
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        html = codec->toUnicode(bytes);
        qDebug()<<html;
        switch(getfiletype)
        {
        case 0:fenxi();break;
        case 1:getsite();break;
        case 2:fenxi3();break;
        case 3:fenxi4();break;
        default:qDebug()<<"player::value of getfiletype is error!";break;
        }
    }
    else this->setWindowTitle("网络错误，请重试。");
}

void player::getsite()
{
    if(zhuanji==0) getfiletype=2;
    else if(zhuanji==1)getfiletype=3;
    reg.setPattern("(?<=id=).+(?=\")");
    match=reg.match(html);
    if(match.hasMatch())
    {
        QString url="http://www.flvcd.com/diy/diy00";
        url=url+match.captured(0)+".htm";
        //qDebug()<<html<<endl<<url;
        reque.setUrl(QUrl(url));
        reply=manager->get(reque);
        connect(reply,SIGNAL(finished()),this,SLOT(getfile()));
    }
    else {
        setWindowTitle("无法播放该视频！");
        ui->tishi->hide();
    }

}

void player::fenxi3()
{
    QString t;
    QTextStream in(&html);
    //qDebug()<<html;
    QStringList cs;
    while(!in.atEnd())
    {
        QString tmp=in.readLine();
        if(tmp.indexOf("<U>http")!=-1){
            tmp.remove("<U>");
            t=t+tmp+"|";
        }
        else if(playrf==""&&tmp.indexOf("<RF>")!=-1)
        {
            tmp.remove("<RF>");
            playrf=tmp;
        }
        else if(tmp.indexOf("<C>http")!=-1){
            tmp.remove("<C>");
            cs.append(tmp);

        }
    }
    if(!cs.isEmpty())
    {
        qDebug()<<"player::fenxi3:cs not empty";
        getufromc *getu=new getufromc(cs);
        connect(getu,SIGNAL(finished(QString&)),this,SLOT(getcsforplay(QString&)));
        getu->start();
        return;
        //qDebug()<<"player::fenxi3:result:"<<t;
    }
    if(t.isEmpty()){
        setWindowTitle("解析失败，请重试！");
        return;
    }

    int which=-1;
    if(format=="normal"||format=="") which=0;
    else if(format=="high") which=1;
    else if(format=="super") which=2;
    else if(format=="real") which=3;

    if(which!=-1)
    {
        ui->qingxidu->setItemData(which,QVariant(t));
        ui->qingxidu->setCurrentIndex(which);
        play4auto(which);
        showbutton(true);
    }
    else return;

}

void player::fenxi4()
{
    showbutton(true);
    ui->listWidget->show();
    QStringList url;
    QStringList part;
    QStringList cs;
    QTextStream in(&html);
    while(!in.atEnd())
    {
        QString tmp=in.readLine();
        if(tmp.indexOf("<U>http")!=-1){
            tmp.remove("<U>");
            url.append(tmp);
        }
        else if(tmp.indexOf("<C>http")!=-1){
            tmp.remove("<C>");
            cs.append(tmp);
        }
        else if(tmp.indexOf("<X>")!=-1)
        {
            tmp.remove("<X>");
            part.append(tmp);
        }
        qDebug()<<tmp;
    }
    qDebug()<<"player::fenxi4:"<<part.count()<<part;
    if(url.isEmpty())
    {
        getufromc *getu=new getufromc(cs);
        QEventLoop loop;
        QObject::connect(getu,SIGNAL(finished()),&loop,SLOT(quit()));
        getu->start();
        loop.exec();
        url=getu->getresult().split("|");
        qDebug()<<url.count()<<url.last();
    }
    //qDebug()<<part<<"\n=============\n"<<url;
    in.flush();

    ui->video->clear();
    ui->listWidget->clear();
    ui->listWidget->show();
    QStringList alldata;
    QListWidgetItem *item;
    int ji=1;
    item=new QListWidgetItem(ui->listWidget);
    item->setSizeHint(QSize(0,23));
    item->setText("第"+QString().setNum(ji)+"集");
    QString t=url.at(0)+"|";
    //qDebug()<<part<<"\n=============\n"<<url;
    int i=0;
    for(i=0;i<part.length()-1;i++)//因为里面用到i+1，所以这里的范围要length()-1！！
    {
        if(part.at(i+1)==part.at(i))
        {

                t=t+url.at(i+1)+"|";
        }
        else{
           // qDebug()<<"\n===="<<part.at(i)<<"=====\n"<<t;
           // item->setWhatsThis(t);
            item->setData(Qt::UserRole,QVariant(t));
            alldata.append(t);
            ui->listWidget->addItem(item);
            ji++;
            item=new QListWidgetItem(ui->listWidget);
            item->setSizeHint(QSize(0,23));
            item->setText("第"+QString().setNum(ji)+"集");
            t=url.at(i+1)+"|";
        }
    }
   // qDebug()<<"\n===="<<part.at(i)<<"=====\n"<<t;
    item->setData(Qt::UserRole,QVariant(t));
    ui->listWidget->addItem(item);
    alldata.append(t);
    int which=-1;
    if(format=="normal"||format=="") which=0;
    else if(format=="high") which=1;
    else if(format=="super") which=2;
    else if(format=="real") which=3;

    if(which!=-1)
    {
        ui->qingxidu->setItemData(which,QVariant(alldata));
        ui->qingxidu->setCurrentIndex(which);
    }
    ui->pushButton_5->setChecked(true);
    ui->widget->show();
    ui->tishi->hide();
    //因为是专辑，所以不自动播放。
}

void player::itemclicked(QListWidgetItem *item)
{
    QString cmd=item->whatsThis();
    cmd.replace("\n"," ");;
    cmd = "smplayer " + cmd;
    QProcess::startDetached(cmd);
}

void player::getcsforplay(QString &t)
{
    qDebug()<<"got cs toplay";
    int which=-1;
    if(format=="normal"||format=="") which=0;
    else if(format=="high") which=1;
    else if(format=="super") which=2;
    else if(format=="real") which=3;

    if(which!=-1)
    {
        ui->qingxidu->setItemData(which,QVariant(t));
        ui->qingxidu->setCurrentIndex(which);
        play4auto(which);
        showbutton(true);
    }
}

bool player::fenxi()
{
    name="noname";
    bool has_set_level=true;
    qDebug()<<"player::fenxi():分析开始";
    //没有"当前解析"就是解析出错.
    if(html.indexOf("当前解析")==-1) {
    //flvcd无法解析
        if(html.indexOf("观看广告")!=-1)
        {
            QString sc="";
            reg.setPattern("[0-9]{32}");
            match=reg.match(html);
            QString num32=match.captured(0);
            reg.setPattern("(?<=')[0-9a-z]{32}");
            match=reg.match(html);
            QString a=match.captured();
            reg.setPattern("(?<==)[0-9]{13};");
            match=reg.match(html);
            QString t=match.captured();
            t.remove(";");
            qlonglong n=t.toLongLong();
            sc=getsc(a,n,num32);
            qDebug()<<"ad:sc"<<sc;
            QNetworkCookie cs,go;
            QDateTime local(QDateTime::currentDateTime());
            local=local.addSecs(300);

            cs.setName("go");
            cs.setValue(sc.toStdString().c_str());
            cs.setExpirationDate(local);
            cs.setDomain(".flvcd.com");
            go.setName("avdGggggtt");
            go.setValue(t.toStdString().c_str());
            go.setExpirationDate(local);
            go.setDomain(".flvcd.com");

            qDebug()<<cs<<endl<<go;

            jar= manager->cookieJar();
            jar->insertCookie(cs);
            jar->insertCookie(go);
            manager->setCookieJar(jar);
            QString re=reque.url().toString();
            reque.setRawHeader(QByteArray("Referer"), re.toLatin1());
            reply=manager->get(reque);

            connect(reply,SIGNAL(finished()),this,SLOT(getfile()));
            return true;
        }
        //这时用you-get 解析
        if(you_get(theurl)){
            QStringList info;
            info.append(name);
            info.append(theurl);
            QDateTime time = QDateTime::currentDateTime();
            info.append(time.toString("yyyy.MM.dd hh:mm:ss"));
            myshoucang->addlishi(info);
            return true;
        }
        else{
            setWindowTitle("没找到当前解析，无法播放视频！");
            ui->tishi->hide();
            return false;
        }
    }
    reg.setPattern("(?<=document.title = \").+?(?=\")");

    match=reg.match(html);
    if(match.hasMatch())
    {
        qDebug()<<"player::has name match";
        name = match.captured();
        setWindowTitle(name);
        //--------addlishi---------
        QStringList info;
        info.append(name);
        info.append(theurl);
        QDateTime time = QDateTime::currentDateTime();
        info.append(time.toString("yyyy.MM.dd hh:mm:ss"));
        myshoucang->addlishi(info);
        //------addlishi end-------

        if(html.indexOf("无法下载")!=-1)
        {
            if(you_get(theurl)) return true;
            else return false;
        }


        if (format=="normal") {
            if(name.indexOf("标清版")==-1)
            {
                qDebug()<<"没有标清版";
                has_set_level = false;
            }
        }
        else if (format=="high") {
            if(name.indexOf("高清版")==-1) {
                qDebug()<<"没有高清版";
                has_set_level = false;
            }
        }
        else if(format== "super"){
            if(name.indexOf("超清版")==-1) {
                qDebug()<<"没有超清版";
                has_set_level = false;
            }
        }
        else if(format== "real"){
            if(name.indexOf("原画")==-1) {
                qDebug()<<"没有原画版";
                has_set_level = false;
            }
        }
        if(!has_set_level)//没有设定的清晰度
        {   //找该视频的最高清晰度，没有设置的清晰度时用该清晰度代替。
            if(format=="normal"&&html.indexOf("高清版")!=-1)
            {
                qDebug()<<"其实你就是标清！";//有高清却没标清，这么现在这个就是标清！
                format="";//设为""，让程序不要进work();
            }
            else if(html.indexOf("超清版")!=-1) format="super";
            else if(html.indexOf("高清版")!=-1) format="high";
            else if(html.indexOf("标清版")!=-1) format="normal";
            else format="";//说明没有清晰度选择；
            if(!(format==""))
            {
                work(encode(theurl));
                return false;
                }//有清晰度选择才work();
            else qDebug()<<"player::fenxi():format not found!"<<format;
        }
    }
    else qDebug()<<"player::fenxi():hasn't name match";

    if(ui->qingxidu->count()==0)
    {
        QStringList qingxi;
       qingxi.append("标清");
        if(html.indexOf("高清版")!=-1)
        {
           qingxi.append("高清");
        }
        if(html.indexOf("超清版")!=-1||html.indexOf("全高清")!=-1||html.indexOf("720P")!=-1)
        {
            qingxi.append("超清");
        }
        if(html.indexOf("原画")!=-1||html.indexOf("1080P")!=-1)
        {
           qingxi.append("原画");
        }
        ui->qingxidu->insertItems(0,qingxi);
        ui->qingxidu->setEnabled(false);
    }
    reg.setPattern("\\|\\s*<a href=\".+?(?=\")");
    match=reg.match(html);
    if(match.hasMatch())
    {
        QString t=match.captured(0);
        qDebug()<<"has 语言"<<t;
        reg.setPattern("\\|\\s*\\<a href=\"parse.php\\?kw=");
        t.remove(reg);
        if(html.indexOf("国语版")!=-1)
        {
            qDebug()<<"国语";
            ui->yuyan->setItemData(0,QVariant(t));
            ui->yuyan->setItemData(1,QVariant(theurl));
            ui->yuyan->setCurrentIndex(1);//找到国语证明现在是粤语，所以yuyan设为1（粤语）
        }
        else if(html.indexOf("粤语版")!=-1)
        {
            qDebug()<<"粤语";
            ui->yuyan->setItemData(1,QVariant(t));
            ui->yuyan->setItemData(0,QVariant(theurl));
            ui->yuyan->setCurrentIndex(0);//找到粤语证明现在是国语，所以yuyan设为0（国语语）
        }
        ui->yuyan->show();

    }

    if(html.indexOf("只能用硕鼠")!=-1)
    {
        qDebug()<<"只能用硕鼠";
        getfiletype=1;
        fenxi2();
        return false;
    }

    else {
        reg.setPattern("(?<=name=\"inf\" value=\").+?(?=\")");
        match=reg.match(html);
      //  if(!matchs.hasNext())return false;
        if(match.hasMatch())
        {
            QString t=match.captured(0);

            int which=-1;
            if(format=="normal"||format=="") which=0;
            else if(format=="high") which=1;
            else if(format=="super") which=2;
            else if(format=="real") which=3;
            //if(which!=-1) list.at(which)->setWhatsThis(t);
            if(which!=-1)
            {
                ui->qingxidu->setItemData(which,QVariant(t));
                ui->qingxidu->setCurrentIndex(which);
                play4auto(which);
            }
            else return false;
        }
        else {
            getfiletype=1;
            fenxi2();
            //setWindowTitle("获取失败！");
            return false;
        }
        showbutton(true);
    }
    return true;
}

void player::showbutton(bool show){
    ui->qingxidu->setEnabled(show);
}


bool player::fenxi2()
{
    reg.setPattern("id=\"parse_form\"[\\s\\S]+name=\"passport\"");
    QStringList form;
    match=reg.match(html);
    if(match.hasMatch())
    {
        QString tmp=match.captured(0);
        reg.setPattern("(?<=action=\").+?(?=\")");
        match=reg.match(tmp);
        QString server=match.captured(0)+"?";
        reg.setPattern("(?<=name=\").+\"\\s+value=\".*?(?=\">)");
        matchs=reg.globalMatch(tmp);
        while(matchs.hasNext())
        {
            form.append(matchs.next().captured(0));
        }
        //qDebug()<<form;
        zhuanji=-1;//只为了里面不用每次都判断是否等于"t",感觉判断int比qstring快点吧;
        if(!form.isEmpty())
        {
            QStringList keyvalue;
            reg.setPattern("\".+value=\"");
            bool havesc=false;
            for(int i=0;i<form.length();i++)
            {
                keyvalue=form.at(i).split(reg);
                QString a=keyvalue.at(1);
                if(keyvalue.at(0)=="name") continue;
                if(keyvalue.at(0)=="url"){
                QUrl u(a);
                a=u.toEncoded().toPercentEncoding();
                }
                if(keyvalue.at(0)=="sc") havesc=true;
                server=server+keyvalue.at(0)+"="+a+"&";
                if(zhuanji==-1) {
                    if(keyvalue.at(0)=="t") {
                        if(keyvalue.at(1)=="1") zhuanji=1;
                        else zhuanji=0;
                    }
                }
            }
            if(!havesc){
                QString sc="";
                reg.setPattern("[0-9]{32}");
                match=reg.match(html);
                QString num32=match.captured(0);
                reg.setPattern("(?<=createSc\\(\").+?(?=\\))");
                match=reg.match(html);//下面直接用match
                reg.setPattern("\" *, *(?=\\d)");
                QStringList kv=match.captured(0).split(reg);
                qDebug()<<"num32:"<<num32<<"createsc:"<<kv;
                QString a=kv.at(0);
                qlonglong n=kv.at(1).toLongLong();
                sc=getsc(a,n,num32);
                qDebug()<<"sc"<<sc;
                server+="sc="+sc;
            }
            QUrl url(server);
            qDebug()<<url;
            QString refer=reque.url().toString();
//            qDebug()<<refer;
            reque.setRawHeader(QByteArray("Referer"), refer.toLatin1());
            reque.setRawHeader(QByteArray("User-Agent"), U_A.toLatin1());

            reque.setUrl(url);
            reply=manager->get(reque);
            connect(reply,SIGNAL(finished()),this,SLOT(getfile()));
        }
    }
    else return false;
    return true;

}




void player::play4auto(int i)
{
    qDebug()<<" 开始播放"<<"RF:"<<playrf;
    if(ui->qingxidu->count()==0) return;
    ui->tishi->hide();
    if(ui->pushButton_7->isChecked()) ui->widget_3->hide();
    QString cmd=ui->qingxidu->itemData(i).toString();
    if(cmd.isEmpty()) return;
    QStringList partlist=cmd.split("|");
    QStringList noemptylist;//用于获取全部时间
    ui->video->clear();
    QListWidgetItem *item;
    QNetworkRequest *request;
    int n=partlist.length();
    qDebug()<<"partlist:"<<n;
    for(int i=0;i<n;i++)
    {
        if(partlist.at(i).isEmpty())continue;
        noemptylist.append(partlist.at(i));
        item=new QListWidgetItem(QString("第"+QString().setNum(i+1)+"段"));
        item->setData(Qt::UserRole,QVariant(partlist.at(i)));
        ui->video->addItem(item);
        //qDebug()<<partlist.at(i);
    }
    //==========reset4self()已经将workerThread 和 getduration 重置===============
//    qDebug()<<":play4auto(int i):workerThread is runing?"<<workerThread->isRunning();
//    if(workerThread->isRunning())
//    {
//        workerThread->quit();
//        workerThread->wait();
//        getduration=NULL;
//    }
//    qDebug()<<"new a gettime";
//    if(getduration!=NULL) getduration->deleteLater();
//    qDebug()<<"can new a gettime";
    getduration = new gettime(noemptylist,playrf,U_A);
//    qDebug()<<"newed a gettime";
   // connect(getduration,SIGNAL(gottime(qint64,int)),this,SLOT(gottime(qint64,int)));
   // getduration->toget();
    getduration->moveToThread(workerThread);
    connect(workerThread, SIGNAL(finished()), getduration, SLOT(deleteLater()));
    connect(workerThread, SIGNAL(started()), getduration, SLOT(toget()));
    connect(getduration,SIGNAL(gottime(qint64,int)),this,SLOT(gottime(qint64,int)));
    workerThread->start();

//    request = new QNetworkRequest;
//    if(playrf=="") request->setRawHeader(QByteArray("Referer"), theurl.toLatin1());
//    else request->setRawHeader(QByteArray("Referer"), playrf.toLatin1());
//    request->setRawHeader(QByteArray("User-Agent"),U_A.toLatin1());
    QString data=ui->video->item(0)->data(Qt::UserRole).toString();
 //   ui->video->setCurrentRow(0);
    if(data.isEmpty()) {
        setWindowTitle("解析失败，请重试！");
        return;
    }
    //qDebug()<<data;


    //
    QVariantHash opt;
    if(playrf=="")  opt["referer"]= theurl;
    else  opt["referer"]= playrf;
    opt["user-agent"]=U_A;
    aplayer->setOptionsForFormat(opt);
    aplayer->setFile(data);
    aplayer->audio()->setVolume(ui->horizontalSlider_2->value()/20.0);
    if(aplayer->mediaStatus()!=QtAV::NoMedia&&aplayer->mediaStatus()!=QtAV::EndOfMedia) autochange = false;
    aplayer->play();

    ui->pushButton_2->setChecked(false);
    ui->pushButton_2->setText("■");
    //下面用来检测是否有列表
    if(ui->listWidget->count()==0&&(theurl.indexOf("albumplay")!=-1||theurl.indexOf("listplay")!=-1))
    {
        qDebug()<<"查找视频列表";
        QRegularExpression reg;
        reg.setPattern("(?<=play/)[^./]+");
        QString code=reg.match(theurl).captured(0);
       // ui->listWidget->clear();
        now_ji=0;

        if(theurl.indexOf("albumplay")!=-1) url="http://www.tudou.com/crp/getAlbumvoInfo.action?charset=utf-8&areaCode=&acode="+code;
        else url="http://www.tudou.com/tvp/getMultiTvcCodeByAreaCode.action?jsoncallback=page_play_model_pListModelList__getTvp&type=2&app=2&areaCode=441900&codes="+code;
        QString refer="http://www.tudou.com";
        reque.setRawHeader(QByteArray("Referer"), refer.toLatin1());
        reque.setRawHeader(QByteArray("User-Agent"), U_A.toLatin1());
        reque.setUrl(QUrl(url));
        reply=manager->get(reque);
        connect(reply,SIGNAL(finished()),this,SLOT(getlist()));
    }
}

void player::gottime(qint64 t,int num)
{
    qDebug()<<timelist.length()<<num;
    if(t==-1)
    {
        qDebug()<<"总时间获取失败";
       // getduration->deleteLater();
        workerThread->quit();
        workerThread->wait();
        getduration=NULL;
        return;
    }
    if(timelist.length()-1>=num) timelist.replace(num,t);
    else timelist.append(t);
    qDebug()<<timelist;
    durationChanged(t);
    if(timelist.length()==ui->video->count()) {
        ui->video->setEnabled(true);
       // getduration->deleteLater();
        workerThread->quit();
        workerThread->wait();
        getduration=NULL;
    }

}

void player::saveset(int i){
    QFile f(cfgpath+"set");
    QTextStream out(&f);
    if(f.open(QIODevice::WriteOnly ))
    {
        out<<i<<endl;
        out.flush();
        f.close();
    }
}
void player::readset(){
    QFile f(cfgpath+"set");
    if(f.open(QIODevice::ReadOnly ))
    {
        QTextStream in(&f);
        int which=-1;
        QString level=in.readLine();
        qDebug()<<level;
        which=level.toInt();
        if(level.isEmpty()) which=1;
        switch(which)
        {
        case 0:format="normal";break;
        case 1:format="high";break;
        case 2:format="super";break;
        case 3:format="real";break;
        default:which=1;format="high";break;//默认高清；
        }
        ui->youxian->setCurrentIndex(which);

        f.close();
    }
}



void player::reset4other()
{
    reset4self();
    ui->listWidget->clear();
    ui->listWidget->hide();
    ui->qingxidu->clear();
    qDebug()<<"reset4other startd";

}

void player::reset4self()
{
    qDebug()<<"reset4self start";
    //-----reset-----
    if(!tmplayer) tmplayer->deleteLater();
    tmplayer=NULL;
//    if(getduration!=NULL)
//    {
//        getduration->deleteLater();
//        getduration=NULL;
//    }
    if(workerThread->isRunning())
    {
        workerThread->quit();
        workerThread->wait();
        getduration=NULL;
    }
    ui->horizontalSlider->setValue(0);
    timelist.clear();
    ui->video->setEnabled(false);
    playrf="";

    qDebug()<<aplayer->mediaStatus();
    getfiletype=0;
    curplay=0;
    ui->video->clear();
    ui->yuyan->setVisible(false);
    showbutton(false);
    //on_pushButton_2_clicked(false);
    //ui->listWidget->hide();
    //ui->widget->hide();
    //----reset------
    qDebug()<<"reset4self started";

}
void player::messfrommp()
{

}
//mplayer -identify -frames 5 -endpos 0 -vo null


void player::on_pushButton_4_clicked(bool checked)
{
    if(checked) aplayer->audio()->setMute(true);
    else  aplayer->audio()->setMute(false);
}


void player::on_pushButton_3_clicked()//直接下一集
{
    if(!online)
    {
        if(curplay<ui->video->count()-1)
        {
            fileplay(curplay+1);
            curplay++;
        }
        return;
    }
    if(now_ji!=-1&&now_ji<ui->listWidget->count()-1)
    {
        ui->listWidget->setCurrentRow(now_ji+1);
        on_listWidget_itemDoubleClicked(ui->listWidget->item(now_ji+1));
    }
}

void player::on_video_doubleClicked(const QModelIndex &index)
{
    autochange = false;
    qDebug()<<aplayer->mediaStatus();
    curplay=index.row();
    qWarning()<<"player::on_video_doubleClicked()"<<index.data(Qt::UserRole);

    if(!online)
    {
        fileplay(curplay);
        return;
    }
    QVariantHash opt;
    if(playrf=="")  opt["referer"]= theurl;
    else  opt["referer"]= playrf;
    opt["user-agent"]=U_A;
    aplayer->setOptionsForFormat(opt);
    aplayer->setFile(index.data(Qt::UserRole).toString());
    aplayer->play();

}

void player::on_pushButton_clicked()//直接上一集
{
    if(!online)
    {
        if(curplay>0)
        {
            fileplay(curplay-1);
            curplay--;
        }
        return;
    }
    if(now_ji!=-1&&now_ji>0)
    {
        ui->listWidget->setCurrentRow(now_ji-1);
        on_listWidget_itemDoubleClicked(ui->listWidget->item(now_ji-1));
    }
}

void player::on_pushButton_2_clicked(bool checked)
{
    qWarning()<<checked;
    if(aplayer->state()!=QtAV::UnknownMediaStatus)
    {
        ui->pushButton_2->setChecked(checked);
        if(checked){
            ui->pushButton_2->setText("▶");
            aplayer->pause();
        }
        else {
            aplayer->pause(false);
            ui->pushButton_2->setText("■");
        }
    }
}
void player::setvideolistcurrentrow(int r)
{
     qWarning()<<"setvideolistcurrentrow";
    ui->video->setCurrentRow(r);
}
void player::positionChanged(qint64 position)
{

  //  qDebug()<<"duration:"<<aplayer->duration();
  //  qDebug()<<"position:"<<position;
    if(aplayer->duration()==-1) return;
    if(aplayer->state()!=QtAV::AVPlayer::PlayingState) return;
    if(online )
    {
        if(!(curplay<timelist.length()))//获取时间失败后，每播放一段就加视频长度到timelist，
        {
            qint64 a=aplayer->duration();
            if(curplay>0)  a += timelist.at(curplay-1);
            timelist.append(a);
            durationChanged(a);
            if(curplay==ui->video->count()) ui->video->setEnabled(true);//如果已经是最后一段就可以将列表恢复点击了
        }

       // if(position!=ui->horizontalSlider->value()) ui->horizontalSlider->setValue(position);
        if(tmplayer==NULL&&curplay<ui->video->count()-1&&aplayer->duration()>10000&&position>0&&aplayer->duration()-position<10000)
        {
            qDebug()<<"preload netx";
            tmplayer = new AVPlayer;
            QVariantHash opt;
            if(playrf=="")  opt["referer"]= theurl;
            else  opt["referer"]= playrf;
            opt["user-agent"]=U_A;
            tmplayer->setOptionsForFormat(opt);
            tmplayer->setFile(ui->video->item(curplay+1)->data(Qt::UserRole).toString());
            tmplayer->load();
        }
        if(curplay>0&&(curplay<timelist.length()||timelist.at(curplay)!=0)) position = timelist.at(curplay-1)+position;
    }
    else durationChanged(aplayer->duration());

    ui->horizontalSlider->setValue(position);
    int h=0,m=0,s=0;
    position/=1000;
    h=position/3600;
    m=position%3600/60;
    s=position%60;
    ui->time_now->setText(QString("%1").arg(h,2,10,QLatin1Char('0'))+":"+QString("%1").arg(m,2,10,QLatin1Char('0'))+":"+QString("%1").arg(s,2,10,QLatin1Char('0')));
   // qDebug()<<"position changed!"<<position;
}




void player::playstoped()
{
    if(!autochange)
    {
        autochange = true;
        return;
    }
    qDebug()<<"player stoped()";
    if(!online)
    {
        if(curplay<ui->video->count()-1)
        {
            qDebug()<<"播放下一个文件";
            fileplay(curplay+1);
            curplay++;
        }
        return;
    }
    else if(curplay!=ui->video->count()-1)
    {
        if(curplay<ui->video->count()-1)
        {
            qDebug()<<"播放下一段";
            ui->video->setCurrentRow(curplay+1);
            curplay++;
        }
        else return;
        if(tmplayer!=NULL)
        {
            aplayer->deleteLater();
            aplayer = tmplayer;
            aplayer->setRenderer(renderer);
            aplayer->audio()->setVolume(ui->horizontalSlider_2->value()/20.0);
            tmplayer=NULL;
            connect(aplayer,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
            connect(aplayer,SIGNAL(stopped()),this,SLOT(playstoped()));
        }
        else
        {
        aplayer->setFile(ui->video->item(curplay)->data(Qt::UserRole).toString());
        }
        aplayer->play();

    }
    else
    {
        if(now_ji!=-1&&now_ji<ui->listWidget->count()-1)
        {
            qDebug()<<"播放下一集";
            ui->listWidget->setCurrentRow(now_ji+1);
            on_listWidget_itemDoubleClicked(ui->listWidget->item(now_ji+1));
            now_ji++;
        }
        else ui->widget_3->show();
    }
}

void player::mediaError(QtAV::AVError error)
{
    if(error!=AVError::NoError)
    {
        qDebug()<<"QMedia出错："<<error;
    }
}

void player::durationChanged(qint64 duration)
{

    qDebug()<<"durationChanged()";
    //这个是获取总时间没事时直接设置调用传来的值
    ui->horizontalSlider->setRange(0, duration);
    int h=0,m=0,s=0;
    duration/=1000;
    h=duration/3600;
    m=duration%3600/60;
    s=duration%60;

    ui->time_len->setText("/ "+QString("%1").arg(h,2,10,QLatin1Char('0'))+":"+QString("%1").arg(m,2,10,QLatin1Char('0'))+":"+QString("%1").arg(s,2,10,QLatin1Char('0')));
    qDebug()<<"duration changed!"<<duration;
}

void player::on_horizontalSlider_sliderMoved(int position)
{
        aplayer->setPosition(position);
}

void player::on_qingxidu_activated(int index)
{
    if(ui->qingxidu->count()==1) return;
    qDebug()<<"player::on_qingxidu_currentIndexChanged"<<index;
    setWindowTitle(ui->qingxidu->itemText(index));
    if(ui->qingxidu->itemData(index).isValid())
    {
        ui->tishi->hide();
        QString cmd = "";
        if(zhuanji==0)
        {
            cmd = ui->qingxidu->itemData(index).toString();
            if(cmd.indexOf("|")!=-1)//不是you-get的参数
            {
                qDebug()<<"已经解析过了！";
                QString text=ui->qingxidu->itemText(index);
                if(text.indexOf("标")!=-1)
                {
                    text="normal";
                }
                else if(text.indexOf("高")!=-1)
                {
                    text="high";
                }
                else if(text.indexOf("超")!=-1)
                {
                    text="super";
                }
                else if(text.indexOf("原")!=-1)
                {
                    text="real";
                }
                if(text==format) return;
                else {
                    format=text;
                    work(encode(theurl));
                }
            }
            else{//you-get的参数
                if(cmd==youget_format) return;
                reset4self();//因为用you-get没有referer返回的，不管了。
                qDebug()<<"调用you-get解析地址";
                QString urls=you_geturls(theurl,cmd);
                ui->qingxidu->setItemData(index,QVariant(urls));
                ui->qingxidu->setEnabled(true);
                play4auto(index);
            }
        }
        else{
            qDebug()<<"专辑已经解析过";
            QListWidgetItem *item;
            QStringList alldata = ui->qingxidu->itemData(index).toStringList();
            ui->listWidget->clear();
            for(int i=0;i<alldata.length();i++)
            {
                item=new QListWidgetItem(ui->listWidget);
                item->setSizeHint(QSize(0,23));
                item->setText("第"+QString().setNum(i+1)+"集");
                item->setData(Qt::UserRole,QVariant(alldata.at(i)));
                ui->listWidget->addItem(item);
            }
        }
    }
    else
    {
        qDebug()<<"准备解析！";
        QString text=ui->qingxidu->itemText(index);
        if(text.indexOf("标")!=-1)
        {
            format="normal";
        }
        else if(text.indexOf("高")!=-1)
        {
            format="high";
        }
        else if(text.indexOf("超")!=-1)
        {
            format="super";
        }
        else if(text.indexOf("原")!=-1)
        {
            format="real";
        }
        work(encode(theurl));
    }

}

void player::on_horizontalSlider_2_valueChanged(int value)
{
    aplayer->audio()->setVolume(value/20.0);

}

void player::on_pushButton_5_clicked(bool checked)
{
    qDebug()<<"player::on_pushButton_5_clicked:"<<checked;
    if(checked)ui->widget->show();
    else ui->widget->hide();
   // ui->widget_3->show();
}

void player::playtimerout()
{
    playtimer->stop();
    on_pushButton_2_clicked(!ui->pushButton_2->isChecked());
}

bool player::eventFilter(QObject *object, QEvent *event)
{
   // qDebug()<<object<<event;
    if(event->type()==QEvent::KeyPress)
    {
        if(ui->widget_2->isHidden())  return QWidget::eventFilter(object,event);
        QKeyEvent *k=static_cast<QKeyEvent*>(event);
       // qDebug()<<k->key();
        if(k->key()==Qt::Key_Space) on_pushButton_2_clicked(!ui->pushButton_2->isChecked());
        else if(k->key()==Qt::Key_Left)
        {
            int pos=ui->horizontalSlider->value()-10000;
            if(pos<0) pos=0;
            toseek(pos);
        }
        else if(k->key()==Qt::Key_Right)
        {
            int pos=ui->horizontalSlider->value()+10000;
             if(pos>ui->horizontalSlider->maximum()) pos = ui->horizontalSlider->maximum();
            toseek(pos);
        }
        else if(k->key()==Qt::Key_Escape)
        {
            if(isFullScreen())
            {
                setfullscreen(false);
            }
        }
        return true;
    }
    else if(object==renderer)
    {
        if(event->type()==QEvent::MouseButtonDblClick){
            if(playtimer->isActive()) playtimer->stop();
            setfullscreen(!isFullScreen());
            return true;

        }
        else if(event->type()==QEvent::MouseButtonPress)
        {
            if(playtimer->isActive()) playtimer->stop();
            else playtimer->start(350);
            return true;

        }
        else return QWidget::eventFilter(object,event);


    }
    else if(object==ui->horizontalSlider)
    {
        //qDebug()<<event;
        if(event->type()==QEvent::HoverMove)
        {
            QHoverEvent *k=static_cast<QHoverEvent*>(event);
            int sx=k->pos().x();
            int wid=ui->horizontalSlider->width();
            float d=sx/(wid*1.0);
            int pos=d*ui->horizontalSlider->maximum();

            int h=0,m=0,s=0;
            pos/=1000;
            h=pos/3600;
            m=pos%3600/60;
            s=pos%60;
            whatstime->setText(QString("%1:%2:%3").arg(h).arg(m).arg(s));
            int y;
            sx+=10;
            y=ui->horizontalSlider->y()+ui->horizontalSlider->height()/2;
            whatstime->move(sx,y);
            whatstime->show();
            whatstime->raise();

        }
        else if(event->type()==QEvent::HoverLeave)
        {
            whatstime->hide();
        }
        else if(event->type()==QEvent::MouseButtonPress){
            QMouseEvent *k=static_cast<QMouseEvent*>(event);
            QPoint p=k->pos();
            int sx=p.x();
            int wid=ui->horizontalSlider->width();
            float d=sx/(wid*1.0);
            int pos=d*ui->horizontalSlider->maximum();
            qDebug()<<d<<"*"<<ui->horizontalSlider->maximum()<<"="<<pos;//不加这句注释进度条不显示！！
            toseek(pos);
            return true;

        }
        else return QWidget::eventFilter(object,event);
    }
    else return QWidget::eventFilter(object,event);
}

void player::toseek(int pos)
{
    ui->horizontalSlider->setValue(pos);
    int i = timelist.length()-1;
    qDebug()<<"总长"<<ui->horizontalSlider->maximum()<<"这里"<<pos;
    if(online)
    {
        for(;i>=0;i--)
        {
            if(pos>=timelist.at(i)) break;//
        }
        if(i!=-1) pos=pos-timelist.at(i);
        qDebug()<<"horizontalslider:第"<<i+1<<"段"<<pos;
        if(curplay!=(i+1))
        {
            curplay=i+1;
            ui->video->setCurrentRow(i+1);
            if(!tmplayer) tmplayer->deleteLater();
            tmplayer=NULL;
            aplayer->setFile(ui->video->item(i+1)->data(Qt::UserRole).toString());
            if(aplayer->mediaStatus()!=QtAV::NoMedia&&aplayer->mediaStatus()!=QtAV::EndOfMedia) autochange = false;
            aplayer->play();
        }
    }
    aplayer->setPosition(pos);
}

void player::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_F1)
    {
         if(!isFullScreen()) ui->widget_3->setVisible(true);
    }
    else return QWidget::keyPressEvent(event);
}

void player::setfullscreen(bool full)
{
    if(full)
    {
        showFullScreen();
        ui->widget->hide();
        ui->pushButton_5->setChecked(false);
        ui->widget_3->hide();
        timer=new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
        timer->start(800);
    }
    else {
        ui->widget_3->show();
        renderer ->setCursor(Qt::ArrowCursor);   //显示鼠标
        showbar(true);
        showNormal();
        timer->stop();
        timer->deleteLater();
        timer=NULL;
    }
}

void player::timeout(){
    QPoint p=QCursor::pos();
    int mx=p.x();
    int my=p.y();
    if(mx==mousex&&my==mousey){
        renderer->setCursor(Qt::BlankCursor);   //隐藏鼠标
        showbar(false);//这个函数起错名字，应该showbar才对！
    }
    else{
        mousex=mx;
        mousey=my;
        renderer->setCursor(Qt::ArrowCursor);   //显示鼠标
        showbar(true);
    }
}

void player::inittimeout()
{
    init=true;
    timer->stop();
    timer->deleteLater();
    timer=NULL;
    ku->init();
}

void player::showbar(bool b)//这个函数起错名字，应该showbar才对！
{
    ui->pushButton->setVisible(b);
    ui->pushButton_2->setVisible(b);
    ui->pushButton_3->setVisible(b);
    ui->pushButton_4->setVisible(b);
    ui->pushButton_5->setVisible(b);
    ui->pushButton_9->setVisible(b);
    ui->pushButton_12->setVisible(b);
    ui->time_len->setVisible(b);
    ui->time_now->setVisible(b);
    ui->qingxidu->setVisible(b);
    ui->horizontalSlider_2->setVisible(b);
    ui->horizontalSlider->setVisible(b);
    if(ui->yuyan->itemData(0).isValid()) ui->yuyan->setVisible(b);
    //这两个widget如果原本是隐藏的就不受全屏操作的影响
    if(ui->pushButton_5->isChecked()) ui->widget->setVisible(b);
    if(ui->widget_3->isVisible()) ui->widget_3->setVisible(b);
}


void player::on_yuyan_activated(int index)
{
    if(ui->yuyan->itemData(index).isValid())
    {

        QString url = ui->yuyan->itemData(index).toString();
        if(url==theurl) return;
        else
        {
            theurl=url;
            reset4self();
            work(encode(theurl));
        }
    }

}

void player::on_pushButton_6_clicked()
{
    if(!init){
        ku->init();
        init=true;
    }
    myshoucang->hide();
    webview->hide();
    ui->widget_2->hide();
    if(aplayer->state()==AVPlayer::PlayingState)
    {
        aplayer->pause();
        on_pushButton_2_clicked(true);
    }
    ku->show();
    download->hide();
    ui->pushButton_11->setChecked(false);
    ui->pushButton_7->setChecked(false);
    ui->pushButton_8->setChecked(false);
    ui->pushButton_6->setChecked(true);
    ui->pushButton_search->setChecked(false);
}

void player::on_pushButton_7_clicked()
{
    myshoucang->hide();
    ku->hide();
    webview->hide();
    ui->widget_2->show();
    download->hide();
    ui->pushButton_11->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(true);
    ui->pushButton_8->setChecked(false);
    ui->pushButton_search->setChecked(false);
}

void player::webplay(QUrl url)
{
    qDebug()<<"player::webplay():"<<url;
    QString str=url.toString();
    if(str.indexOf("url=")!=-1)
    {
        str.remove(QRegularExpression("http.+url="));
       // qDebug()<<"after:"<<str;
        toplay(str);
    }
    else if(str.indexOf("soku.com")==-1)
    {
       // qDebug()<<"right!";
        toplay(str);
    }
    else if(str.indexOf("refer=www.soku.com")!=-1) toplay(str);
    else webview->load(url);
}


void player::on_pushButton_search_clicked()
{

    if(webview->url().isEmpty()) webview->load(QUrl("http://www.soku.com"));

    myshoucang->hide();
    ku->hide();
    if(aplayer->state()==AVPlayer::PlayingState)
    {
        aplayer->pause();
        on_pushButton_2_clicked(true);
    }
    ui->widget_2->hide();
    webview->show();
    download->hide();
    ui->pushButton_11->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
    ui->pushButton_8->setChecked(false);
    ui->pushButton_search->setChecked(true);
}

void player::on_pushButton_9_clicked()
{
    ui->widget_3->setVisible(!ui->widget_3->isVisible());
}

void player::on_pushButton_8_clicked()
{
    ku->hide();
    if(aplayer->state()==AVPlayer::PlayingState)
    {
        aplayer->pause();
        on_pushButton_2_clicked(true);
    }
    ui->widget_2->hide();
    webview->hide();
    myshoucang->show();
    download->hide();
    ui->pushButton_11->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
    ui->pushButton_8->setChecked(true);
    ui->pushButton_search->setChecked(false);

}

void player::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    curplay=0;
    if(zhuanji!=1)//
    {
        now_ji=ui->listWidget->currentRow();
        QString cmd=item->data(Qt::UserRole).toString();
        work(encode(cmd));//encode里有resetself()
    }
    else//网页调用的专辑用下面的解析（用flvcd来解析专辑）；
    {
        now_ji=ui->listWidget->currentRow();
        QString cmd=item->data(Qt::UserRole).toString();
        QStringList partlist=cmd.split("|");
        ui->video->clear();
        QListWidgetItem *item;
        QNetworkRequest *request;
        int n=partlist.length();
        for(int i=0;i<n;i++)
        {
            if(partlist.at(i).isEmpty())continue;
            item=new QListWidgetItem(QString("第"+QString().setNum(i+1)+"段"));
            item->setData(Qt::UserRole,QVariant(partlist.at(i)));
            ui->video->addItem(item);

        }
        QVariantHash opt;
        if(playrf=="")  opt["referer"]= theurl;
        else  opt["referer"]= playrf;
        opt["user-agent"]=U_A;
        aplayer->setOptionsForFormat(opt);
        aplayer->setFile(ui->video->item(0)->data(Qt::UserRole).toString());
        if(aplayer->mediaStatus()!=QtAV::NoMedia&&aplayer->mediaStatus()!=QtAV::EndOfMedia) autochange = false;
        aplayer->play();

        on_pushButton_2_clicked(false);
    }
}


void player::on_pushButton_10_clicked()
{
    openDialog *dialog = new openDialog(this);
    dialog->show();
    connect(dialog,SIGNAL(result(int,QString)),this,SLOT(toopen(int,QString)));

}
void player::toopen(int type, QString str)
{
    switch (type) {
    case 0:
    {
        QStringList list;
        list=str.split("\n");
        openfile(list);
        break;
    }
    case 1:
    {
        toplay(str);
        break;
    }
    default:
    {
        if(online)
        {
            reset4other();
            online=false;
        }
        QListWidgetItem *item=new QListWidgetItem(str);
        item->setData(Qt::UserRole,QVariant(str));
        item->setToolTip(str);
        int i=0;
        for(i=0;i<ui->video->count();i++)
        {
            if(str==ui->video->item(i)->data(Qt::UserRole).toString()) break;
        }
        if(i==ui->video->count()) ui->video->addItem(item);
        on_pushButton_7_clicked();
        curplay=ui->video->count()-list.length();
        //------------[直接播放视频地址的不添加进历史，原因:播放方式不同，暂时不想修改太多]--------------
//        QStringList info;
//        info.append(str);
//        info.append(str);
//        QDateTime time = QDateTime::currentDateTime();
//        info.append(time.toString("yyyy.MM.dd hh:mm:ss"));
//        myshoucang->addlishi(info);

        setWindowTitle(str);
        ui->video->setCurrentRow(curplay);
        aplayer->setFile(str);
        if(aplayer->mediaStatus()!=QtAV::NoMedia&&aplayer->mediaStatus()!=QtAV::EndOfMedia) autochange = false;
        aplayer->play();

        on_pushButton_2_clicked(false);
        break;
    }
    }
}

void player::openfile(QStringList &list)
{
    qDebug()<<"player::openfile";
    if(list.isEmpty()) return;
   // bool auto2play=false;
   // if(ui->video->count()==0) auto2play=true;
    QStringList::Iterator it = list.begin();
    while(it != list.end())
    {

        if(QFileInfo(*it).isFile())
        {
            QString name=(*it).split("/").last();
            if(online)
            {
                reset4other();
                online=false;
            }
            QListWidgetItem *item=new QListWidgetItem(name);
            item->setData(Qt::UserRole,QVariant(*it));
            item->setToolTip(name);
            int i=0;
            for(i=0;i<ui->video->count();i++)
            {
                if(*it==ui->video->item(i)->data(Qt::UserRole).toString())
                {
                    ui->video->takeItem(i);
                    break;
                }
            }
            ui->video->addItem(item);
            qDebug()<<"file:"<<(*it);
            ++it;
        }
    }

    on_pushButton_7_clicked();
    curplay=ui->video->count()-list.length();//下载的视频是分段的
    if(curplay<0&&(!ui->video->count()<0)) curplay=0;
    fileplay(curplay);

}

void player::fileplay(int cur)
{
    QStringList info;
    info.append(ui->video->item(cur)->text());
    info.append(ui->video->item(cur)->data(Qt::UserRole).toString());
    QDateTime time = QDateTime::currentDateTime();
    info.append(time.toString("yyyy.MM.dd hh:mm:ss"));
    myshoucang->addlishi(info);

    setWindowTitle(ui->video->item(cur)->text());
    ui->video->setCurrentRow(cur);

    qDebug()<<cur<<ui->video->item(cur)->data(Qt::UserRole).toString();
   aplayer->play(ui->video->item(cur)->data(Qt::UserRole).toString());
    //on_pushButton_2_clicked(false);//false 即时播放

}

void player::setbase(QString& title)
{
    base=title;
    setWindowTitle(base);
}

void player::adddownload(QString &name, QString &url)
{
    base=name;
    if(url.indexOf("http")==-1)
    {
        qDebug()<<"player::adddownload is zhuanji";
        zjdialog=new zhuanjidl(this);
        zjdialog->setWindowFlags(Qt::Dialog);
        zjdialog->setWindowModality(Qt::WindowModal);
        zjdialog->show();
        zjdialog->getdata(url);
        connect(zjdialog,SIGNAL(finished(QStringList&)),this,SLOT(addzjdownload(QStringList&)));
    }
    else
    {
        QString path=base;
        QString name=path+"\n  ";
        download->addstart(name,url,path);
    }
}

void player::addzjdownload(QStringList& zjlist){
    QStringList list=zjlist;
    for(int i=0;i<list.length();i++)
    {
        QStringList data=list.at(i).split("|");
        QString url=data.at(0);
        QString path=base+"_"+data.at(1);//下载文件夹在dllist里设置
        QString name=path+"\n  ";
        download->addstart(name,url,path);
    }
}

void player::on_pushButton_11_clicked()
{
    ku->hide();
    if(aplayer->state()==AVPlayer::PlayingState)
    {
        aplayer->pause();
        on_pushButton_2_clicked(true);
    }
    ui->widget_2->hide();
    webview->hide();
    myshoucang->hide();
    download->show();
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
    ui->pushButton_8->setChecked(false);
    ui->pushButton_search->setChecked(false);
    ui->pushButton_11->setChecked(true);
}

void player::on_youxian_activated(int index)
{
    QString format="";
    switch(index)
    {
    case 0:format="normal";break;
    case 1:format="high";break;
    case 2:format="super";break;
    case 3:format="real";break;
    default:format="super";break;//默认高清；
    }
    download->setformat(format);
    saveset(index);


}


void player::on_pushButton_12_clicked()
{
    if(ui->video->count()!=0)
    {
        QString title=windowTitle();
        adddownload(title,theurl);
    }
}

QString getsc(QString& a,qlonglong t,QString& b)
{
    //const char* b=num32.toStdString().c_str();

    t=t/(600*1000);
    QString ret="";

    int n=a.length();

    qDebug()<<a<<t<<b<<n;
    for(int i=0;i<n;i++)
    {
        int ai=(int)a.at(i).toLatin1();
        int bi=(int)b.at(i).toLatin1();
        int j=ai^bi^t;
        qDebug()<<ai<<bi<<j;
        j=j%'z';
        QString c="";
        if(j<'0'){
            c=(char)('0'+j%9);
        }
        else if(j>='0'&&j<='9') {
            c=(char)(j);
        }
        else if(j>'9'&&j<'A'){
            c='9';
        }
        else if(j>='A'&&j<='Z'){
            c=(char)(j);
        }
        else if(j>'Z'&&j<'a'){
            c='Z';
        }
        else if(j>='z'&&j<='z'){
            c=(char)(j);
        }
        else c='z';
        //qDebug()<<c<<i;
        ret+=c;
    }
    return ret;
}

void player::passad()
{

}

void player::closeEvent(QCloseEvent *event)
{
    aplayer->pause();

    if(download->isdownloading())
    {
        int result= QMessageBox::question(this, "退出程序", "有任务正在下载！是否继续退出？","是","否",NULL,1);
        if(result==0)
        {
            download->stopall();
            event->accept();
        }
        else event->ignore();
    }
    else event->accept();
}
