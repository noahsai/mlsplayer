#include "ownapplication.h"
#define VERSION "马铃薯视频 v0.2.11"
Ownapplication::Ownapplication(int argc, char *argv[])
    :QApplication(argc,argv)
{
    toquit=false;
    serverName = "/tmp/"+QApplication::applicationName();
    qDebug()<<serverName;
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(500)) { //如果能够连接得上的话，将参数发送到服务器，然后退出
        QTextStream stream(&socket);
        QStringList args = QApplication::arguments();
        if (args.count() > 1)
            stream << args.last();
        else
            stream << QString();
        stream.flush();
        socket.waitForBytesWritten();
        toquit=true;
        return ;
    }
    //运行到这里，说明没有实例在运行，那么创建服务器。
    server = new QLocalServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newconnection())); //监听新到来的连接
    if (!server->listen(serverName)) {
        if (server->serverError() == QAbstractSocket::AddressInUseError ) { //确保能够监听成功

            qDebug()<<server->errorString()<<"remove server" <<QFile::remove(serverName);
            server->listen(serverName);
        }
    }
    qDebug()<<"没有已打开的mlsplayer。"<<server->isListening();

    play=new player;
    play->setWindowTitle(VERSION);
    play->setWindowIcon(QIcon(":/mls.png"));
    play->move(QApplication::desktop()->width()/2-play->width()/2,QApplication::desktop()->height()/2-play->height()/2);
    play->resize(990,530);
    play->show();

    if(argc>1)
    {
        QString ii(argv[1]);
        play->toplay(ii);
    }

}

void Ownapplication::newconnection(){
    QLocalSocket *socket = server->nextPendingConnection();
        if (!socket)
            return;
        socket->waitForReadyRead(1000);
        QTextStream stream(socket);
        QString url = stream.readAll();
        url.remove(" ").remove("\n");
        if(!url.isEmpty())
        play->toplay(url);
        delete socket;
        play->raise();
        play->activateWindow(); //记得激活窗口哦
}

bool Ownapplication::istoquit()
{
    return toquit;
}
