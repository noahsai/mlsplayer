#ifndef DLSET_H
#define DLSET_H

#include <QDialog>
#include<QFile>
#include<QDataStream>
#include<QDebug>
#include<QFileDialog>
#include <QDesktopServices>
#include<QProcess>
namespace Ui {
class dlset;
}

class dlset : public QDialog
{
    Q_OBJECT

public:
    explicit dlset(QWidget *parent = 0);
    ~dlset();
signals:
    void setfilepath(QString&,QString&);
    void setmaxtask(int);
private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void on_buttonBox_rejected();

private:
    Ui::dlset *ui;
    void readset();
    void saveset();
    QString filepath;
    int max;

};

#endif // DLSET_H
