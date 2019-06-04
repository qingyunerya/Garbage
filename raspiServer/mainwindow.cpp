#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QTextCursor>
#include <QMetaMethod>
#include <QTimer>
#include  <QGraphicsOpacityEffect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tcp_server = new QTcpServer(this);
    connect(tcp_server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    data_timer=new QTimer(this);
    data_timer->setInterval(100);
    connect(data_timer,SIGNAL(timeout()),this,SLOT(data_timer_tick()));
    commandList.append("control_key1_click");
    commandList.append("control_key2_click");
    commandList.append("control_key3_click");
    commandList.append("control_led1_click");
    commandList.append("control_led2_click");
    commandList.append("control_led3_click");
    commandList.append("control_readKeys_click");
    commandList.append("control_directControl_click");
    commandList.append("control_recLedOp_click");
    commandList.append("control_recOp_click");
    commandList.append("control_viewLedOp_click");
    commandList.append("control_viewOp_click");
    reCommandList.append("remote_status_isReadkeys:");
    reCommandList.append("remote_status_isdirectControl:");
    reCommandList.append("remote_status_l1:");
    reCommandList.append("remote_status_l2:");
    reCommandList.append("remote_status_l3:");
    ui->groupBox->setEnabled(false);
    QPixmap tb=QPixmap(":/Background/tb");
    tb.scaled(ui->centralWidget->size(),Qt::KeepAspectRatio);
    ui->centralWidget->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window,QBrush(tb));
    ui->centralWidget->setPalette(palette);
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

void MainWindow::debug2(QString str)
{
    ui->textEdit_console_2->setText(str);
    ui->textEdit_console_2->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
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
        data_timer->start(0);
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
        data_timer->stop();
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
    ui->groupBox->setEnabled(true);
    isRemoteControl=true;
}

//服务端发送数据
void MainWindow::on_s_send_clicked()
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss.zzz");
    QString str=time+ui->textEdit_ss->toPlainText();
    this->client->write(str.toUtf8());
}

//接受到客户端数据
void MainWindow::ReceiveData()
{
    debug("接受到来自"+client->peerAddress().toString()+"\n"+"客户端的数据");
    QString sarr=QString::fromUtf8(client->readAll());
    ui->textEdit_sa->setText(sarr);
    ui->textEdit_sa->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
    if(acceptCommand().contains("remote_status_"))
    {
        curCommand=acceptCommand();
        if(curCommand.compare(befCommand,Qt::CaseSensitive)!=0){befCommand=curCommand;}

        if(isRemoteControl)
        {
            int index=0;
            foreach (QString command,reCommandList)
            {
                if(curCommand.contains(command))
                {
                    index=reCommandList.indexOf(command);
                }
            }
            switch (index)
            {
            case 0:
                if(curCommand.contains("true"))
                {
                    ui->pushButton_readKeys->setText("停止读取");

                }
                else {ui->pushButton_readKeys->setText("读取按钮状态：");}
                break;
            case 1:
                if(curCommand.contains("true"))
                {
                    ui->pushButton_directControl->setText("停止直控");

                }
                else {ui->pushButton_directControl->setText("直接控制灯：");}
                break;
            case 2:
                if(curCommand.contains("true"))
                {
                    l1=true;
                    palette.setColor(QPalette::Button,QColor(255,255,0));
                    ui->pushButton_led1->setFlat(true);
                    ui->pushButton_led1->setAutoFillBackground(true);
                    ui->pushButton_led1->setPalette(palette);

                }
                else {ui->pushButton_led1->setAutoFillBackground(false);l1=false;}
                break;
            case 3:
                if(curCommand.contains("true"))
                {
                    palette.setColor(QPalette::Button,QColor(255,255,0));
                    ui->pushButton_led2->setFlat(true);
                    ui->pushButton_led2->setAutoFillBackground(true);
                    ui->pushButton_led2->setPalette(palette);
                    l2=true;
                }
                else {ui->pushButton_led2->setAutoFillBackground(false);l2=false;}
                break;
            case 4:
                if(curCommand.contains("true"))
                {
                    palette.setColor(QPalette::Button,QColor(255,255,0));
                    ui->pushButton_led3->setFlat(true);
                    ui->pushButton_led3->setAutoFillBackground(true);
                    ui->pushButton_led3->setPalette(palette);
                    l3=true;
                }
                else {ui->pushButton_led3->setAutoFillBackground(false);l3=false;}
                break;
            default:
                break;
            }
        }
    }
    else debug2(acceptCommand());
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
    if(ui->comboBox->currentText().trimmed().isEmpty())
    {
        ui->groupBox->setEnabled(false);
        debug(client->peerAddress().toString()+"已断开服务端连接");
        isRemoteControl=false;
        ui->textEdit_sa->clear();
        ui->textEdit_ss->clear();
    }

}
//发送命令
void MainWindow::sendCommand(QString str)
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss.zzz");
    str=time+":"+str;
    QByteArray ba=str.toUtf8();
    this->client->write(ba);
}
//接收命令
QString MainWindow::acceptCommand()
{
    QString carr=ui->textEdit_sa->toPlainText();
    return carr;
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
void MainWindow::data_timer_tick()
{

}

void MainWindow::on_pushButton_key1_clicked()
{
    sendCommand(commandList.at(0));
    if(isControlLed)
    {
        if(!l1)
        {
            palette.setColor(QPalette::Button,QColor(255,255,0));
            ui->pushButton_led1->setFlat(true);
            ui->pushButton_led1->setAutoFillBackground(true);
            ui->pushButton_led1->setPalette(palette);
            l1=true;
        }
        else {ui->pushButton_led1->setAutoFillBackground(false);l1=false;}
    }
}

void MainWindow::on_pushButton_key2_clicked()
{
    sendCommand(commandList.at(1));
    if(isControlLed)
    {
        if(!l2)
        {
            palette.setColor(QPalette::Button,QColor(255,255,0));
            ui->pushButton_led2->setFlat(true);
            ui->pushButton_led2->setAutoFillBackground(true);
            ui->pushButton_led2->setPalette(palette);
            l2=true;
        }
        else {ui->pushButton_led2->setAutoFillBackground(false);l2=false;}
    }
}

void MainWindow::on_pushButton_key3_clicked()
{
    sendCommand(commandList.at(2));
    if(isControlLed)
    {
        if(!l3)
        {
            palette.setColor(QPalette::Button,QColor(255,255,0));
            ui->pushButton_led3->setFlat(true);
            ui->pushButton_led3->setAutoFillBackground(true);
            ui->pushButton_led3->setPalette(palette);
            l3=true;
        }
        else {ui->pushButton_led3->setAutoFillBackground(false);l3=false;}
    }
}

void MainWindow::on_pushButton_led1_clicked()
{
    sendCommand(commandList.at(3));
    if(!l1)
    {
        palette.setColor(QPalette::Button,QColor(255,255,0));
        ui->pushButton_led1->setFlat(true);
        ui->pushButton_led1->setAutoFillBackground(true);
        ui->pushButton_led1->setPalette(palette);
        l1=true;
    }
    else {ui->pushButton_led1->setAutoFillBackground(false);l1=false;}
}

void MainWindow::on_pushButton_led2_clicked()
{
    sendCommand(commandList.at(4));
    if(!l2)
    {
        palette.setColor(QPalette::Button,QColor(255,255,0));
        ui->pushButton_led2->setFlat(true);
        ui->pushButton_led2->setAutoFillBackground(true);
        ui->pushButton_led2->setPalette(palette);
        l2=true;
    }
    else {ui->pushButton_led2->setAutoFillBackground(false);l2=false;}
}

void MainWindow::on_pushButton_led3_clicked()
{
    sendCommand(commandList.at(5));
    if(!l3)
    {
        palette.setColor(QPalette::Button,QColor(255,255,0));
        ui->pushButton_led3->setFlat(true);
        ui->pushButton_led3->setAutoFillBackground(true);
        ui->pushButton_led3->setPalette(palette);
        l3=true;
    }
    else {ui->pushButton_led3->setAutoFillBackground(false);l3=false;}
}
void MainWindow::on_pushButton_readKeys_clicked()
{
    if(!isReadKeys)
    {
        ui->pushButton_readKeys->setText("停止读取");
        sendCommand(commandList.at(6));
        isReadKeys=true;
    }
    else {ui->pushButton_readKeys->setText("读取按钮状态");sendCommand(commandList.at(6));isReadKeys=false;}
}

void MainWindow::on_pushButton_directControl_clicked()
{
    if(!isControlLed)
    {
        ui->pushButton_directControl->setText("停止直控");
        sendCommand(commandList.at(7));
        isControlLed=true;
    }
    else {ui->pushButton_directControl->setText("直接控制灯：");sendCommand(commandList.at(7));isControlLed=false;}
}
void MainWindow::on_pushButton_recLed_clicked()
{
    sendCommand(commandList.at(8));
}

void MainWindow::on_pushButton_recOperate_clicked()
{
    sendCommand(commandList.at(9));
}

void MainWindow::on_pushButton_viewLedOp_clicked()
{
    sendCommand(commandList.at(10));
}

void MainWindow::on_pushButton_viewOp_clicked()
{
    sendCommand(commandList.at(11));
}
