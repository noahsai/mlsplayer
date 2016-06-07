#include "huancun.h"
extern QString getsc(QString& a,qlonglong t,QString& b);

huancun::huancun(QObject *parent) :
    QObject(parent)
{
    U_A = "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)";
    manager=new QNetworkAccessManager;
    jar =new QNetworkCookieJar;
    manager->setCookieJar(jar);
    reply=NULL;
    dlmg=new downloadmg;
    connect(dlmg,SIGNAL(stoped()),this,SIGNAL(stoped()));
    connect(dlmg,SIGNAL(finished()),this,SIGNAL(finished()));
    connect(dlmg,SIGNAL(onefinished(QString&)),this,SIGNAL(onefinished(QString&)));
    connect(dlmg,SIGNAL(progress(int)),this,SIGNAL(progress(int)));
    connect(dlmg,SIGNAL(error()),this,SIGNAL(error()));
    num=-1;
    goon=0;//永远是0
    all=0;
    urls="";
    playrf="";
    name="noname";
    format="normal";//默认优先清晰度
    getfiletype=0;
    downloading=false;
    isstop=false;
}
huancun::~huancun(){

}
void huancun::setdling(bool b)
{
    downloading=b;
}

void huancun::setnum(int n)
{
    num=n;
}

void huancun::setformat(QString& fm)
{
    format=fm;
}

void huancun::seturlpath(QString& url, QString& path)
{
    reply=NULL;
    isstop=false;
    getfiletype=0;
    playurl=url;//实际huancun的playurl是当rf用的，不用管cfg文件里的playurl是否正确
                //有cfg时，playurl当refer，没有cfg时，dllist会设置playurl，所以就算downloadmg里面将playurl设置成refer也没事！
    dirpath=path;
}

bool huancun::readcfg(){
    qDebug()<<"huancun::readcfg";
    QFile file;
    QString cfg=dirpath+"/cfg";
    file.setFileName(cfg);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in;
        in.setDevice(&file);
        in>>all>>name>>goon>>playurl>>urls>>playrf>>U_A;
        return true;
    }
    else {
        QString cmd=dirpath;
        cmd="rm -rf \""+cmd+"\"";
        system(cmd.toStdString().c_str());
        qDebug()<<"huancun:open cfg file error!";
        //QDir().mkpath(path);
        return false;
    }
    file.close();
}

int huancun::getnum(){
    return num;
}

QString& huancun::geturl(){
    return playurl;
}
void huancun::start(){
    isstop=false;
    if(readcfg()){
        if(playrf=="")
            dlmg->seturlpath(name,urls,dirpath,playurl,U_A);
        else
            dlmg->seturlpath(name,urls,dirpath,playrf,U_A);
        dlmg->start(goon);
    }
    else {
        fenxi();
    }
    downloading=true;
}

void huancun::stop(){
    qDebug()<<"huancun::stop()";
    if(!isstop)
    {
        isstop=true;

        if(reply!=NULL)
        {
            qDebug()<<"停止获取分段文件列表";
            reply->abort();

        }
        dlmg->stop();
        downloading=false;
        num=-1;
    }
}


void huancun::getinfo(int i){
    QString result=pro->readAllStandardOutput();
    pro->kill();
    pro->deleteLater();
    qDebug()<<result;
    if(result.indexOf("title",0,Qt::CaseInsensitive)!=-1)
    {
        QStringList cmds;
        QStringList formats;
        if(name=="noname")
        {
            reg.setPattern("(?<=[Tt][iI][tT][lL][eE]:).+");
            name=reg.match(result).captured(0);
            name.remove(" ");
            qDebug()<<"huancun:name"<<name;
        }
        reg.setPattern(" --.+ ");
        matchs=reg.globalMatch(result);
        if(!matchs.hasNext())//证明只有一个清晰度
        {
            QString f="";
            you_geturls(playurl,f);
            return ;
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
                //qDebug()<<t;
                if(t.indexOf("标清")!=-1||t.indexOf("360")!=-1) formats.append("normal");
                else if(t.indexOf("高清")!=-1||t.indexOf("480")!=-1) formats.append("high");
                else if(t.indexOf("超清")!=-1||t.indexOf("720")!=-1) formats.append("super");
                else if(t.indexOf("原画")!=-1||t.indexOf("1080")!=-1) formats.append("real");
            }
            int which=0;
            qDebug()<<formats<<endl<<format;
            which=formats.indexOf(format);
            if(which == -1) {
                emit error();
                return;
            }
            QString fm=cmds.at(which);
            you_geturls(playurl,fm);
        }
    }
}

bool huancun::you_get(QString url)
{
    pro = new QProcess;
    QString yougetpath=QApplication::applicationDirPath()+"/you-get-develop/you-get";
    QString cmd="python3 "+yougetpath+" -i "+url;
    pro->start(cmd);
    connect(pro,SIGNAL(finished(int)),this,SLOT(getinfo(int)));
    connect(pro,SIGNAL(error(QProcess::ProcessError)),this,SLOT(error()));

    if(!pro->waitForStarted())    return false;

}

void huancun::geturls(int i)
{
    QString result=pro->readAllStandardOutput();
    pro->kill();
    pro->deleteLater();
//            qDebug()<<result;
    if(result.indexOf("urls",0,Qt::CaseInsensitive)!=-1)
    {
        reg.setPattern("[uU][rR][lL][sS]:[\\s\\S]+");
        QString data=reg.match(result).captured(0);
        reg.setPattern("http.+");
        qDebug()<<"huancun:"<<data;
        matchs=reg.globalMatch(data);
        QString urls;
        while(matchs.hasNext())
        {
            urls+=matchs.next().captured(0)+"|";
        }
        qDebug()<<"huancun:urls found!";
        todownload(urls);
        return ;
    }
    qDebug()<<"huancun:urls not found!";
\

}

void huancun::you_geturls(QString& url,QString& format)
{
    U_A = "Python-urllib/3.5";
    pro=new QProcess;
    QString yougetpath=QApplication::applicationDirPath()+"/you-get-develop/you-get";
    QString cmd="python3 "+yougetpath+" -u "+format+url;
    qDebug()<<cmd;
    pro->start(cmd);
    connect(pro,SIGNAL(finished(int)),this,SLOT(geturls(int)));
    connect(pro,SIGNAL(error(QProcess::ProcessError)),this,SLOT(error()));

    if(!pro->waitForStarted())
    {
        qDebug()<<"huancun:you-get waitfinished error!";
        return ;
    }
}


void huancun::fenxi(){
   work(encode(playurl));
}

QString huancun::encode(const QString& url)
{
    QString a;
    QUrl u(url);
    a=u.toEncoded().toPercentEncoding();
    a="http://www.flvcd.com/parse.php?kw="+a+"&format="+format+"&go=1";//加上清晰度
    //qDebug()<<a;
    return a;
}
void huancun::work(const QString &edurl)
{
    U_A = "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)";
    qDebug()<<"huancun::work";
    QUrl url;
    url.setUrl(edurl);
    QString refer="http://www.flvcd.com";
    reque.setRawHeader(QByteArray("Referer"), refer.toLatin1());
    //qDebug()<<"work";
    reque.setRawHeader(QByteArray("User-Agent"), U_A.toLatin1());
    reque.setUrl(url);
    //qDebug()<<"work";
    reply=manager->get(reque);

    connect(reply,SIGNAL(finished()),this,SLOT(replyfinished()));
    //qDebug()<<"work";

}

void huancun::replyfinished()
{
    if(isstop){
        reply->deleteLater();
        return;
    }
    qDebug()<<"huancun:replyfin:"<<reply->url();
    QByteArray bytes =reply->readAll();
  //  qDebug()<<bytes;
    reply->deleteLater();
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QString html = codec->toUnicode(bytes);
    reply=NULL;
    switch(getfiletype)
    {
    case 0:fenxi1(html);break;
    case 1:getsite(html);break;
    case 2:fenxi3(html);break;
    default:qDebug()<<"value of getfiletype is error!";break;
    }

}


bool huancun::fenxi1(QString& html)
{
    bool has_set_level=true;
    qDebug()<<"分析开始";
    //没有"当前解析"就是解析出错.
    if(playurl.indexOf("iqiyi")!=-1)
    {
        if(you_get(playurl)) return true;
        else return false;
    }
    if(html.indexOf("当前解析")==-1) {     //flvcd没有解析出来时

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

            jar= manager->cookieJar();
            jar->insertCookie(cs);
            jar->insertCookie(go);
            manager->setCookieJar(jar);
            QString re=reque.url().toString();
            reque.setRawHeader(QByteArray("Referer"), re.toLatin1());
            reply=manager->get(reque);

            connect(reply,SIGNAL(finished()),this,SLOT(replyfinished()));
            return true;
        }
        //尝试you-get解析
        if(you_get(playurl)) return true;
        else
        {
            qDebug()<<"解析失败，无法下载";
            return false;
        }
    }
    reg.setPattern("(?<=document.title = \").+?(?=\")");

    match=reg.match(html);
    if(match.hasMatch())
    {
        qDebug()<<"has name match";
        name = match.captured();

        if(html.indexOf("无法下载")!=-1)
        {
            if(you_get(playurl)) return true;
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
                work(encode(playurl));
                return false;
                }//有清晰度选择才work();
            else qDebug()<<"huancun:format not found!";
        }
    }
    else qDebug()<<"hasn't name match";

    if(html.indexOf("只能用硕鼠")!=-1)
    {
        qDebug()<<"只能用硕鼠";
        getfiletype=1;
        fenxi2(html);
        return false;
    }
    else {
        reg.setPattern("(?<=name=\"inf\" value=\").+?(?=\")");
        match=reg.match(html);
      //  if(!matchs.hasNext())return false;
        if(match.hasMatch())
        {
            QString t=match.captured(0);
            todownload(t);
        }
        else {
            getfiletype=1;
            fenxi2(html);
            //setWindowTitle("获取失败！");
            return false;
        }
    }
}


bool huancun::fenxi2(QString& html)
{
    reg.setPattern("id=\"parse_form\"[\\s\\S]+name=\"passport\"");
    QStringList form;
    int zhuanji;
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
        qDebug()<<form;
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
                match=reg.match(html);
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
            reque.setRawHeader(QByteArray("Referer"), refer.toLatin1());
            reque.setUrl(url);
            reply=manager->get(reque);
            connect(reply,SIGNAL(finished()),this,SLOT(replyfinished()));
        }
    }
    else return false;
    return true;

}

void huancun::getsite(QString& html)
{
    delete reply;
    getfiletype=2;
    reg.setPattern("(?<=id=).+(?=\")");
    match=reg.match(html);
    QString url="http://www.flvcd.com/diy/diy00";
    url=url+match.captured(0)+".htm";
    qDebug()<<"huancun:getsite";
    reque.setUrl(QUrl(url));
    reply=manager->get(reque);
    connect(reply,SIGNAL(finished()),this,SLOT(replyfinished()));
}

void huancun::fenxi3(QString& html)
{
    QString t;
    QTextStream in(&html);
    qDebug()<<"fenxi3";
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
        qDebug()<<"huancun:cs";
        getufromc *getu=new getufromc(cs);
//        QEventLoop *loop= new QEventLoop;
        connect(getu,SIGNAL(finished(QString&)),this,SLOT(gotcstodownload(QString&)));
//        connect(getu,SIGNAL(finished()),loop,SLOT(quit()));
        getu->start();
//        loop->exec();
//        t=getu->getresult();
//        qDebug()<<"cs got"<<isstop<<t;
    }

    //todownload(t);


}

void huancun::gotcstodownload(QString& t)
{
    todownload(t);

}
void huancun::todownload(QString& cmd)
{

    reply=NULL;
    qDebug()<<"------------\nurls:"<<cmd;
    urls=cmd;
    save();

    if(!isstop)
    {
        qDebug()<<"开始下载";
        if(playrf=="")
            dlmg->seturlpath(name,urls,dirpath,playurl,U_A);
        else
            dlmg->seturlpath(name,urls,dirpath,playrf,U_A);
        dlmg->start(goon);
    }
}
void  huancun::save()
{
    QFile file;
    QString cfg=dirpath+"/cfg";
    QDataStream out;
    out.setDevice(&file);
    file.setFileName(cfg);
    if(file.open(QIODevice::WriteOnly))
    {
        out<<(int)all;
        out<<name;
        out<<(int)0;
        out<<playurl;
        out<<urls;//这个urls已经是包含了|分隔符的QString；
        out<<playrf;
    }
    else {
        qDebug()<<"huancun:cfg file open error!";
    }
    file.close();
    emit stoped();
}
bool huancun::isdownloading()
{
    return downloading;
}
