#include "opendialog.h"
#include "ui_opendialog.h"
#define U_A "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)"

openDialog::openDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::opendialog)
{
    ui->setupUi(this);
    setModal(true);
    setLayout(ui->verticalLayout);
    setAttribute(Qt::WA_DeleteOnClose);
    tostop=false;
    manager=NULL;
    reply=NULL;
}

openDialog::~openDialog()
{
    delete ui;
}

void openDialog::on_pushButton_clicked()
{
    QStringList file=QFileDialog::getOpenFileNames(this,tr("打开视频"),"/home",tr("Video (*.avi *.mp4 *.mpeg *.mkv *.rmvb *.rm);;All (*)"));
    if(file.isEmpty()) return;
    str=file.join("\n");
    emit result(0,str);
    close();
}

void openDialog::on_pushButton_2_clicked()
{
    if(reply!=NULL)
    {
        tostop=true;
        reply->abort();
    }
    close();
}

void openDialog::on_pushButton_3_clicked()
{
    str=ui->lineEdit->text();

    str.remove("\n");
    if(str.isEmpty()) return;
    QFile file;
    file.setFileName(str);
    if(file.exists()) {
        emit result(0,str);
        close();
        return;
    }
    manager = new QNetworkAccessManager;
    QNetworkRequest request;
    request.setUrl(QUrl(str));
    request.setRawHeader(QByteArray("User-Agent"), U_A);
    reply=manager->head(request);
    connect(reply,SIGNAL(finished()),this,SLOT(gothead()));
}

void openDialog::gothead()
{
    if(tostop)
    {
        reply->deleteLater();
        close();
        return;

    }
    if(reply->hasRawHeader("Content-Type"))
    {
        reply->deleteLater();
        QString head = reply->rawHeader("Content-Type");
        if(head.indexOf("text/html")!=-1)
        {
            emit result(1,str);
            close();
            return;
        }
    }
    emit result(2,str);
    close();
}
