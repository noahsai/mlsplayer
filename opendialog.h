#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>
#include<QFile>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QFileDialog>

namespace Ui {
class opendialog;
}

class openDialog : public QDialog
{
    Q_OBJECT

public:
    explicit openDialog(QWidget *parent = 0);
    ~openDialog();

signals:
    void result(int, QString);
    //0----->file
    //1----->html
    //2----->other
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void gothead();

private:

    Ui::opendialog *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString str;
    bool tostop;
};

#endif // OPENDIALOG_H
