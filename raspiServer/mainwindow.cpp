#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QTextCursor>
#include <QMetaMethod>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tcp_server = new QTcpServer(this);
    connect(tcp_server,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//调试输出
void MainWindow::debug(QString str)
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss:");
    consoleStr=consoleStr+"\n"+time+str;
    ui->textEdit_console->setText(consoleStr);
    ui->textEdit_console->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}
//监听端口
void MainWindow::on_connectToClient_clicked()
{
    QString sport=ui->lineEdit_sp->text();
    if(QString(sport).isEmpty()){ui->lineEdit_sp->setText("8080");sport="8080";}
    quint16 port=quint16(sport.toUInt());
    if(!tcp_server->isListening())
    {
        tcp_server->listen(QHostAddress::Any,port);
        if(tcp_server->isListening())ui->connectToClient->setText("停止监听");
        debug("监听端口"+sport+"中");
    }
    else
    {
        this->tcp_server->close();
        if(!tcp_server->isListening())
        ui->connectToClient->setText("监听端口");
        QTcpSocket *client_socket;
        foreach (client_socket, this->ClientSocketList)
        {
            debug(client_socket->peerAddress().toString());
            int i=ui->comboBox->findText(client_socket->peerAddress().toString());
            ui->comboBox->removeItem(i);
            debug("清理"+QString::number(i,10));
            client_socket->close();
            if(client_socket!=nullptr)
            {
               client_socket->destroyed();
               debug("清理socket"+QString::number(i,10));
            }
        }
        this->ClientSocketList.clear();
        debug("已停止监听");
    }
}
//连接到客户端
void MainWindow::newConnection()
{
    client=tcp_server->nextPendingConnection();
    if(ui->comboBox->findText(client->peerAddress().toString())==-1)
    {
        ui->comboBox->addItem(client->peerAddress().toString());
    }
    connect(client,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
    this->ClientSocketList.append(client); 
    debug("连接到一个客户端");
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client_socket->peerAddress().toString()==ui->comboBox->currentText()){
        this->client=client_socket;
        }
    }
}

//服务端发送数据
void MainWindow::on_s_send_clicked()
{
    this->client->write(ui->textEdit_ss->toPlainText().toLocal8Bit());
}

//接受到客户端数据
void MainWindow::ReceiveData()
{
    debug("接受到来自"+client->peerAddress().toString()+"\n"+"客户端的数据");
    QString sarr=QString::fromLocal8Bit(client->readAll());
    ui->textEdit_sa->setText(client->peerAddress().toString()+"\n"+sarr);
}
//断开服务端连接
void MainWindow::disconnected()
{
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client_socket->peerAddress().toString()==ui->comboBox->currentText()){
        int i=ui->comboBox->findText(client->peerAddress().toString());
        ui->comboBox->removeItem(i);
        client->destroyed();
        }
    }
    debug(client->peerAddress().toString()+"已断开服务端连接");
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client_socket->peerAddress().toString()==arg1){
        this->client=client_socket;
        }
    }
    debug("与"+arg1+"通信中");
}

void MainWindow::on_pushButton_readKeys_clicked()
{

}

void MainWindow::on_pushButton_directControl_clicked()
{

}

void MainWindow::on_pushButton_recLed_clicked()
{

}

void MainWindow::on_pushButton_recOperate_clicked()
{
    debug("dianji");
}

void MainWindow::on_pushButton_key1_clicked()
{

}

void MainWindow::on_pushButton_key2_clicked()
{

}

void MainWindow::on_pushButton_key3_clicked()
{

}

void MainWindow::on_pushButton_led1_clicked()
{

}

void MainWindow::on_pushButton_led2_clicked()
{

}

void MainWindow::on_pushButton_led3_clicked()
{

}
