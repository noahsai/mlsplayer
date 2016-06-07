#include <QApplication>
#include<player.h>
#include<QApplication>
#include <QDesktopWidget>
#include<pianku.h>
#include<QSharedMemory>
#include<ownapplication.h>
int main(int argc, char *argv[])
{
    Ownapplication *a=new Ownapplication(argc,argv);
    qDebug()<<"a.quit:"<<a->istoquit();
    if(a->istoquit()) return 0;
    return a->exec();
}
