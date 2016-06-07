#ifndef ZHUANJIDL_H
#define ZHUANJIDL_H

#include <QWidget>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QRegularExpression>
namespace Ui {
class zhuanjidl;
}

class zhuanjidl : public QWidget
{
    Q_OBJECT

public:
    explicit zhuanjidl(QWidget *parent = 0);
    ~zhuanjidl();
    void getdata(QString& url);


private slots:
    void on_pushButton_clicked();
    void getlist();
    void on_pushButton_2_clicked();

signals:
    void  finished(QStringList&);
private:
    Ui::zhuanjidl *ui;
    QRegularExpression reg;
    QRegularExpressionMatchIterator matchs;
    QNetworkAccessManager *manager;
    QNetworkRequest reque;
    QNetworkReply *reply;
    QStringList list;
};

#endif // ZHUANJIDL_H
