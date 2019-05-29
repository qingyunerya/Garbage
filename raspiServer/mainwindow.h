#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTcpSocket;
class QTcpServer;
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
    QList<QTcpSocket *> ClientSocketList;
    QTcpServer *tcp_server;
    QTcpSocket *client;
    QString consoleStr;

private slots:

    void newConnection();
    void ReceiveData();
    void disconnected();
    void on_connectToClient_clicked();
    void on_s_send_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_readKeys_clicked();

    void on_pushButton_directControl_clicked();

    void on_pushButton_recLed_clicked();

    void on_pushButton_recOperate_clicked();

    void on_label_key1_linkActivated(const QString &link);

    void on_label_key2_linkHovered(const QString &link);

    void on_pushButton_key1_clicked();

    void on_pushButton_key2_clicked();

    void on_pushButton_key3_clicked();

    void on_pushButton_led1_clicked();

    void on_pushButton_led2_clicked();

    void on_pushButton_led3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
