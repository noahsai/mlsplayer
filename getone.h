#ifndef GETONE_H
#define GETONE_H
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QListWidget>
#include<QTextCodec>
#include<QRegularExpression>
#include<QEventLoop>
#include<QTimer>
class getone: public QObject
{
    Q_OBJECT
public:
    getone(QObject* parent = 0);
    ~getone();
    void start(const QUrl &url);
    QString getresult();

signals:
    void finish();

public slots:
    void replyfinished();

private:
    QNetworkAccessManager man;
    QNetworkReply *reply;
    QString result;
    QRegularExpression reg;
    QRegularExpressionMatch match;
};



#endif // GETONE_H
