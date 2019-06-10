#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTcpSocket;
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
    QTcpSocket *tcp_client;
    QString consoleStr;
    QTimer *data_timer;
    QTimer *ani_timer;


private slots:
    //void data_timer_tick();
    //void ani_timer_tick();
    void readyread();
    void connected();
    void disconnected();
    void on_connectToSever_clicked();
    void on_c_send_clicked();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
