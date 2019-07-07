
#include <QFile>
#include <QtDebug>
#include <QTreeWidgetItem>
#include <ui_dialognewplugin.h>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "dialogcreateproject.h"
#include "dialognewplugin.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "acmeplugin.h"
#include "dialognewinstrument.h"
#include "dialogdelplugin.h"
#include "dialognetworkset.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initdb();
}

void MainWindow::initdb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString curPath = QCoreApplication::applicationDirPath();
    QString dbFileName = curPath + "/acme.db";
    db.setDatabaseName(dbFileName);
    if(!db.open()){
        qDebug()<<"open database failed";
    }else {
        qDebug()<<"open database success :"<<dbFileName;
    }
    QSqlQuery query;
    QString sql = "select * from projectTab";
    if(!query.exec(sql)){
        sql = "create table projectTab("
                      "name text,"
                      "fileName text"
                      ")";
        if(!query.exec(sql)){
            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            return;
        }
    }

    sql = "select * from serviceTab";
    if(!query.exec(sql)){
        sql = "create table serviceTab("
                      "name text,"
                      "projectName text"
                      ")";
        if(!query.exec(sql)){
            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            return;
        }
    }

    sql = "select * from instrumentTab";
    if(!query.exec(sql)){
        sql = "create table instrumentTab("
                      "name text,"
                      "serviceName text,"
                      "projectName text"
                      ")";
        if(!query.exec(sql)){
            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            return;
        }
    }

    sql = "select * from pluginTab";
    if(!query.exec(sql)){
        sql = "create table pluginTab("
                      "name text,"
                      "type int,"
                      "fileName text"
                      ")";
        if(!query.exec(sql)){
            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            return;
        }
    }
}



void MainWindow::initNetwork()
{
//    clientFront = new QTcpSocket(this);
//    connect(clientFront, SIGNAL(connected()), this, SLOT(onConnectFrontServer()));
//    connect(clientFront, SIGNAL(disconnected()), this, SLOT(onDisConnectFrontServer()));
//    connect(clientFront,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//            this,SLOT(onSocketConnectFrontServerStateChange(QAbstractSocket::SocketState)));
//    connect(clientFront,SIGNAL(readyRead()),
//            this,SLOT(onSocketConnectFrontServerReadyRead()));

//    clientDataCenter = new QTcpSocket(this);
//    connect(clientDataCenter, SIGNAL(connected()), this, SLOT(onConnectDataCenter()));
//    connect(clientDataCenter, SIGNAL(disconnected()), this, SLOT(onDisConnectDataCenter()));
//    connect(clientDataCenter,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//            this,SLOT(onDataCenterStateChange(QAbstractSocket::SocketState)));
//    connect(clientDataCenter,SIGNAL(readyRead()),
//            this,SLOT(onDataCenterReadyRead()));

//    serverAgent = new QTcpSocket(this);
//    connect(serverAgent, SIGNAL(connected()), this, SLOT(onConnectDataCenter()));
//    connect(serverAgent, SIGNAL(disconnected()), this, SLOT(onDisConnectDataCenter()));
//    connect(serverAgent,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//            this,SLOT(onDataCenterStateChange(QAbstractSocket::SocketState)));
//    connect(serverAgent,SIGNAL(readyRead()),
//            this,SLOT(onDataCenterReadyRead()));

//    clientA12 = new QTcpSocket(this);
//    connect(clientA12, SIGNAL(connected()), this, SLOT(onConnectDataCenter()));
//    connect(clientA12, SIGNAL(disconnected()), this, SLOT(onDisConnectDataCenter()));
//    connect(clientA12,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//            this,SLOT(onDataCenterStateChange(QAbstractSocket::SocketState)));
//    connect(clientA12,SIGNAL(readyRead()),
//            this,SLOT(onDataCenterReadyRead()));
}




MainWindow::~MainWindow()
{
    delete ui;

    if(db.isOpen()){
        db.commit();
        db.close();
    }

    if(clientFront->isValid()){
        delete clientFront;
    }

    if(clientDataCenter->isValid()){
        delete clientDataCenter;
    }
    if(serverAgent->isValid()){
        delete serverAgent;
    }

    if(clientA12->isValid()){
        delete clientA12;
    }
}



void MainWindow::on_treeWidgetProject_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidgetProject->itemAt(pos);
    QMenu *m = new QMenu(this);
    if(item == nullptr)
    {
        qDebug()<<pos;
        m->addAction(ui->actionNewProject);
        m->addAction(ui->actionOpenProject);
        m->exec(QCursor::pos());
        return;
    }
    int itemType = item->type();
    qDebug()<<item;
    qDebug()<<"item type = "<<itemType;
    switch (itemType) {
    case projectLevel:
        m->addAction(ui->actionCloseProject);
        m->addAction(ui->actionDelProject);
        m->addAction(ui->actionNewService);
        break;
    case serviceLevel:
        m->addAction(ui->actionNewInstrument);
        m->addAction(ui->actionDelService);
        break;
    case instrumentLevel:
        m->addAction(ui->actionDelInstrument);
        break;
    default:
        return;
    }
    m->exec(QCursor::pos());
}


void MainWindow::on_actionNewPlugin_triggered()
{
    DialogNewPlugin *d = new DialogNewPlugin(this);
    Qt::WindowFlags flags = d->windowFlags();
    d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    int ret = d->exec();
    if (ret != QDialog::Accepted){
        delete d;
        return;
    }
    int pluginTypeIndex = d->getPluginTypeIndex();
    QString pluginName = d->getPluginName();
    QString pluginFileName = d->getPluginFileName();
    delete d;

    //保存插件数据
    QString curPath = QCoreApplication::applicationDirPath(); //获取应用程序的路径
//    QString fileName = curPath;
//    switch (pluginTypeIndex) {
//    case pluginTypeIngestor:
//        fileName += "/ingestor";
//        break;
//    case pluginTypeProcessor:
//        fileName += "/processor";
//        break;
//    case pluginTypeDumper:
//        fileName += "/dumper";
//        break;
//    case pluginTypeCommander:
//        fileName += "/commander";
//        break;
//    case pluginTypeDescriptor:
//        fileName += "/descriptor";
//        break;
//    default:
//        return;
//    }
//    QDir pluginDir(fileName);
//    if(!pluginDir.exists())
//    {
//        qDebug()<<"目录不存在，创建目录";
//        pluginDir.mkdir(fileName);
//    }

    //拷贝插件文件到当前目录
//    QFileInfo info = QFileInfo(pluginFileName);
//    QString shortName = info.fileName();
//    qDebug()<<pluginFileName;
//    qDebug()<<curPath + "/" + shortName;
    QString newPluginFileName = curPath + "/" + QFileInfo(pluginFileName).fileName();
    QFile::copy(pluginFileName, newPluginFileName);

    QSqlQuery query;
    QString sql = "insert into pluginTab values(?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(pluginName);
    query.addBindValue(pluginTypeIndex);
    query.addBindValue(newPluginFileName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
    }


    //    sql = "select * from pluginTab";
    //    if(!query.exec(sql)){
    //        qDebug()<<sql<<query.lastError().text();
    //        return;
    //    }
    //    while(query.next()){
    //        if(query.isValid()){
    //            QString name = query.value(0).toString();
    //            int type = query.value(1).toInt();
    //            qDebug()<<"name="<<name<<"type="<<type;
    //        }
    //    }

}

void MainWindow::on_actionOpenProject_triggered()
{
    QString curPath = QCoreApplication::applicationDirPath();
    QString caption = tr("打开工程");
    QString filter = tr("工程文件(*.pp)");
    QString projectFlileName = QFileDialog::getOpenFileName(this, caption, curPath, filter);
    qDebug()<<"打开工程文件名"<<projectFlileName;
    if(projectFlileName.isEmpty()){
        return;
    }
    QFile aFile(projectFlileName);
    if (!(aFile.open(QIODevice::ReadOnly)))
    {
        qDebug()<<"打开文件失败";
        return;
    }
    QDataStream aStream(&aFile);
    aStream.setVersion(QDataStream::Qt_5_9);
    QString projectName;
    aStream>>projectName;
    //    QString well_name;
    //    aStream>>well_name;
    //    QString task_name;
    //    aStream>>task_name;
    //    QString task_type;
    //    aStream>>task_type;
    //    QString task_content;
    //    aStream>>task_content;
    //    QString fileName;
    //    aStream>>fileName;
    aFile.close();

    QTreeWidgetItem* item = new QTreeWidgetItem(projectLevel);
    item->setText(projectColumn, projectName);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    //添加顶层节点
    ui->treeWidgetProject->addTopLevelItem(item);

    QSqlQuery query;
    QString sql = "select * from serviceTab where projectName = ?";
    query.prepare(sql);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
        return;
    }
    while (query.next()) {
        if(query.isValid()){
            QString serviceName = query.value(0).toString();
            QTreeWidgetItem* serviceItem = new QTreeWidgetItem(serviceLevel);
            serviceItem->setText(projectColumn, serviceName);
            serviceItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
            item->addChild(serviceItem);


            QSqlQuery instrumentQuery;
            sql = "select * from instrumentTab where projectName = ? and serviceName = ?";
            instrumentQuery.prepare(sql);
            instrumentQuery.addBindValue(projectName);
            instrumentQuery.addBindValue(serviceName);
            if(!instrumentQuery.exec()){
                qDebug()<<sql<<instrumentQuery.lastError().text();
                continue;
            }
            while (instrumentQuery.next()) {
                if(instrumentQuery.isValid()){
                    QString instrumenName = instrumentQuery.value(0).toString();
                    QTreeWidgetItem* instrumenItem = new QTreeWidgetItem(instrumentLevel);
                    instrumenItem->setText(projectColumn, serviceName);
                    instrumenItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
                    serviceItem->addChild(instrumenItem);
                }
            }
        }
    }


}

void MainWindow::on_actionNewProject_triggered()
{
    DialogCreateProject *d = new DialogCreateProject(this);
    Qt::WindowFlags flags = d->windowFlags();
    d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    int ret = d->exec();
    //OK键被按下,对话框关闭，若设置了setAttribute(Qt::WA_DeleteOnClose)，对话框被释放，无法获得返回值
    if (ret != QDialog::Accepted){
        delete d;
        return;
    }
    QString projectName  = d->get_project_name();
    QString wellName     = d->get_well_name();
    QString taskName     = d->get_task_name();
    QString taskType     = d->get_task_type();
    QString taskContent  = d->get_task_content();
    QString saveLocation = d->get_save_location();
    delete d; //删除对话框

    QSqlQuery query;
    QString sql = "insert into projectTab values(?, ?)";
    query.prepare(sql);
    query.addBindValue(projectName);
    query.addBindValue(saveLocation);
    if(!query.exec()){
        qDebug()<<"insert projectTab item failed";
        return;
    }


    //保存工程基本数据
    QFile aFile(saveLocation);
    if (!(aFile.open(QIODevice::NewOnly | QIODevice::WriteOnly | QIODevice::Truncate)))
    {
        qDebug()<<"打开文件失败";
        return;
    }

    QDataStream aStream(&aFile);
    aStream.setVersion(QDataStream::Qt_5_9); //设置版本号，写入和读取的版本号要兼容
    aStream<<projectName;
    aFile.close();

    QTreeWidgetItem* item = new QTreeWidgetItem(projectLevel);
    item->setText(0, projectName); //设置第1列的文字
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    //添加顶层节点
    ui->treeWidgetProject->addTopLevelItem(item);
}



void MainWindow::on_actionCloseProject_triggered()
{
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr) return;
    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem == nullptr){
        qDebug()<<"关闭工程";
        ui->treeWidgetProject->removeItemWidget(item, 0);
        delete item;
    }else {
        qDebug()<<"当前条目非工程";
        //        parentItem->removeChild(item);
    }

}

void MainWindow::on_actionDelProject_triggered()
{
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr) return;
    qDebug()<<"删除工程";

    QString projectName = item->text(0);
    qDebug()<<"project name = "<<projectName;

    QSqlQuery query;
    QString sql = "select * from projectTab where name = ?";
    query.prepare(sql);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
    }
    QString projectFileName;
    while (query.next()) {
        if(query.isValid()){
            projectFileName = query.value(1).toString();
            break;
        }
    }
    if(!projectFileName.isEmpty()){
        QFile::remove(projectFileName);
    }

    sql = "delete from projectTab where name = ?";
    query.prepare(sql);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
    }

    sql = "delete from serviceTab where projectName = ?";
    query.prepare(sql);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
    }

//    sql = "delete from instrumentTab where projectName = ?";
    sql = "delete from instrumentTab where projectName = ?";
    query.prepare(sql);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
    }
    //删除工程条目
    ui->treeWidgetProject->removeItemWidget(item, 0);
    delete item;
}

void MainWindow::on_actionDelService_triggered()
{
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr) return;
    QString service = item->text(0);
    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem == nullptr){
        return;
    }
    parentItem->removeChild(item);

    QString projectName = parentItem->text(0);
    //    int size = project.size();
    //    for(int i = 0; i < size; i++){
    //        if(project[i].getProjectName() == projectName){
    //            project[i].delService(service);
    //            break;
    //        }
    //    }
    QSqlQuery query;
    QString sql = "delete from serviceTab where name = ? and projectName = ?";
    query.prepare(sql);
    query.addBindValue(service);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<"delete serviceTab item failed";
        return;
    }

    sql = "delete from instrumentTab where serviceName = ?";
    query.prepare(sql);
    query.addBindValue(service);
    if(!query.exec()){
        qDebug()<<"delete instrumentTab item failed";
        return;
    }
}

void MainWindow::on_actionNewService_triggered()
{
    bool ok;
    QString service = QInputDialog::getText(this, tr("新建服务"), tr("服务名："),
                                            QLineEdit::Normal, tr(""), &ok);
    if(!ok || service.isEmpty()){
        return;
    }
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr) return;
    QTreeWidgetItem *child = new QTreeWidgetItem(serviceLevel);
    child->setText(0, service);
    child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->addChild(child);

    QString projectName = item->text(0);
    //    QString fileName;
    //    int size = project.size();
    //    for(int i = 0; i < size; i++){
    //        if(project[i].getProjectName() == projectName){
    //            project[i].addService(service);
    //            break;
    //        }
    //    }

    QSqlQuery query;
    QString sql = "insert into serviceTab values(?, ?)";
    query.prepare(sql);
    query.addBindValue(service);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<"insert serviceTab failed";
        qDebug()<<query.lastError().text();
        return;
    }

}


void MainWindow::on_actionDelInstrument_triggered()
{
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr) return;
    QString instrumentName = item->text(0);
    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem == nullptr){
        return;
    }
    QTreeWidgetItem *ppItem = parentItem->parent();
    if(ppItem == nullptr){
        return;
    }
    QString serviceName = parentItem->text(0);
    QString projectName = ppItem->text(0);
    parentItem->removeChild(item);

    //    QString projectName = parentItem->text(0);
    //    int size = ins.size();
    //    for(int i = 0; i < size; i++){
    //        if(project[i].getProjectName() == projectName){
    //            project[i].delService(service);
    //            break;
    //        }
    //    }

    QSqlQuery query;
//    QString sql = "delete from instrumentTab where name = ? and serviceName = ? and projectName = ?";
    QString sql = "delete from instrumentTab where name = ?";
    query.prepare(sql);
    query.addBindValue(instrumentName);
//    query.addBindValue(serviceName);
//    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<"delete instrumentTab item failed"<<query.lastError().text();
        return;
    }


}


void MainWindow::on_actionNewInstrument_triggered()
{
    QSqlQuery query;
    QString sql = "select * from pluginTab";
    if(!query.exec(sql)){
        qDebug()<<sql<<query.lastError().text();
        return;
    }
    QList<AcmePlugin> plugins;
    while(query.next()){
        if(query.isValid()){
            QString name = query.value(0).toString();
            int type = query.value(1).toInt();
            AcmePlugin p(name, type);
            plugins<<p;
        }
    }


    DialogNewInstrument *d = new DialogNewInstrument(plugins, this);
    Qt::WindowFlags flags = d->windowFlags();
    d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    int ret = d->exec();
    if (ret != QDialog::Accepted){
        return;
    }

    //    QString curPath = QCoreApplication::applicationDirPath();
    //    QString instrumentFile = curPath + "/instruments.dat";
    //    QFile aFile(instrumentFile);
    //    if (!(aFile.open(QIODevice::NewOnly | QIODevice::Append))){
    //        delete d;
    //        return;
    //    }

    QString instrumentName = d->getInstrumentName();
    QString processorName  = d->getProcessorName();
    QString dumperName     = d->getDumperName();
    QString commander      = d->getCommanderName();
    QString descriptor     = d->getDescriptorName();

    //    QDataStream aStream(&aFile);
    //    aStream.setVersion(QDataStream::Qt_5_9); //设置版本号，写入和读取的版本号要兼容
    //    aStream<<instrumentName;
    //    aStream<<d->getIngestorName();
    //    aStream<<d->getProcessorName();
    //    aStream<<d->getDumperName();
    //    aStream<<d->getcommanderName();
    //    aStream<<d->getDescriptorName();
    //    aFile.close();

    delete d; //删除对话框
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr){
        return;
    }
    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem == nullptr){
        return;
    }
    QString serviceName = item->text(0);
    QString projectName = parentItem->text(0);

    QTreeWidgetItem *child = new QTreeWidgetItem(instrumentLevel);
    child->setText(0, instrumentName);
    child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->addChild(child);




    sql = "insert into instrumentTab values(?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(instrumentName);
    query.addBindValue(serviceName);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<"insert instrumentTab item failed "<<query.lastError().text();
        return;
    }

}


void MainWindow::on_actionDelPlugin_triggered()
{
    DialogDelPlugin *d = new DialogDelPlugin(this);
    Qt::WindowFlags flags = d->windowFlags();
    d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    d->exec();
    delete d;
}

void MainWindow::on_actionNetworkSet_triggered()
{
    DialogNetworkSet *d = new DialogNetworkSet(this);
    Qt::WindowFlags flags = d->windowFlags();
    d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    d->exec();
    delete d;
}
