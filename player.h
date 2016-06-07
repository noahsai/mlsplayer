#ifndef PLAYER_H
#define PLAYER_H
#include <QWidget>
#include<QFile>
#include<QTextStream>
#include<QIODevice>
#include<QDebug>
#include<QRegularExpression>
#include<QLayout>
#include<QPushButton>
#include<QProcess>
#include <QApplication>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QListWidget>
#include<QTextCodec>
#include<getufromc.h>
//#include<QMediaPlaylist>
#include<QTimer>
#include<QKeyEvent>
#include<pianku.h>
#include<QWebView>
#include<shoucang.h>
#include<QModelIndex>
#include<QFileDialog>
#include<dllist.h>
#include<zhuanjidl.h>
#include<QWebPage>
#include<QProcess>
#include<opendialog.h>
#include<mywebpage.h>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include<gettime.h>
#include<QThread>
#include<QtAV>
#include <QtAVWidgets>
QString getsc(QString& a,qlonglong t,QString& b);

using namespace QtAV ;

namespace Ui {
class player;
}

class player : public QWidget
{
    Q_OBJECT

public:
    explicit player(QWidget *parent = 0);
    ~player();
    // player(const QString&);

public slots:
    void toplay(QString&);

private slots:
    void gottime(qint64,int);
    void addzjdownload(QStringList&);
    void adddownload(QString&,QString&);
    void setbase(QString& title);
    void getfile();
    void getlist();
    void itemclicked(QListWidgetItem*);
    void messfrommp();
    void timeout();
    void inittimeout();
    void webplay(QUrl);
    void openfile(QStringList&);
    void playtimerout();
    void toopen(int,QString);
    void getcsforplay(QString&);

    void on_pushButton_4_clicked(bool checked);

    void on_pushButton_3_clicked();

    void on_video_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked(bool checked);

    void setvideolistcurrentrow(int);

    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);

    void on_horizontalSlider_sliderMoved(int position);


    void on_qingxidu_activated(int index);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_pushButton_5_clicked(bool checked);

    void setfullscreen(bool full);

    void on_yuyan_activated(int index);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();


    void playstoped();

    void mediaError(AVError);

    void on_pushButton_search_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_10_clicked();


    void on_pushButton_11_clicked();

    void on_youxian_activated(int index);

    void on_pushButton_12_clicked();

private:
    Ui::player *ui;
    void fileplay(int);
    void work(const QString& );
    bool fenxi();
    bool fenxi2( );
    void getsite();
    void fenxi3();
    void fenxi4();
    void passad();
    void reset4other();
    void reset4self();
    void play4auto(int);
    void saveset(int);
    void readset();
    bool you_get(QString );
    QString you_geturls(QString&,QString&);
    bool eventFilter(QObject *, QEvent *);
    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *event);
    void showbutton(bool);
    void showbar(bool);
    void toseek(int pos);
    QNetworkAccessManager *manager;
    QNetworkRequest reque;
    QNetworkReply *reply;
    //QNetworkReply *reply2;

    QUrl url;
    QString encode(const QString& );
    QList<QPushButton*> list;
    QString name;
    QString format;
    QString youget_format;
    QString theurl;
    int zhuanji;
    int getfiletype;
    QString html;
    QRegularExpression reg;
    QRegularExpressionMatch match;
    QRegularExpressionMatchIterator matchs;
    QProcess *process;
    bool notlistcheck;
    int now_ji;
    QWidget w;

    //QMediaPlaylist *plist;
    QTimer *timer;
    QTimer *playtimer;
    int mousex,mousey;
    pianku *ku;
    int curplay;
    QWebView* webview;
    shoucang *myshoucang;
    bool online;
    QString base;
    dllist *download;
    zhuanjidl *zjdialog;
    bool init;
    QNetworkCookieJar *jar;

    gettime* getduration;
    QList<qint64> timelist;
    QString playrf;
    QThread *workerThread;
    qint64 oldposition;
    QLabel *whatstime;
    QString U_A;

    AVPlayer *aplayer,*tmplayer;
    QtAV::GLWidgetRenderer2 *renderer;

    bool autochange;
};
#endif // PLAYER_H
