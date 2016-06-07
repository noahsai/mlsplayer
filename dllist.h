#ifndef DLLIST_H
#define DLLIST_H

#include <QWidget>
#include<huancun.h>
#include<QMessageBox>
#include <QDesktopServices>
#include<dlset.h>
#include<QFile>
#include<QDataStream>
#include<hebing.h>
namespace Ui {
class dllist;
}

class dllist : public QWidget
{
    Q_OBJECT

public:
    explicit dllist(QWidget *parent = 0);
    ~dllist();
    int freehc();
    void rm2changedling(int);
    int isdling(int);
    void setformat(QString&);
    bool isdownloading();
    void stopall();

signals:
    void playfile(QStringList&);
public slots:
    void addstart(QString& name,QString& url,QString& path);
    void setdlfilepath(QString&,QString&);
    void setmaxtasks(int);
private slots:
    void on_del_pushButton_clicked();

    void on_start_pushButton_clicked();

    void on_stop_pushButton_2_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void error();
    void starttask(QListWidgetItem*,int n);
    void fin2nexttask();
    void next();
    void stop(QListWidgetItem*,int n);
    void setprogress(int ps);
    void rmtask(QListWidgetItem*);
    void partfinished(QString&);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::dllist *ui;
    int tasks;
    int max;
    QList<huancun*>  mglist;
    huancun* hc;
    void readdlset();
    void savelist();
    void readlist();
    QString format;
    QString dlfilepath;
    hebing *concat;
    QString ffmpeg;

};

#endif // DLLIST_H
