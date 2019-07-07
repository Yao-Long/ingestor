#include "mainwindow.h"






void MainWindow::onConnectFrontServer()
{
//    QString ip = ui->lineEditFrontServerIp->text();
//    QString port = ui->lineEditFrontServerPort->text();
//    ui->plainTextEdit->appendPlainText(tr("连接前端服务器 ") +
//                                    ip + tr(" port ") + port + tr("成功！"));
//    ui->pushButtonConnectFrontServer->setText(tr("断开与前端服务器连接"));
}


void MainWindow::onDisConnectFrontServer()
{
//    ui->plainTextEdit->appendPlainText("已断开与前端服务器连接");
//    ui->pushButtonConnectFrontServer->setText(tr("连接前端服务器"));
}


void MainWindow::onSocketConnectFrontServerStateChange(QAbstractSocket::SocketState socketState)
{
//    QString state = QString::asprintf("%d", socketState);
//    ui->plainTextEdit->appendPlainText(tr("socketConnectFrontServer 状态：") + state);
//    switch(socketState)
//    {
//    case QAbstractSocket::UnconnectedState:
//        ui->plainTextEdit->appendPlainText(tr("UnconnectedState"));
//        break;
//    case QAbstractSocket::HostLookupState:
//        ui->plainTextEdit->appendPlainText(tr("HostLookupState"));
//        break;
//    case QAbstractSocket::ConnectingState:
//        ui->plainTextEdit->appendPlainText(tr("ConnectingState"));
//        break;
//    case QAbstractSocket::ConnectedState:
//        ui->plainTextEdit->appendPlainText(tr("ConnectedState"));
//        break;
//    case QAbstractSocket::BoundState:
//        ui->plainTextEdit->appendPlainText(tr("BoundState"));
//        break;
//    case QAbstractSocket::ClosingState:
//        ui->plainTextEdit->appendPlainText(tr("ClosingState"));
//        break;
//    case QAbstractSocket::ListeningState:
//        ui->plainTextEdit->appendPlainText(tr("ListeningState"));
//        break;
//    }
}


void MainWindow::onSocketConnectFrontServerReadyRead()
{
    //读数据
//    socketConnectFrontServer->readAll()
//    socketConnectFrontServer->readLine();
//    while(socketConnectFrontServer->canReadLine())
//        ui->plainTextEdit->appendPlainText("[in] "+socketConnectFrontServer->readLine());
}
