#ifndef NETWORKDATA_H
#define NETWORKDATA_H

#include <QTcpServer>
#include <QTcpSocket>



class NetWorkData
{
public:
    NetWorkData();
    QTcpSocket *clientFront;
    QTcpSocket *clientDataCenter;
    QTcpServer *serverAgent;
    QTcpSocket *clientA12;
};

#endif // NETWORKDATA_H
