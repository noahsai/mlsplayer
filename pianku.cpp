#include "pianku.h"
#include "ui_pianku.h"
#define U_A "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)"
QString ku="首页|全部|原创|电视|动漫|电影|综艺|音乐|纪实|搞笑|游戏|娱乐|资讯|汽车|科技|体育|时尚|生活|健康|教育|曲艺|母婴|旅游|宗教";
QString tudou=",,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=0&areaCode=&tags=%5B%5D&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=1&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/s3portal/service/pianku/data.action?pageSize=90&app=mainsitepc&deviceType=1&tags=&tagType=3&firstTagId=3&areaCode=&initials=&hotSingerId=&pageNo=1&sortDesc=quality,全部:|大陆:42|香港:43|美国:44|台湾:45|韩国:46|泰国:47|新加坡:48|日本:1494|法国:49,全部:|神话:52|古装:53|悬疑:54|历史:55|偶像:56|家庭:57|军事:59|警匪:60|喜剧:61|传记:63|生活:64|剧情:65|武侠:66|科幻:67|儿童:68|农村:69|都市:71|搞笑:72|言情:73|时装:74|优酷出品:131,全部:|跟播:323|完结:324\
\nhttp://www.tudou.com/s3portal/service/pianku/data.action?pageSize=90&app=mainsitepc&deviceType=1&tags=&tagType=3&firstTagId=4&areaCode=&initials=&hotSingerId=&pageNo=1&sortDesc=quality,全部:|日本:120|大陆:121|美国:122|韩国:123|英国:124|法国:125|台湾:126|香港:127|其他:128,全部:|热血:132|科幻:133|神魔:134|机战:135|竞技:136|格斗:137|恋爱:138|励志:139|社会:140|搞笑:141|冒险:142|刑侦:143|历史:144|神话:145|宠物:146|悬疑:147|幻想:148|友情:149|真人:150|运动:151|战争:152|美少女:153|LOLI:154|校园:155|青春:156|推理:157|吸血鬼:158|忍者:159|魔法:160|后宫:161|穿越:162|同人:163|剧情:164|教育:165|亲子:166|益智:167|童话:168|其他:169,全部:|更新中:350|全集:351\
\nhttp://www.tudou.com/s3portal/service/pianku/data.action?pageSize=90&app=mainsitepc&deviceType=1&tags=&tagType=3&firstTagId=5&areaCode=&initials=&hotSingerId=&pageNo=1&sortDesc=quality,全部:|大陆:77|香港:78|韩国:79|美国:80|法国:81|意大利:82|英国:83|台湾:84|泰国:85|加拿大:86|德国:87|日本:88|俄罗斯:89|印度:90,全部:|剧情:91|文艺:92|喜剧:93|爱情:94|犯罪:95|动画:96|战争:97|恐怖:98|惊悚:99|动作:100|冒险:101|悬疑:102|历史:103|搞笑:104|都市:105|警匪:109|传记:106|儿童:107|武侠:108|科幻:110|纪录片:111|西部:112|戏曲:113|歌舞:114|奇幻:115|短片:116|运动:117|土豆制造:119|优酷出品:1009|其他:130,全部:|即将上映:352|正片:353\
\nhttp://www.tudou.com/s3portal/service/pianku/data.action?pageSize=90&app=mainsitepc&deviceType=1&tags=&tagType=3&firstTagId=6&areaCode=&initials=&hotSingerId=&pageNo=1&sortDesc=quality,全部:|大陆:170|台湾:171|香港:172|韩国:173|美国:174|日本:175|澳大利亚:176|其他:177,全部:|选秀:178|音乐:179|脱口秀:180|舞蹈:181|访谈:182|生活:183|娱乐:184|搞笑:185|益智:186|演唱会:187|真人秀:188|情感:189|魔术:190|模仿秀:191|情景剧:192|少儿:193|美食:194|旅游:195|纪实:196|晚会:197|理财:198|曲艺:199|体育:200|游戏:201|时尚:202|汽车:203|母婴:204|土豆制造:207|优酷出品:1013|其他:208,全部:|更新中:420|全集:421\
\nhttp://www.tudou.com/list/albumData.action?tagType=3&firstTagId=12&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,全部:|大陆:926|台湾:927|香港:928|韩国:929|日本:930|泰国:1668|美国:931|法国:933|印度:1700|英国:1701|意大利:1702|德国:1703|俄罗斯:1704|爱尔兰:1705|西班牙:1706|以色列:1707|新加坡:1708|其他:1668,全部:|男歌手:1812|女歌手:1813|乐队组合:1814|群星:1816,全部:|普通话:1819|粤语:1820|台语:1821|英语:1824|日语:1822|韩语:1823|法语:1825德语:1846|意大利语:1827|西班牙语:1828|俄语:1829|其他:1830\
\nhttp://www.tudou.com/s3portal/service/pianku/data.action?pageSize=90&app=mainsitepc&deviceType=1&tags=&tagType=3&firstTagId=8&areaCode=&initials=&hotSingerId=&pageNo=1&sortDesc=quality,全部:|大陆:1751|香港:1752|台湾:1753|美国:1754|英国:1755|日本:1756|法国:1757|加拿大:1758|澳大利亚:1759|其他:1763,全部:|人物:209|军事:210|历史:211|宇宙:212|自然:213|古迹:214|探险:215|科技:216|文化:217|社会:218|刑侦:219|财经:220|旅游:221|名人名嘴:1709|公开课:1710,全部:|BBC:1760|北京电视台:396|美国国家地理:397|美国历史频道:398|上海纪实频道:399|五星传奇:1761|IMG:1762\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=7&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=15&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=13&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=14&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=9&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=10&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=2&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=11&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=16&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=17&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/s3portal/service/pianku/data.action?pageSize=90&app=mainsitepc&deviceType=1&tags=&tagType=3&firstTagId=18&areaCode=&initials=&hotSingerId=&pageNo=1&sortDesc=quality,全部:|公开课:442|名人名嘴:443|艺术:445|文化:444|伦理社会:446|理工:447|历史:448|心理学:449|经济:450|政治:451|管理学:452|外语:453|法律:454|计算机:455|哲学:456|职业培训:457|职业教育:459|家庭教育:458,全部:|TED:1774|泰学传媒:1775|耶鲁大学:1776|哈佛大学:1777|斯坦福大学:1778|麻省理工学院:1779|牛津大学:1780|加州大学:1781|剑桥大学:1783|Udacity:1784|中学网上课堂:1785,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=19&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=20&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=21&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,\
\nhttp://www.tudou.com/list/itemData.action?tagType=1&firstTagId=23&areaCode=&tags=&initials=&hotSingerId=&page=1&sort=2&key=,,,";

pianku::pianku(QWidget *parent) :
    QWidget(parent),
 ui(new Ui::pianku)
{
    ui->setupUi(this);
    ui->leibie->installEventFilter(this);
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();//这两个按钮不用了
    ui->leibie->clear();
    qDebug()<<"pianku: start";
    QStringList list=ku.split("|");
    ui->leibie->addItems(list);
 //   ui->listWidget_2->hide();
//    ui->listWidget_2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    setLayout(ui->horizontalLayout);
    manager=new QNetworkAccessManager;
    QTextStream fenlei;
    fenlei.setString(&tudou);
    QString tmp;
    pos=0;
    page=1;
    imgstart=0;
    imgend=0;
    reply=NULL;
    all=false;
    tostop=false;
//    bar=ui->listWidget_2->verticalScrollBar();
//    bar->setMaximumWidth(12);
//    connect(bar,SIGNAL(valueChanged(int)),this,SLOT(loadimgs(int)));
 //   qDebug()<<"pianku start2";
    shouyeweb = new QWebView(this);
    mywebpage *page = new mywebpage;
    shouyeweb->installEventFilter(this);
    shouyeweb->setPage(page);
    shouyeweb->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(shouyeweb,SIGNAL(linkClicked(QUrl)),this,SLOT(webplay(QUrl)));
    ui->horizontalLayout_3->addWidget(shouyeweb);

    int i=0;
    while(!fenlei.atEnd())
    {
        tmp=fenlei.readLine();
       // qDebug()<<tmp;
        ui->leibie->item(i)->setData(Qt::UserRole,QVariant(tmp));
        ui->leibie->item(i)->setTextAlignment(Qt::AlignHCenter);
        i++;

    }

    mutex=new QMutex();
    qDebug()<<"pianku:pianku started";
  //  on_leibie_itemClicked(ui->leibie->item(0));
}

pianku::~pianku()
{
    delete ui;
}

void pianku::init()
{
    qDebug()<<"pianku init";
    on_leibie_itemClicked(ui->leibie->item(0));
    ui->leibie->setCurrentRow(0);
    qDebug()<<"pianku inited";

}

void pianku::on_leibie_itemClicked(QListWidgetItem *item)
{
    if(reply)
    {
        if(reply->isRunning())
        {
            tostop=true;
            reply->abort();
        }
    }
    //ui->leibie->setEnabled(false);
    page=1;
    ui->comboBox_4->setCurrentIndex(1);
    ui->comboBox_4->show();//后面判断是首页时会将他隐藏，所以这里show();
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox->hide();
    ui->comboBox_2->hide();
    ui->comboBox_3->hide();
    ui->pushButton_3->hide();

    ui->pushButton_4->setEnabled(false);
    QStringList list,tmp;
    list=item->data(Qt::UserRole).toString().split(",");
    qDebug()<<list;
    if(!list.at(1).isEmpty())
    {
        tmp=list.at(1).split("|");
        for(int i=0;i<tmp.length();i++)
        {
            QStringList t=tmp.at(i).split(":");
            ui->comboBox->addItem(t.at(0));
            ui->comboBox->setItemData(i,QVariant(t.at(1)));
        }
        ui->comboBox->show();
        ui->pushButton_3->show();
    }    qDebug()<<"pianku:liebiao clicked";

    if(!list.at(2).isEmpty())
    {
        tmp=list.at(2).split("|");
        for(int i=0;i<tmp.length();i++)
        {
            QStringList t=tmp.at(i).split(":");
            ui->comboBox_2->addItem(t.at(0));
            ui->comboBox_2->setItemData(i,QVariant(t.at(1)));
        }
        ui->comboBox_2->show();

    }
    if(!list.at(3).isEmpty())
    {
        tmp=list.at(3).split("|");
        for(int i=0;i<tmp.length();i++)
        {
            QStringList t=tmp.at(i).split(":");
            ui->comboBox_3->addItem(t.at(0));
            ui->comboBox_3->setItemData(i,QVariant(t.at(1)));
        }
        ui->comboBox_3->show();

    }
    if(list.at(0).isEmpty())
    {
        ui->comboBox_4->hide();
        reque.setRawHeader(QByteArray("User-Agent"), U_A);
        reque.setUrl(QUrl("http://www.tudou.com"));

        reply=manager->get(reque);
        connect(reply,SIGNAL(finished()),this,SLOT(getshouye()));
        return;
    }
    on_comboBox_4_activated(ui->comboBox_4->currentIndex());//这里会获取数据生成列表。
    qDebug()<<"pianku:liebiao clicked";

}

void pianku::getshouye()
{
    qDebug()<<"pianku:getshouye";
    ui->pushButton_2->hide();
    if(tostop)
    {
        reply->deleteLater();
        tostop=false;
        reply=NULL;
        return;
    }
    ui->pushButton_4->setEnabled(false);
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    for(int i=0;i<imgs.length();i++)
    {
        delete imgs.at(i);
    }
    imgs.clear();
//    ui->listWidget_2->clear();
//    ui->listWidget_2->setViewMode(QListView::IconMode);
    qDebug()<<"getshouye";

    QString list=reply->readAll();
    if(list.isEmpty()) return;
    qDebug()<<"getshouye";
    reply->deleteLater();
    reply=NULL;

    QRegularExpression reg("(?<=<div class=\"mod fix\")[\\S\\s]+?<div class=\"mod fix\"");
    QRegularExpressionMatchIterator matchs=reg.globalMatch(list);
    int m=0;
    QString html="<html>\
<head>\
<meta charset=\"utf-8\" /> \
<link type=\"text/css\" href=\"tudou.css\" rel=\"stylesheet\">\
<script type=\"text/javascript\" src=\"tudou.js\"></script>\
</head>\
<body oncontextmenu=self.event.returnValue=false>\
<div class=\"main\">";
    if(!matchs.hasNext()) return;
    while(matchs.hasNext())
    {
        int sp=0;
        html+="<div class=\"div\">";
        QRegularExpressionMatch match=matchs.next();
        QString part = match.captured(0);
        if(part.indexOf("买买买买买")!=-1) continue;
        else if(part.indexOf(QRegularExpression("<h2>.*今日热门.*</h2>"))!=-1) html+="<h2>今日热门</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*电视剧.*</h2>"))!=-1) html+="<h2>电视剧</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*综艺.*</h2>"))!=-1) html+="<h2>综艺</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*动漫.*</h2>"))!=-1) html+="<h2>动漫</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*电影.*</h2>"))!=-1){
            sp=1;
            html+="<h2>电影</h2>";
        }
        else if(part.indexOf(QRegularExpression("<h2>.*你而.*</h2>"))!=-1) html+="<h2>音乐</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*娱乐.*</h2>"))!=-1) html+="<h2>娱乐</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*搞笑.*</h2>"))!=-1) html+="<h2>搞笑</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*时尚.*</h2>"))!=-1) html+="<h2>时尚</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*乐活.*</h2>"))!=-1) html+="<h2>乐活</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*纪实.*</h2>"))!=-1) html+="<h2>纪实</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*精选.*</h2>"))!=-1) html+="<h2>精选</h2>";
        else if(part.indexOf(QRegularExpression("<h2>.*精彩栏目.*</h2>"))!=-1){
            sp=2;
            html+="<h2>精彩栏目</h2>";
        }
        reg.setPattern("<div class=\"pic\">.*?<a.+?title=.*?</div>");
        QRegularExpressionMatchIterator data=reg.globalMatch(part);

        while(data.hasNext())
        {
            QString d=data.next().captured(0);
            //qDebug()<<d;
            if(d.indexOf("\"ico\">会员免费</i>")!=-1) continue;

            html+="<li class=\"sp"+QString().number(sp)+"\">";

//            QListWidgetItem *listitem;
//            listitem=new QListWidgetItem;

//                listitem->setSizeHint(QSize(208,210));
//                ui->listWidget_2->setIconSize(QSize(200,160));
//                listitem->setIcon(QIcon(":/2300.png"));


            reg.setPattern("(?<=href=\").+?(?=\")");
            QString da=reg.match(d).captured(0);
            html+="<a class=\"down\" href=\"\" title=\"缓存\">▼</a>";
            html+="<a class=\"keep\" href=\"\" title=\"收藏\">♥</a>";
            html+="<a class=\"url\" href=\""+da+"\">";

//            listitem->setToolTip(da);
//            listitem->setData(Qt::UserRole,QVariant(da));

            reg.setPattern("http[^\"]+?img\\.com[^\"]+?(?=\")");
            da=reg.match(d).captured(0);
            html+="<img class=\"lazy\" alt=\""+da+"\"/></a>";

//            img=new imgload(m,da);
//            connect(img,SIGNAL(loaded(int)),this,SLOT(seticon(int)));
//            imgs.append(img);


            reg.setPattern("(?<=title=\").+?(?=\")");
            da=reg.match(d).captured(0);
            html+="<p>"+da+"</p></li>";

//            da.remove(QRegularExpression("&.+?;"));
//            listitem->setText(da);


         //   ui->listWidget_2->addItem(listitem);
            m++;
        }
        html+="</div>";
    }
    html+="</div></body></html>";
    QString dir=QApplication::applicationDirPath();
    shouyeweb->setHtml(html,QUrl().fromLocalFile(dir+"/text.html"));
//    ui->listWidget_2->hide();

    //shouyeweb->load(url);
    //qDebug()<<"getshouye"<<endl<<html;
//    imgstart=0;
//    int beishu=1;//一开始就加载两页
//    imgend=beishu*(ui->listWidget_2->width()*ui->listWidget_2->height()/20000);
//    for(;imgstart<imgend&&imgstart<imgs.length();imgstart++)
//    {
//        imgs.at(imgstart)->start();
//    }
    QFile f(dir+"/text.html");
    f.open(QIODevice::WriteOnly);
    QTextStream out(&f);
    out<<html;
    out.flush();
    f.close();
    pos=0;
//    bar->setValue(0);
//    qDebug()<<imgs.length();
    all=true;

}

void pianku::getlist()
{
    if(tostop)
    {
        reply->deleteLater();
        reply=NULL;
        tostop=false;
        return;
    }
    all=false;
    ui->pushButton_4->setEnabled(false);
    //ui->leibie->setEnabled(true);
    ui->pushButton->hide();
    ui->pushButton_2->hide();
//    for(int i=0;i<imgs.length();i++)
//    {
//        delete imgs.at(i);
//    }
//    imgs.clear();
 //   ui->listWidget_2->clear();
 //   ui->listWidget_2->setViewMode(QListView::IconMode);
    QString list=reply->readAll();
    //qDebug()<<"gotitemsdata:"<<list;
    reply->deleteLater();
    reply=NULL;

    type=false;
    if(list.indexOf("error")==-1) type=true;
    if(!type)
    {
        list.remove(QRegularExpression("{\"error\":0,.*\"data\":\\[{"));
        list.remove("}]}");
 //       ui->listWidget_2->setIconSize(QSize(208,116));
    }
    else {
 //       ui->listWidget_2->setIconSize(QSize(200,300));
        QRegularExpression reg("\\[[\\s\\S]+\\]");
        QRegularExpressionMatch match=reg.match(list);
        list=match.captured(0);
        reg.setPattern("\"actors\":\\[.*?\\],");
        list.remove(reg);
    }
    QString html="<html>\
<head>\
<meta charset=\"utf-8\" /> \
<link type=\"text/css\" href=\"tudou.css\" rel=\"stylesheet\">\
<script type=\"text/javascript\" src=\"tudou.js\"></script>\
</head>\
<body>\
<div class=\"main\">";

    QString href,text,pic;
   // qDebug()<<list;
    QStringList items;
  //  QListWidgetItem *listitem;

    items=list.split("},{");
    for(int i=0,m=0;i<items.length();i++)
    {
//        listitem=new QListWidgetItem;
//        if(!type)
//        {
//            listitem->setSizeHint(QSize(208,166));
//            ui->listWidget_2->setIconSize(QSize(208,116));
//        }
//        else {
//            listitem->setSizeHint(QSize(208,350));
//            ui->listWidget_2->setIconSize(QSize(200,300));
//        }
        QStringList att;
        att=items.at(i).split(",");
        int sign=0;
        QString title="";
        bool pay=false;
        for(int j=0;j<att.length();j++)
        {
            QString test=att.at(j);
            if(test.indexOf("code")!=-1)
            {
                test.remove("\"code\":");
                test.remove("\"");
                test="http://www.tudou.com/programs/view/"+test;
//                listitem->setData(Qt::UserRole,QVariant(test));
                href=test;
                sign++;
            }
            else if(test.indexOf("playUrl")!=-1)
            {
                QRegularExpression reg("(?<=/)[^/]+(?=\\.html)");
                QRegularExpressionMatch match=reg.match(test);
                test=match.captured(0);
//                listitem->setData(Qt::UserRole,QVariant(test));
                href=test;
                //listitem->setToolTip(test);
                //qDebug()<<m<<test;
                sign++;
            }
            else if(test.indexOf("itemUrl")!=-1)
            {
                test.remove("\"itemUrl\":");
                test.remove("\"");
//                listitem->setData(Qt::UserRole,QVariant(test));
                href=test;
                //qDebug()<<m<<test;
                sign++;
            }
            else if((test.indexOf("title")!=-1)||(test.indexOf("itemTitle")!=-1))
            {
                if(test.indexOf("title")!=-1)
                {
                    test.remove("\"title\":");
                    test.remove("\"");
                }
                else if(test.indexOf("itemTitle")!=-1)
                {
                    test.remove("\"itemTitle\":");
                    test.remove("\"");
                }
                sign++;
                if(title!="") {
                    title=title+"\n";
                    sign--;
                }
                title=title+test;
//                listitem->setText(title);
                text=title;
            }
            else if(test.indexOf("\"picUrl\"")!=-1)
            {
                test.remove("\"picUrl\":");
                test.remove("\"");
//                listitem->setIcon(QIcon(":/2116.png"));
                sign++;
//                img=new imgload(m,test);
                m++;
                //qDebug()<<m<<test;
//                connect(img,SIGNAL(loaded(int)),this,SLOT(seticon(int)));
                //img->start();
//                imgs.append(img);
                pic=test;
            }
            else if(test.indexOf("picUrl_200x300")!=-1)
            {
                test.remove("\"picUrl_200x300\":");
                test.remove("\"");
//                listitem->setIcon(QIcon(":/2300.png"));
                sign++;
//                img=new imgload(m,test);
                m++;
              //  qDebug()<<test;
//                connect(img,SIGNAL(loaded(int)),this,SLOT(seticon(int)));
                //img->start();
//                imgs.append(img);
                pic=test;

            }
            else if(test.indexOf("itemPicUrl")!=-1)
            {
                test.remove("\"itemPicUrl\":");
                test.remove("\"");
//                listitem->setIcon(QIcon(":/2300.png"));
                sign++;
//                img=new imgload(m,test);
                m++;
//                connect(img,SIGNAL(loaded(int)),this,SLOT(seticon(int)));
                //img->start();
//                imgs.append(img);
                pic=test;

            }
            else if(test.indexOf("payInfo")!=-1)
            {
                if(test.indexOf("null")==-1)
                {
                    pay=true;
                    m--;
                }
                else pay=false;
            }
        }

        if(!pay)
        {
//            ui->listWidget_2->addItem(listitem);
            if(!type)  html+="<li class=\"sp0\">";
            else html+="<li class=\"sp1\">";
            html+="<a class=\"down\" href=\"\" title=\"缓存\">▼</a>";
            html+="<a class=\"keep\" href=\"\" title=\"收藏\">♥</a>";
            html+="<a class=\"url\" href=\""+href+"\">";
            html+="<img class=\"lazy\" alt=\""+pic+"\"/></a>";
            html+="<p>"+text+"</p></li>";

        }
        else
        {
//            delete listitem;
//            imgs.removeLast();
        }
    }
    html+="</div></body></html>";
    QString dir=QApplication::applicationDirPath();
    shouyeweb->setHtml(html,QUrl().fromLocalFile(dir+"/text.html"));
    imgstart=0;
//    int beishu=1;//一开始就加载两页
//    imgend=beishu*(ui->listWidget_2->width()*ui->listWidget_2->height()/20000);
//    for(;imgstart<imgend&&imgstart<imgs.length();imgstart++)
//    {
//        imgs.at(imgstart)->start();
//    }
//    loadimgs(0);
    pos=0;
    if(items.at(0).isEmpty()){
        ui->pushButton_2->show();//如果是结尾直接显示上一页按钮。
        all=true;
    }
//    bar->setValue(0);
//    qDebug()<<imgs.length();

}

bool pianku::eventFilter(QObject *obj, QEvent *event)//必须逻辑正确，各分支都需要return！
{
    if(obj==shouyeweb&&event->type()==QEvent::Wheel)
    {
        if(shouyeweb->page()->mainFrame()->scrollBarValue(Qt::Vertical)==shouyeweb->page()->mainFrame()->scrollBarMaximum(Qt::Vertical))
        {
            if(ui->leibie->currentRow()!=0)
            {
                ui->pushButton_2->show();
                if(!all) ui->pushButton->show();
                //qDebug()<<"shouyeweb's wheel event";
                return QObject::eventFilter(obj, event);;
            }
        }
        return QObject::eventFilter(obj, event);
    }
    else if(obj==ui->leibie&&event->type()==QEvent::Wheel)
    {
        QWheelEvent *w=static_cast<QWheelEvent*>(event);
        int now=ui->leibie->horizontalScrollBar()->value();
        if(w->delta()<0) ui->leibie->horizontalScrollBar()->setValue(now+30);
        else ui->leibie->horizontalScrollBar()->setValue(now-30);
        event->accept();
        return true;
    }
    return QObject::eventFilter(obj, event);


}

//void pianku::seticon(int i)
//{
//    mutex->lock();
//    imgload* t= static_cast<imgload*>(sender());
//    ui->listWidget_2->item(i)->setIcon(QIcon(t->getname()));
//    mutex->unlock();
//    //qDebug()<<t->getname();
//}

//void pianku::on_listWidget_2_doubleClicked(const QModelIndex &index)
//{
//    QString value=ui->listWidget_2->item(index.row())->data(Qt::UserRole).toString();
//    qDebug()<<"pianku:"<<value;
//    emit toplay(value);
//    QString title=ui->listWidget_2->item(index.row())->text();
//    emit setbase(title);
//}


//void pianku::loadimgs(int value)
//{
//    int beishu=value/ui->listWidget_2->height()+1;//
//    int tmpend=imgend;
//    imgend=beishu*(ui->listWidget_2->width()*ui->listWidget_2->height()/20000);
//    if(imgend!=tmpend)
//    {
//        for(;imgstart<imgend&&imgstart<imgs.length();imgstart++)
//        {
//            imgs.at(imgstart)->start();
//            pos=1;
//        }

//    }
//    if(value==bar->maximum())
//    {
//        if(ui->leibie->currentRow()!=0)
//        {
//            ui->pushButton_2->show();
//            if(!all) ui->pushButton->show();
//        }
//    }
//}

//void pianku::timeout()
//{
//    if(pos==bar->value())
//    {


//        timer.stop();
//    }
//    else {
//        pos=bar->value();
//    }
//}


void pianku::on_pushButton_clicked()
{
    page++;
    QString url="";
    url=ui->leibie->currentItem()->data(Qt::UserRole).toString().split(",").at(0);
    if(ui->comboBox->isVisible())
    {
        QString cs="";
        if(!ui->comboBox->currentData().toString().isEmpty()) cs=cs+"&tags="+ui->comboBox->currentData().toString();
        if(!ui->comboBox_2->currentData().toString().isEmpty())cs=cs+"&tags="+ui->comboBox_2->currentData().toString();
        if(!ui->comboBox_3->currentData().toString().isEmpty())cs=cs+"&tags="+ui->comboBox_3->currentData().toString();
        url.replace("&tags=",cs);
    }
    int index=ui->comboBox_4->currentIndex();
    if(index==0)
    {
        url.replace("sort=2","sort=1");
        url.replace("sortDesc=quality","sortDesc=pubTime");
    }
    else
    {
        url.replace("sort=1","sort=2");
        url.replace("sortDesc=pubTime","sortDesc=quality");
    }
    QString p="page";
    if(url.indexOf("page=")!=-1)
    {
        p="page="+QString().setNum(page);
        url.replace("page=1",p);
    }
    else
    {
        p="pageNo="+QString().setNum(page);
        url.replace("pageNo=1",p);
    }
    qDebug()<<url;
    reque.setRawHeader(QByteArray("Referer"), "http://www.tudou.com");
    reque.setRawHeader(QByteArray("User-Agent"), U_A);
    reque.setUrl(QUrl(url));

    reply=manager->get(reque);
    connect(reply,SIGNAL(finished()),this,SLOT(getlist()));
}


void pianku::on_pushButton_2_clicked()
{

    page--;
    if(page<1) page=1;
    else
    {
        QString url="";
        url=ui->leibie->currentItem()->data(Qt::UserRole).toString().split(",").at(0);
        //http://www.tudou.com/s3portal/service/pianku/data.action?pageSize=90&app=mainsitepc&deviceType=1&tagType=3&firstTagId=4&areaCode=&initials=&hotSingerId=&pageNo=1&sortDesc=quality
        if(ui->comboBox->isVisible())
        {
            QString cs="";
            if(!ui->comboBox->currentData().toString().isEmpty()) cs=cs+"&tags="+ui->comboBox->currentData().toString();
            if(!ui->comboBox_2->currentData().toString().isEmpty())cs=cs+"&tags="+ui->comboBox_2->currentData().toString();
            if(!ui->comboBox_3->currentData().toString().isEmpty())cs=cs+"&tags="+ui->comboBox_3->currentData().toString();
            url.replace("&tags=",cs);
        }
        int index=ui->comboBox_4->currentIndex();
        if(index==0)
        {
            url.replace("sort=2","sort=1");
            url.replace("sortDesc=quality","sortDesc=pubTime");
        }
        else
        {
            url.replace("sort=1","sort=2");
            url.replace("sortDesc=pubTime","sortDesc=quality");
        }
        QString p="page";
        if(url.indexOf("page=")!=-1)
        {
            p="page="+QString().setNum(page);
            url.replace("page=1",p);
        }
        else
        {
            p="pageNo="+QString().setNum(page);
            url.replace("pageNo=1",p);
        }
        qDebug()<<url;
        reque.setRawHeader(QByteArray("Referer"), "http://www.tudou.com");
        reque.setRawHeader(QByteArray("User-Agent"), U_A);
        reque.setUrl(QUrl(url));

        reply=manager->get(reque);
        connect(reply,SIGNAL(finished()),this,SLOT(getlist()));
    }
}

void pianku::on_comboBox_4_activated(int index)
{
    QString url="";
    url=ui->leibie->currentItem()->data(Qt::UserRole).toString().split(",").at(0);
    if(ui->comboBox->isVisible())
    {
        qDebug()<<"pianku:on_comboBox_4_activated(int index)";
        if(ui->leibie->currentRow()==6)
        {
            QString cs="";
            if(!ui->comboBox->currentData().toString().isEmpty()) cs=cs+"&tags=1_"+ui->comboBox->currentData().toString();
            if(!ui->comboBox_2->currentData().toString().isEmpty())cs=cs+"%2C2_"+ui->comboBox_2->currentData().toString();
            if(!ui->comboBox_3->currentData().toString().isEmpty())cs=cs+"%2C4_"+ui->comboBox_3->currentData().toString();
            url.replace("&tags=",cs);
        }
        else
        {
        QString cs="";
        if(!ui->comboBox->currentData().toString().isEmpty()) cs=cs+"&tags="+ui->comboBox->currentData().toString();
        if(!ui->comboBox_2->currentData().toString().isEmpty())cs=cs+"&tags="+ui->comboBox_2->currentData().toString();
        if(!ui->comboBox_3->currentData().toString().isEmpty())cs=cs+"&tags="+ui->comboBox_3->currentData().toString();
        url.replace("&tags=",cs);
        }
    }
    if(index==0)
    {
        url.replace("sort=2","sort=1");
        url.replace("sortDesc=quality","sortDesc=pubTime");
    }
    else
    {
        url.replace("sort=1","sort=2");
        url.replace("sortDesc=pubTime","sortDesc=quality");
    }
    qDebug()<<"on_comboBox_4_activated(int index)"<<url;
    reque.setRawHeader(QByteArray("Referer"), "http://www.tudou.com");
    reque.setRawHeader(QByteArray("User-Agent"), U_A);
    reque.setUrl(QUrl(url));

    reply=manager->get(reque);
    connect(reply,SIGNAL(finished()),this,SLOT(getlist()));
}

void pianku::on_pushButton_3_clicked()
{
    on_comboBox_4_activated(ui->comboBox_4->currentIndex());

}

//void pianku::on_listWidget_2_itemClicked(QListWidgetItem *item)
//{
//    ui->pushButton_4->setEnabled(true);

//}

//void pianku::on_pushButton_4_clicked()
//{
//    QStringList info;
//    info.append(ui->listWidget_2->currentItem()->text());
//    info.append(ui->listWidget_2->currentItem()->data(Qt::UserRole).toString());
//   // qDebug()<<ui->listWidget_2->currentItem()->icon();
//    QDateTime time = QDateTime::currentDateTime();
//    info.append(time.toString("yyyy.MM.dd hh:mm:ss"));
//    emit addshoucang(info);
//}

//void pianku::on_pushButton_5_clicked()
//{
//    QList<QListWidgetItem*> tmplist=ui->listWidget_2->selectedItems();
//    if(tmplist.isEmpty()) return;
//    for(int i=0;i<tmplist.length();i++)
//    {
//        QString name=tmplist.at(i)->text();
//        name.replace("\n","_");
//        QString url=tmplist.at(i)->data(Qt::UserRole).toString();
//        emit adddownload(name,url);
//    }
//}
void pianku::closeEvent(QCloseEvent* event){
//    for(int i=0;i<imgs.length();i++)
//    {
//        imgs.at(i)->stop();
//        imgs.at(i)->deleteLater();
//    }
}

void pianku::webplay(QUrl u){
    QByteArray d=u.toEncoded();
    QByteArray c;
    c=c.fromPercentEncoding(d);
    QString data;
    data=c;
    qDebug()<<"pianku::webplay:视频："<<data;
    QString url,title;
    if(data.indexOf(":file:")!=-1)
    {
        data.remove(QRegularExpression("file:.+/"));
    }
    if(data.indexOf("down:")!=-1)
    {
        data.remove("down:");
        url=data.split("(]").first();
        title=data.split("(]").last();
        QString name=title;
        name.replace("\n","_");
        emit adddownload(name,url);
    }
    else if(data.indexOf("keep:")!=-1)
    {
        data.remove("keep:");
        url=data.split("(]").first();
        title=data.split("(]").last();
        QStringList info;
        info.append(title);
        info.append(url);
        QDateTime time = QDateTime::currentDateTime();
        info.append(time.toString("yyyy.MM.dd hh:mm:ss"));
        emit addshoucang(info);
    }
    else{
         if(data.indexOf("file:")!=-1) url=data.split("/").last();
         else url=data;
        emit toplay(url);
        emit setbase(title);
    }

}
