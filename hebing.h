#ifndef HEBING_H
#define HEBING_H

#include <QDialog>
#include<QProcess>
#include<QStandardPaths>
#include<QDebug>
#include<QDir>
namespace Ui {
class hebing;
}

class hebing : public QDialog
{
    Q_OBJECT

public:
    explicit hebing(QWidget *parent = 0);
    bool addtask(QString& title,QString& path);
    bool isrunning();
    void setffmpeg(QString&);
    ~hebing();

private slots:
    void next();
    void error();
    void on_pushButton_clicked();

private:
    void start();
    Ui::hebing *ui;
    QProcess *tty;
    int all;
    int now;
    bool ing;
    QString ffmpeg;
};

#endif // HEBING_H
