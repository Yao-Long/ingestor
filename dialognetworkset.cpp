#include "dialognetworkset.h"
#include "ui_dialognetworkset.h"


#include <QHostAddress>

DialogNetworkSet::DialogNetworkSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNetworkSet)
{
    ui->setupUi(this);
    clientFront = new QTcpSocket(this);
    connect(clientFront, SIGNAL(connected()), this, SLOT(onConnectFrontServer()));
    connect(clientFront, SIGNAL(disconnected()), this, SLOT(onDisConnectFrontServer()));
    connect(clientFront,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onSocketConnectFrontServerStateChange(QAbstractSocket::SocketState)));
    connect(clientFront,SIGNAL(readyRead()),
            this,SLOT(onSocketConnectFrontServerReadyRead()));

    clientDataCenter = new QTcpSocket(this);
    connect(clientDataCenter, SIGNAL(connected()), this, SLOT(onConnectDataCenter()));
    connect(clientDataCenter, SIGNAL(disconnected()), this, SLOT(onDisConnectDataCenter()));
    connect(clientDataCenter,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onDataCenterStateChange(QAbstractSocket::SocketState)));
    connect(clientDataCenter,SIGNAL(readyRead()),
            this,SLOT(onDataCenterReadyRead()));

    serverAgent = new QTcpSocket(this);
    connect(serverAgent, SIGNAL(connected()), this, SLOT(onConnectDataCenter()));
    connect(serverAgent, SIGNAL(disconnected()), this, SLOT(onDisConnectDataCenter()));
    connect(serverAgent,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onDataCenterStateChange(QAbstractSocket::SocketState)));
    connect(serverAgent,SIGNAL(readyRead()),
            this,SLOT(onDataCenterReadyRead()));

    clientA12 = new QTcpSocket(this);
    connect(clientA12, SIGNAL(connected()), this, SLOT(onConnectDataCenter()));
    connect(clientA12, SIGNAL(disconnected()), this, SLOT(onDisConnectDataCenter()));
    connect(clientA12,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onDataCenterStateChange(QAbstractSocket::SocketState)));
    connect(clientA12,SIGNAL(readyRead()),
            this,SLOT(onDataCenterReadyRead()));

}

DialogNetworkSet::~DialogNetworkSet()
{
    delete ui;
}

bool DialogNetworkSet::ipAndPortIsValid(QString ip, QString port)
{
    QHostAddress host;
    if(!host.setAddress(ip)) return false;
    if(port.toInt() < 1000 || port.toInt() > 65535) return false;
    return true;
}

void DialogNetworkSet::onConnectFrontServer()
{
    QString ip = ui->lineEditFrontServerIp->text();
    QString port = ui->lineEditFrontServerPort->text();
    ui->plainTextEdit->appendPlainText(tr("连接前端服务器 ") +
                                    ip + tr(" port ") + port + tr("成功！"));
    ui->pushButtonConnectFrontServer->setText(tr("断开与前端服务器连接"));
}


void DialogNetworkSet::onDisConnectFrontServer()
{
    ui->plainTextEdit->appendPlainText("已断开与前端服务器连接");
    ui->pushButtonConnectFrontServer->setText(tr("连接前端服务器"));
}


void DialogNetworkSet::onSocketConnectFrontServerStateChange(QAbstractSocket::SocketState socketState)
{
    QString state = QString::asprintf("%d", socketState);
    ui->plainTextEdit->appendPlainText(tr("socketConnectFrontServer 状态：") + state);
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        ui->plainTextEdit->appendPlainText(tr("UnconnectedState"));
        break;
    case QAbstractSocket::HostLookupState:
        ui->plainTextEdit->appendPlainText(tr("HostLookupState"));
        break;
    case QAbstractSocket::ConnectingState:
        ui->plainTextEdit->appendPlainText(tr("ConnectingState"));
        break;
    case QAbstractSocket::ConnectedState:
        ui->plainTextEdit->appendPlainText(tr("ConnectedState"));
        break;
    case QAbstractSocket::BoundState:
        ui->plainTextEdit->appendPlainText(tr("BoundState"));
        break;
    case QAbstractSocket::ClosingState:
        ui->plainTextEdit->appendPlainText(tr("ClosingState"));
        break;
    case QAbstractSocket::ListeningState:
        ui->plainTextEdit->appendPlainText(tr("ListeningState"));
        break;
    }
}


void DialogNetworkSet::onSocketConnectFrontServerReadyRead()
{
    //读数据
//    socketConnectFrontServer->readAll()
//    socketConnectFrontServer->readLine();
//    while(socketConnectFrontServer->canReadLine())
//        ui->plainTextEdit->appendPlainText("[in] "+socketConnectFrontServer->readLine());
}


void DialogNetworkSet::on_pushButtonConnectFrontServer_clicked()
{
    QString ipStr = ui->lineEditFrontServerIp->text();
    QString port = ui->lineEditFrontServerPort->text();
    if(!ipAndPortIsValid(ipStr, port)){
        ui->plainTextEdit->appendPlainText(tr("前端服务器ip或者端口不合法"));
        return;
    }

    ui->plainTextEdit->appendPlainText(tr("尝试连接前端服务器 ") + tr("ip地址 ")+ ipStr
                                       + tr(" 端口 ") + port);
    clientFront->connectToHost(ipStr, port.toInt());
}
