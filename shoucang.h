#ifndef SHOUCANG_H
#define SHOUCANG_H

#include <QWidget>
#include<QFile>
#include<QListWidgetItem>
#include<QTextStream>
#include<QDebug>
#include<QMessageBox>
#include<QSettings>
namespace Ui {
class shoucang;
}

class shoucang : public QWidget
{
    Q_OBJECT

public:
    explicit shoucang(QWidget *parent = 0);
    ~shoucang();
    void addlishi(QStringList &);


signals:
    void toplay(QString&);
private slots:
    void on_shoucang_list_itemDoubleClicked(QListWidgetItem *item);

    void on_lishi_list_itemDoubleClicked(QListWidgetItem *item);
    void addshoucang(QStringList &);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    void readsc();
    void readls();
    void savesc();
    void savels();
    Ui::shoucang *ui;
    QListWidgetItem *item;
};

#endif // SHOUCANG_H
