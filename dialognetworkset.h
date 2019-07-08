#ifndef DIALOGNETWORKSET_H
#define DIALOGNETWORKSET_H

#include "networkdata.h"

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class DialogNetworkSet;
}



class DialogNetworkSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNetworkSet(NetWorkData *d, QWidget *parent = nullptr);
    ~DialogNetworkSet();

    bool ipAndPortIsValid(QString ip, QString port);
//    Ui::DialogNetworkSet *getUi() const;
//signals:
    //与前端服务器连接状态变化的信号
//    void frontServerChanged(bool isConnected);

private slots:
    void on_pushButtonConnectFrontServer_clicked();
//    void onConnectFrontServer();
//    void onDisConnectFrontServer();
//    void onSocketConnectFrontServerStateChange(QAbstractSocket::SocketState socketState);
//    void onSocketConnectFrontServerReadyRead();

    void on_pushButtonConnectDataCenter_clicked();

    void on_pushButtonStartLocalAgentReceiver_clicked();


    void onLocalAgentClientConnected();
    void onLocalAgentClientDisconnected();
    void onLocalAgentClientStateChange(QAbstractSocket::SocketState state);
    void onLocalAgentClientReadyRead();

    void on_pushButtonConnectA12_clicked();

public slots:
//    void onConnectFrontServer();
//    void onConnectFrontServer1();
//    void onDisConnectFrontServer();
    void onFrontServerStateChange(QAbstractSocket::SocketState socketState);
    void onDataCenterStateChange(QAbstractSocket::SocketState socketState);
    void onLocalAgentStateChange(QAbstractSocket::SocketState socketState);
    void onLocalAgentNewConnect();
    void onA12StateChange(QAbstractSocket::SocketState socketState);


private:
    Ui::DialogNetworkSet *ui;

    QTcpSocket *clientFront;
    QTcpSocket *clientDataCenter;
    QTcpSocket *serverAgent;
    QTcpSocket *clientA12;
    NetWorkData netWorkData;

};

#endif // DIALOGNETWORKSET_H
