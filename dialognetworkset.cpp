#include "dialognetworkset.h"
#include "mainwindow.h"
#include "networkdata.h"
#include "ui_dialognetworkset.h"


#include <QHostAddress>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QHostInfo>



#define A12PORT 1200



DialogNetworkSet::DialogNetworkSet(NetWorkData *d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNetworkSet)
{
    ui->setupUi(this);
    initLanguage();

    netWorkData = *d;

    connect(d->clientFront, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onFrontServerStateChange(QAbstractSocket::SocketState)));
    connect(d->clientDataCenter, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onDataCenterStateChange(QAbstractSocket::SocketState)));
//    connect(d->serverAgent, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//            this, SLOT(onLocalAgentStateChange(QAbstractSocket::SocketState)));
    connect(d->serverAgent,SIGNAL(newConnection()),this,SLOT(onLocalAgentNewConnect()));
    connect(d->clientA12, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onA12StateChange(QAbstractSocket::SocketState)));


    //初始化本地地址组合框
    QString hostName=QHostInfo::localHostName();
    QHostInfo hostInfo=QHostInfo::fromName(hostName);
    QList<QHostAddress> addrList=hostInfo.addresses();
    int localAddrCount = addrList.count();
    for (int i = 0; i < localAddrCount; i++){
        ui->comboBoxLocalAgent->addItem(addrList.at(i).toString());
    }


//    QSqlQuery query;
//    QString sql = "select * from networkTab";
//    if(!query.exec(sql)){
//        QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
//        return;
//    }
//    QString frontIp;
//    int frontPort;
//    QString dataCenterIp;
//    int dataCenterPort;
//    QString localAgentIp;
//    int localAgentPort;
//    QString a12Ip;
//    QString a12TeamName;

//    "frontIp text,"
//    "frontPort int,"
//    "dataCenterIp text,"
//    "dataCenterPort int,"
//    "localAgentIp text,"
//    "localAgentPort int,"
//    "a12Ip text,"
//    "a12TeamName text"

//    while (query.next()) {
//        if(query.isValid()){
//            frontIp = query.value(0).toString();
//            frontPort = query.value(1).toInt();
//            dataCenterIp = query.value(2).toString();
//            dataCenterPort = query.value(3).toInt();
//            localAgentIp = query.value(4).toString();
//            localAgentPort = query.value(5).toInt();
//            a12Ip = query.value(6).toString();
//            a12TeamName = query.value(7).toString();

//            ui->lineEditFrontServerIp->setText(frontIp);
//            ui->lineEditFrontServerPort->setText(QString::asprintf("%d", frontPort));
//            ui->lineEditDataCenterIp->setText(dataCenterIp);
//            ui->lineEditDataCenterPort->setText(QString::asprintf("%d", dataCenterPort));
//            ui->lineEditLocalAgentReceiverIP->setText(localAgentIp);
//            ui->lineEditLocalAgentReceiverPort->setText(QString::asprintf("%d", localAgentPort));
//            ui->lineEditA12Ip->setText(a12Ip);
//            ui->lineEditA12TeamName->setText(a12TeamName);

//            break;
//        }
//    }
//    ui->lineEditFrontServerIp->setText(frontIp);
//    ui->lineEditFrontServerPort->setText(QString::asprintf("%d", frontPort));
//    ui->lineEditDataCenterIp->setText(dataCenterIp);
//    ui->lineEditDataCenterPort->setText(QString::asprintf("%d", dataCenterPort));
//    ui->lineEditLocalAgentReceiverIP->setText(localAgentIp);
//    ui->lineEditLocalAgentReceiverPort->setText(QString::asprintf("%d", localAgentPort));
//    ui->lineEditA12Ip->setText(a12Ip);
//    ui->lineEditA12TeamName->setText(a12TeamName);

//    if(d->frontIsConnected){
//        ui->pushButtonConnectFrontServer->setText(tr("断开与前端服务器连接"));
//    }
//    if(d->dataCenterIsConnected){
//        ui->pushButtonConnectDataCenter->setText(tr("断开与数据中心连接"));
//    }
//    if(d->localAgentIsStarted){
//        ui->pushButtonConnectFrontServer->setText(tr("停止本地代理服务器"));
//    }
//    if(d->a12Isconnected){
//        ui->pushButtonConnectDataCenter->setText(tr("断开与A12连接"));
//    }
//    MainWindow *w = qobject_cast<MainWindow *>(parent);
//    connect(netWorkData.clientFront, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//            this, SLOT(onConnectFrontServer()));


//    connect(w->clientFront, SIGNAL(disconnected()), this, SLOT(onDisConnectFrontServer()));
    //连接与前端服务器连接状态变化的信号和槽
//    connect(this, SIGNAL(frontServerChanged(bool)), parent, SLOT(onFrontServerStatusChanged(bool)));
}

DialogNetworkSet::~DialogNetworkSet()
{
    delete ui;
}



extern QTranslator *trans;


void DialogNetworkSet::initLanguage()
{
    qApp->installTranslator(trans);
    ui->retranslateUi(this);
}


bool DialogNetworkSet::ipAndPortIsValid(QString ip, QString port)
{
    QHostAddress host;
    if(!host.setAddress(ip)) return false;
    if(port.toInt() < 1000 || port.toInt() > 65535) return false;
    return true;
}



void DialogNetworkSet::on_pushButtonConnectFrontServer_clicked()
{
    if (netWorkData.clientFront->state()==QAbstractSocket::ConnectedState){
        //断开连接
        netWorkData.clientFront->disconnectFromHost();
        ui->plainTextEdit->appendPlainText(tr("已断开与前端服务器连接"));
        ui->pushButtonConnectFrontServer->setText(tr("连接前端服务器"));
        ui->pushButtonConnectFrontServer->setDisabled(false);
        return;
    }

    QString ipStr = ui->lineEditFrontServerIp->text();
    QString port = ui->lineEditFrontServerPort->text();
    if(!ipAndPortIsValid(ipStr, port)){
        ui->plainTextEdit->appendPlainText(tr("前端服务器ip或者端口不合法"));
        return;
    }

    ui->plainTextEdit->appendPlainText(tr("尝试连接前端服务器 ") + tr("ip地址 ")+ ipStr
                                       + tr(" 端口 ") + port);
    netWorkData.clientFront->connectToHost(ipStr, port.toInt());
//    emit frontServerChanged(true);
}


void DialogNetworkSet::on_pushButtonConnectDataCenter_clicked()
{
    if (netWorkData.clientDataCenter->state()==QAbstractSocket::ConnectedState){
        //断开连接
        netWorkData.clientDataCenter->disconnectFromHost();
        ui->plainTextEdit->appendPlainText(tr("已断开与数据中心连接"));
        ui->pushButtonConnectDataCenter->setText(tr("连接数据中心"));
        ui->pushButtonConnectDataCenter->setDisabled(false);
        return;
    }


    QString ipStr = ui->lineEditDataCenterIp->text();
    QString port = ui->lineEditDataCenterPort->text();
    if(!ipAndPortIsValid(ipStr, port)){
        ui->plainTextEdit->appendPlainText(tr("数据中心ip或者端口不合法"));
        return;
    }
    ui->plainTextEdit->appendPlainText(tr("数据中心 ") + tr("ip地址 ")+ ipStr
                                       + tr(" 端口 ") + port);
    netWorkData.clientDataCenter->connectToHost(ipStr, port.toInt());
}


void DialogNetworkSet::on_pushButtonStartLocalAgentReceiver_clicked()
{
    if(netWorkData.serverAgent->isListening()){
        netWorkData.serverAgent->close();
        ui->plainTextEdit->appendPlainText(tr("停止本地代理服务器"));
        ui->pushButtonStartLocalAgentReceiver->setText(tr("启动本地代理服务器"));
        return;
    }
    QString ipStr = ui->comboBoxLocalAgent->currentText();
    QString port = ui->lineEditLocalAgentReceiverPort->text();
    if(!ipAndPortIsValid(ipStr, port)){
        ui->plainTextEdit->appendPlainText(tr("本地代理服务器ip或者端口不合法"));
        return;
    }
    ui->plainTextEdit->appendPlainText(tr("本地代理服务器 ") + tr("ip地址 ")+ ipStr
                                       + tr(" 端口 ") + port);

    QString hostName=QHostInfo::localHostName();//本地主机名
    QHostInfo hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";
    QList<QHostAddress> addrList=hostInfo.addresses();
    int localAddrCount = addrList.count();
    if(localAddrCount < 1){
        ui->plainTextEdit->appendPlainText(tr("本机没有ip地址！"));
        return;
    }
    bool isLocalAddr = false;
    QHostAddress aHost;
    for (int i = 0; i < localAddrCount; i++)
    {
        aHost=addrList.at(i);
        if(aHost.toString() == ipStr){
            isLocalAddr = true;
            break;
        }
    }
    if(!isLocalAddr){
        ui->plainTextEdit->appendPlainText(ipStr + tr("不是本地地址！"));
        return;
    }
    //开始监听
    netWorkData.serverAgent->listen(aHost, port.toInt());
    ui->pushButtonStartLocalAgentReceiver->setText(tr("停止本地代理服务器"));

}


void DialogNetworkSet::on_pushButtonConnectA12_clicked()
{
    if (netWorkData.clientA12->state()==QAbstractSocket::ConnectedState){
        //断开连接
        netWorkData.clientA12->disconnectFromHost();
        ui->plainTextEdit->appendPlainText(tr("已断开与A12系统连接"));
        ui->pushButtonConnectA12->setText(tr("连接A12服务器"));
        ui->pushButtonConnectA12->setDisabled(false);
        return;
    }


    QString ipStr = ui->lineEditA12Ip->text();
    if(!ipAndPortIsValid(ipStr, "1111")){
        ui->plainTextEdit->appendPlainText(tr("A12服务器ip不合法"));
        return;
    }
    ui->plainTextEdit->appendPlainText(tr("A12 ") + tr("ip地址 ")+ ipStr
                                       + tr(" 端口 ") + QString::asprintf("%d", A12PORT));
    netWorkData.clientA12->connectToHost(ipStr, A12PORT);
}


//void DialogNetworkSet::onConnectFrontServer()
//{
//    QString ip = ui->lineEditFrontServerIp->text();
//    QString port = ui->lineEditFrontServerPort->text();
//    ui->plainTextEdit->appendPlainText(tr("连接前端服务器 ") +
//                                    ip + tr(" port ") + port + tr("成功！"));
//    ui->pushButtonConnectFrontServer->setText(tr("断开与前端服务器连接"));
//}

//void DialogNetworkSet::onConnectFrontServer1()
//{
//    QString ip = ui->lineEditFrontServerIp->text();
//    QString port = ui->lineEditFrontServerPort->text();
//    ui->plainTextEdit->appendPlainText(tr("11111111111连接前端服务器 ") +
//                                    ip + tr(" port ") + port + tr("成功！"));
//    ui->pushButtonConnectFrontServer->setText(tr("断开与前端服务器连接"));
//}



//void DialogNetworkSet::onDisConnectFrontServer()
//{
//    ui->plainTextEdit->appendPlainText("已断开与前端服务器连接");
//    ui->pushButtonConnectFrontServer->setText(tr("连接前端服务器"));
//}


void DialogNetworkSet::onFrontServerStateChange(QAbstractSocket::SocketState socketState)
{
    QString state = QString::asprintf("%d", socketState);

//    qDebug()<<tr("socketConnectFrontServer 状态：") + state;

    ui->plainTextEdit->appendPlainText(tr("socketConnectFrontServer 状态：") + state);
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        ui->plainTextEdit->appendPlainText(tr("断开与前端服务器连接！"));
        ui->pushButtonConnectFrontServer->setText(tr("连接前端服务器"));
        ui->pushButtonConnectFrontServer->setDisabled(false);
        break;
    case QAbstractSocket::HostLookupState:
        ui->plainTextEdit->appendPlainText(tr("HostLookupState"));
        break;
    case QAbstractSocket::ConnectingState:
        ui->plainTextEdit->appendPlainText(tr("连接前端服务器中..."));
        ui->pushButtonConnectFrontServer->setDisabled(true);
        break;
    case QAbstractSocket::ConnectedState:
        ui->plainTextEdit->appendPlainText(tr("连接前端服务器成功！"));
        ui->pushButtonConnectFrontServer->setText(tr("断开与前端服务器连接"));
        ui->pushButtonConnectFrontServer->setDisabled(false);
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



void DialogNetworkSet::onDataCenterStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        ui->plainTextEdit->appendPlainText(tr("断开与数据中心连接！"));
        ui->pushButtonConnectDataCenter->setText(tr("连接数据中心"));
        ui->pushButtonConnectDataCenter->setDisabled(false);
        break;
    case QAbstractSocket::HostLookupState:
//        ui->plainTextEdit->appendPlainText(tr("HostLookupState"));
        break;
    case QAbstractSocket::ConnectingState:
        ui->plainTextEdit->appendPlainText(tr("连接数据中心中..."));
        ui->pushButtonConnectDataCenter->setDisabled(true);
        break;
    case QAbstractSocket::ConnectedState:
        ui->plainTextEdit->appendPlainText(tr("连接数据中心成功！"));
        ui->pushButtonConnectDataCenter->setText(tr("断开与数据中心连接"));
        ui->pushButtonConnectDataCenter->setDisabled(false);
        break;
    case QAbstractSocket::BoundState:
//        ui->plainTextEdit->appendPlainText(tr("BoundState"));
        break;
    case QAbstractSocket::ClosingState:
//        ui->plainTextEdit->appendPlainText(tr("ClosingState"));
        break;
    case QAbstractSocket::ListeningState:
//        ui->plainTextEdit->appendPlainText(tr("ListeningState"));
        break;
    }
}


//void DialogNetworkSet::onLocalAgentStateChange(QAbstractSocket::SocketState socketState)
//{
//    switch(socketState)
//    {
//    case QAbstractSocket::UnconnectedState:
//        ui->plainTextEdit->appendPlainText(tr("断开与前端服务器连接！"));
//        ui->pushButtonConnectFrontServer->setText(tr("连接前端服务器"));
//        ui->pushButtonConnectFrontServer->setDisabled(false);
//        break;
//    case QAbstractSocket::HostLookupState:
////        ui->plainTextEdit->appendPlainText(tr("HostLookupState"));
//        break;
//    case QAbstractSocket::ConnectingState:
//        ui->plainTextEdit->appendPlainText(tr("连接前端服务器中..."));
//        ui->pushButtonConnectFrontServer->setDisabled(true);
//        break;
//    case QAbstractSocket::ConnectedState:
//        ui->plainTextEdit->appendPlainText(tr("连接前端服务器成功！"));
//        ui->pushButtonConnectFrontServer->setText(tr("断开与前端服务器连接"));
//        ui->pushButtonConnectFrontServer->setDisabled(false);
//        break;
//    case QAbstractSocket::BoundState:
////        ui->plainTextEdit->appendPlainText(tr("BoundState"));
//        break;
//    case QAbstractSocket::ClosingState:
////        ui->plainTextEdit->appendPlainText(tr("ClosingState"));
//        break;
//    case QAbstractSocket::ListeningState:
//        ui->plainTextEdit->appendPlainText(tr("启动本地代理服务器成功！"));
//        ui->pushButtonConnectFrontServer->setText(tr("停止本地代理服务器"));
//        ui->pushButtonConnectFrontServer->setDisabled(false);
//        break;
//    }
//}


void DialogNetworkSet::onA12StateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        ui->plainTextEdit->appendPlainText(tr("断开与A12服务器连接！"));
        ui->pushButtonConnectA12->setText(tr("连接A12服务器"));
        ui->pushButtonConnectA12->setDisabled(false);
        break;
    case QAbstractSocket::HostLookupState:
//        ui->plainTextEdit->appendPlainText(tr("HostLookupState"));
        break;
    case QAbstractSocket::ConnectingState:
        ui->plainTextEdit->appendPlainText(tr("连接A12服务器中..."));
        ui->pushButtonConnectA12->setDisabled(true);
        break;
    case QAbstractSocket::ConnectedState:
        ui->plainTextEdit->appendPlainText(tr("连接A12服务器成功！"));
        ui->pushButtonConnectA12->setText(tr("断开与A12服务器连接"));
        ui->pushButtonConnectA12->setDisabled(false);
        break;
    case QAbstractSocket::BoundState:
//        ui->plainTextEdit->appendPlainText(tr("BoundState"));
        break;
    case QAbstractSocket::ClosingState:
//        ui->plainTextEdit->appendPlainText(tr("ClosingState"));
        break;
    case QAbstractSocket::ListeningState:
//        ui->plainTextEdit->appendPlainText(tr("ListeningState"));
        break;
    }
}


void DialogNetworkSet::onLocalAgentClientConnected(){
//    ui->plainTextEdit->appendPlainText("**client socket connected");
//    ui->plainTextEdit->appendPlainText("**peer address:"+
//                                   tcpSocket->peerAddress().toString());
//    ui->plainTextEdit->appendPlainText("**peer port:"+
//                                   QString::number(tcpSocket->peerPort()));
}

void DialogNetworkSet::onLocalAgentClientDisconnected(){
//    ui->plainTextEdit->appendPlainText("**client socket disconnected");
//    tcpSocket->deleteLater();
//    //    deleteLater();//QObject::deleteLater();
}

void DialogNetworkSet::onLocalAgentClientStateChange(QAbstractSocket::SocketState socketState){
    QString state = QString::asprintf("%d", socketState);
    ui->plainTextEdit->appendPlainText(tr("local agent client state：") + state);
}

void DialogNetworkSet::onLocalAgentClientReadyRead(){
//    while(tcpSocket->canReadLine())
//        ui->plainTextEdit->appendPlainText("[in] "+tcpSocket->readLine());
////        lines.append(clientConnection->readLine());
}

void DialogNetworkSet::onLocalAgentNewConnect()
{
    //有新的客户端连接
    QTcpSocket *tcpSocket = netWorkData.serverAgent->nextPendingConnection();

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(onLocalAgentClientConnected()));
    onLocalAgentClientConnected();
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onLocalAgentClientDisconnected()));
    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onLocalAgentClientStateChange(QAbstractSocket::SocketState)));
    onLocalAgentClientStateChange(tcpSocket->state());

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onLocalAgentClientReadyRead()));
}




