#ifndef PIANKU_H
#define PIANKU_H

#include <QWidget>
#include<QTextStream>
#include<QListWidgetItem>
#include<QDebug>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
//#include<player.h>
#include<QApplication>
#include<QDesktopWidget>
#include <QAbstractItemView>
#include<QListWidget>
#include<QListWidgetItem>
#include<imgload.h>
#include<QMutex>
#include<QTimer>
#include<QScrollBar>
#include<QWheelEvent>
#include<mywebpage.h>
#include<QWebView>
#include<mywebpage.h>
#include<QWebFrame>

namespace Ui {
class pianku;
}

class pianku : public QWidget
{
    Q_OBJECT

public:
    explicit pianku(QWidget *parent = 0);
    void init();
    ~pianku();

signals:
    void toplay(QString&);
    void setbase(QString&);
    void addshoucang(QStringList&);
    void adddownload(QString&,QString&);
private slots:
    void on_leibie_itemClicked(QListWidgetItem *item);
    void getlist();
    void webplay(QUrl);
    void getshouye();
//    void on_listWidget_2_doubleClicked(const QModelIndex &index);
//    void seticon(int);
//    void loadimgs(int);
//    void timeout();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_4_activated(int index);

    void on_pushButton_3_clicked();

//    void on_listWidget_2_itemClicked(QListWidgetItem *item);

//    void on_pushButton_4_clicked();


 //   void on_pushButton_5_clicked();

private:
    qint64 pos;
    int page;
    imgload *img;
    Ui::pianku *ui;
    QNetworkAccessManager *manager;
    QNetworkRequest reque;
    QNetworkReply *reply;
  //  player* play;
    QMutex *mutex;
    QList<imgload*> imgs;
    QScrollBar *bar;
    bool all;
    bool tostop;
    bool type;
    QTimer timer;
    QString url;
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent*);
    int imgstart;
    int imgend;
    QWebView *shouyeweb;
};

#endif // PIANKU_H
