#ifndef DIALOGNETWORKSET_H
#define DIALOGNETWORKSET_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class DialogNetworkSet;
}

class DialogNetworkSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNetworkSet(QWidget *parent = nullptr);
    ~DialogNetworkSet();

    bool ipAndPortIsValid(QString ip, QString port);
private slots:
    void on_pushButtonConnectFrontServer_clicked();
    void onConnectFrontServer();
    void onDisConnectFrontServer();
    void onSocketConnectFrontServerStateChange(QAbstractSocket::SocketState socketState);
    void onSocketConnectFrontServerReadyRead();



private:
    Ui::DialogNetworkSet *ui;

    QTcpSocket *clientFront;
    QTcpSocket *clientDataCenter;
    QTcpSocket *serverAgent;
    QTcpSocket *clientA12;

};

#endif // DIALOGNETWORKSET_H
