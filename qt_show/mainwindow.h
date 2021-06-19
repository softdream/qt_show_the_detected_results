#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void udpInit();
    void timerInit();
    void getImageFromArray( const QString &data );

    void udpServer2Init();

private slots:
    void on_pushButton_clicked();
    void udpServerReceiveData();

    void timerUpdate();

    void udpServerRecvResults();

private:
    QUdpSocket *udp_server;
    QHostAddress client_address;
    quint16 client_port = 0;

    QUdpSocket *udp_server2;

    QImage image;

    //-------- Test Data ------------//
    int recvCount;

    QTimer *timer;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
