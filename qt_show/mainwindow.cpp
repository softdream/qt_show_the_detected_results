#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkInterface>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    ui->label_image->setAutoFillBackground(true);
    ui->label_image->setPalette(label_palette);

    // --------- Init the Udp Server ----------//
    this->udpInit();
    recvCount = 0;
    this->udpServer2Init();

    // --------- Init A Time ---------//
    timerInit();
}

MainWindow::~MainWindow()
{
    delete ui;

    udp_server->close();
}

void MainWindow::udpInit()
{
    quint16 local_port = 2333;
    udp_server = new QUdpSocket(this);
    ui->textBrowser_local_ip->setText(QNetworkInterface().allAddresses().at(2).toString());

    //int ret = udp_server->bind( local_port, QUdpSocket::ShareAddress );
    int ret = udp_server->bind( QHostAddress("192.168.8.240"), local_port );
    if( ret < 0 ){
        ui->label_status->setText("bind failed");
    }
    else {
        ui->label_status->setText("port binded");
        connect(udp_server, SIGNAL(readyRead()), this, SLOT(udpServerReceiveData()));
    }
}

void MainWindow::udpServer2Init()
{
    quint16 local_port = 2334;
    udp_server2 = new QUdpSocket(this);

    //int ret = udp_server->bind( local_port, QUdpSocket::ShareAddress );
    int ret = udp_server2->bind( QHostAddress("192.168.8.240"), local_port );
    if( ret < 0 ){

    }
    else {
        connect(udp_server2, SIGNAL(readyRead()), this, SLOT(udpServerRecvResults()));
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setText("Ready For Receiving");

    // Add a Slot
   // connect(udp_server, SIGNAL(readyRead()), this, SLOT(udpServerReceiveData()));
}

void MainWindow::udpServerReceiveData()
{
    QByteArray datagram;

    while( udp_server->hasPendingDatagrams() ){
        recvCount ++;
        qDebug()<<"receive count: "<<recvCount<<endl;

        ui->textBrowser_frame_count->setText(QString::number(recvCount));
        qDebug()<<"received length: "<<udp_server->pendingDatagramSize()<<endl;

        //------------ Allocate the memory for the coming image ----------//
        datagram.resize( udp_server->pendingDatagramSize() );

        //------------ Received the Data sent from the client ------------//
        udp_server->readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);
        ui->textBrowser_ip->setText(client_address.toString());
        ui->textBrowser_port->setText(QString::number(client_port));
        //qDebug()<<"received data: "<<datagram.data()<<endl;
        //ui->textEdit_console->setText(datagram);

        // --------- Display the Image ---------//
        getImageFromArray( datagram.data() );
        ui->label_image->setPixmap(QPixmap::fromImage(this->image).scaled(ui->label_image->size()));

    }
}

void MainWindow::getImageFromArray( const QString &data )
{
    QByteArray imageData = QByteArray::fromBase64(data.toLatin1());

    this->image.loadFromData(imageData);

}

void MainWindow::timerInit()
{
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(500);
}

void MainWindow::timerUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");

    ui->label_date->setText(str);
}

void MainWindow::udpServerRecvResults()
{
    QByteArray datagram;

    while( udp_server2->hasPendingDatagrams() ){
        //------------ Allocate the memory for the coming image ----------//
        datagram.resize( udp_server2->pendingDatagramSize() );

        //------------ Received the Data sent from the client ------------//
        udp_server2->readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);

        qDebug()<<"received data: "<<datagram.data()<<endl;
        ui->textEdit_console->setText(datagram);

        ui->textBrowser_objects->setText(datagram.data());
    }
}
