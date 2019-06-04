#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTcpSocket;
class QTcpServer;
class QTimer;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void debug(QString str);
    void debug2(QString str);
    QTimer *data_timer;
    QList<QString> commandList;
    QList<QString> reCommandList;
    QList<QTcpSocket *> ClientSocketList;
    QTcpServer *tcp_server;
    QTcpSocket *client;
    QString consoleStr;
    QString consoleStr2;
    QString curCommand;
    QString befCommand="null";
    bool isStartReadKeys=false;
    void sendCommand(QString str);
    QString acceptCommand();
    bool isReadKeys=false;
    bool isControlLed=false;
    bool isRemoteControl=false;
    QPalette palette;
    bool l1=false,l2=false,l3=false;

private slots:

    void newConnection();
    void ReceiveData();
    void disconnected();
    void on_connectToClient_clicked();
    void on_s_send_clicked();
    void data_timer_tick();
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_readKeys_clicked();

    void on_pushButton_directControl_clicked();

    void on_pushButton_recLed_clicked();

    void on_pushButton_recOperate_clicked();

    void on_pushButton_key1_clicked();

    void on_pushButton_key2_clicked();

    void on_pushButton_key3_clicked();

    void on_pushButton_led1_clicked();

    void on_pushButton_led2_clicked();

    void on_pushButton_led3_clicked();

    void on_pushButton_viewLedOp_clicked();

    void on_pushButton_viewOp_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
