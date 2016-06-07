#include "dlset.h"
#include "ui_dlset.h"

extern QString cfgpath;

dlset::dlset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlset)
{
    ui->setupUi(this);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setLayout(ui->verticalLayout);
    filepath=cfgpath+"dlset";
    QString dlfilepath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/MLScache/";
    max=2;
    ui->label_2->setText(dlfilepath);
    ui->spinBox->setValue(max);
    readset();
}

dlset::~dlset()
{
    delete ui;
}

void dlset::readset()
{
    QString path,ffmpeg;
    qDebug()<<"dlset:to read dlset";
    QFile file;
    QDataStream out;
    out.setDevice(&file);
    file.setFileName(filepath);
    if(file.open(QIODevice::ReadOnly))
    {
        out>>max;
        out>>path;
        out>>ffmpeg;
        ui->spinBox->setValue(max);
        ui->label_2->setText(path);
        ui->lineEdit->setText(ffmpeg);
    }
    else {
        qDebug()<<"dlset:dlset file open error!";
    }
    file.close();
}

void dlset::saveset()
{
    int max=ui->spinBox->value();
    QString path=ui->label_2->text();
    QString ffmpeg=ui->lineEdit->text();
    if(ffmpeg.isEmpty())
    {
        ffmpeg= QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/FFmpeg/ffmpeg";
        QProcess *tty=new QProcess(this);
        qDebug()<<"dlset:find ffmpeg";
        QStringList i;
        i.append("ffmpeg");
        tty->start("/bin/which ffmpeg");
        if(tty->waitForStarted(3000))
        {
            if(tty->waitForFinished(3000))
            {
                QString result=tty->readAllStandardOutput();
                if(!result.isEmpty()) {
                    ffmpeg=result.remove("\n");
                    qDebug()<<result;
                }
                else qDebug()<<"dlset:ffmpeg not found in your system.";
            }
        }
        else qDebug()<<tty->readAll();
    }
    qDebug()<<"dlset:to save dlset";
    QFile file;
    QDataStream out;
    out.setDevice(&file);
    file.setFileName(filepath);
    if(file.open(QIODevice::WriteOnly))
    {
        out<<(int)max;
        out<<path;
        out<<ffmpeg;
    }
    else {
        qDebug()<<"dlset:dlset file open error!";
    }
    file.close();
    qDebug()<<"dlset:dlset saved";
    emit setfilepath(path,ffmpeg);
    emit setmaxtask(max);

}

void dlset::on_buttonBox_accepted()
{
    saveset();
    close();
}

void dlset::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择存放漫画的文件夹"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui->label_2->setText(dir);
    }
}

void dlset::on_buttonBox_rejected()
{
    close();
}
