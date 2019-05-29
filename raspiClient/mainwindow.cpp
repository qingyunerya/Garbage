#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QTime>
#include <QNetworkInterface>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tcp_client=new QTcpSocket(this);
    connect(this->tcp_client,SIGNAL(connected()),this,SLOT(connected()));
    connect(this->tcp_client,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(this->tcp_client,SIGNAL(readyRead()),this,SLOT(readyread()));
    //获取所有网络接口的列表
    foreach(QNetworkInterface interface,QNetworkInterface::allInterfaces())
    {
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)
        {
            if(entry.ip().toString().contains("192.168."))
            {
                ui->lineEdit_ca->setText(entry.ip().toString());
                ui->lineEdit_cp->setText("8080");
                qDebug()<<"ipdrr"<<entry.ip().toString();
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
//输出调试
void MainWindow::debug(QString str)
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss:");
    consoleStr=consoleStr+"\n"+time+str;
    ui->textEdit_console->setText(consoleStr);
    ui->textEdit_console->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}
//发送数据
void MainWindow::on_c_send_clicked()
{
     QString str=ui->textEdit_cs->toPlainText();
     QByteArray ba=str.toLocal8Bit();
     this->tcp_client->write(ba);
}
//连接服务端
void MainWindow::on_connectToSever_clicked()
{
    QString address=ui->lineEdit_ca->text();
    QString sport=ui->lineEdit_cp->text();
    quint16 port=quint16(sport.toUInt());
    if(!this->tcp_client->isOpen())
    {
        this->tcp_client->connectToHost(address,port,QTcpSocket::ReadWrite);
        ui->connectToSever->setText(QStringLiteral("断开连接"));
        debug(QStringLiteral("连接服务器"));
    }
    else
    {
        tcp_client->close();
        debug(QString::number(this->tcp_client->isOpen())+QStringLiteral("测试"));
        ui->connectToSever->setText(QStringLiteral("连接服务器"));
        debug(QStringLiteral("断开连接"));
    }
}

//连接到服务器
void MainWindow::connected()
{
    debug(QStringLiteral("连接服务器成功"));
}
//开始读取数据
void MainWindow::readyread()
{
     debug(QStringLiteral("接受到服务端数据"));
     QString carr=QString::fromLocal8Bit(this->tcp_client->readAll());
     ui->textEdit_ca->setText(carr);
}
//断开服务端连接
void MainWindow::disconnected()
{
    debug(QStringLiteral("已断开服务端连接"));
    ui->connectToSever->setText(QStringLiteral("连接服务端"));
}
