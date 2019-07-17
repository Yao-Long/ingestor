
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
#include <QKeyEvent>

#include "dialogcreateproject.h"
#include "dialognewplugin.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialognewinstrument.h"
#include "dialogdelplugin.h"
#include "dialognetworkset.h"
#include "dialoginstrumentmanager.h"



extern languageType langType;
extern QTranslator *trans;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSuccess = true;
    initdb();
    initNetwork();
    initLanguage();
    //初始化为全屏
    showFullScreen();
    isFullScreen = true;
}


void MainWindow::keyPressEvent(QKeyEvent *event){
    //按ESC快速切换全屏
    if(event->key() == Qt::Key_Escape){
        if(isFullScreen){
            showNormal();
            isFullScreen = false;
        }else {
            showFullScreen();
            isFullScreen = true;
        }
    }
}


void MainWindow::initLanguage()
{
    QSqlQuery query;
    QString sql = "select * from languageTab";
    if(!query.exec(sql)){
//        QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
        this->initSuccess = false;
        return;
    }
    int type = -1;
    while (query.next()) {
        if(query.isValid()){
            type = query.value(0).toInt();
            break;
        }
    }
    trans = new QTranslator;
    QString curPath = QCoreApplication::applicationDirPath();
    if(type == languageTypeEnglish){
        trans->load(curPath + "/gLanguage_en.qm");
        langType = languageTypeEnglish;
    }else{
        trans->load(curPath + "/gLanguage_cn.qm");
        langType = languageTypeChinese;
    }
    qApp->installTranslator(trans);
    ui->retranslateUi(this);
}

void MainWindow::initdb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString curPath = QCoreApplication::applicationDirPath();
    QString dbFileName = curPath + "/ingestor.db";
    db.setDatabaseName(dbFileName);
    if(!db.open()){
        qDebug()<<"open database failed";
        this->initSuccess = false;
        return;
    }
    QSqlQuery query;
    QString sql = "select * from projectTab";
    if(!query.exec(sql)){
        sql = "create table projectTab("
                      "name text,"
                      "fileName text"
                      ")";
        if(!query.exec(sql)){
//            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            this->initSuccess = false;
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
//            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            this->initSuccess = false;
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
//            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            this->initSuccess = false;
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
//            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            this->initSuccess = false;
            return;
        }
    }

    sql = "select * from networkTab";
    if(!query.exec(sql)){
        sql = "create table networkTab("
                      "frontIp text,"
                      "frontPort int,"
                      "dataCenterIp text,"
                      "dataCenterPort int,"
                      "localAgentIp text,"
                      "localAgentPort int,"
                      "a12Ip text,"
                      "a12TeamName text"
                      ")";
        if(!query.exec(sql)){
//            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            this->initSuccess = false;
            return;
        }
    }

    sql = "select * from languageTab";
    if(!query.exec(sql)){
        sql = "create table languageTab("
                      "language int"
                      ")";
        if(!query.exec(sql)){
//            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            this->initSuccess = false;
            return;
        }

        sql = "insert into languageTab values(?)";
        query.prepare(sql);
        query.addBindValue(languageTypeChinese);
        if(!query.exec()){
//            QMessageBox::critical(this, tr("错误信息"), sql + query.lastError().text());
            this->initSuccess = false;
            return;
        }
    }
}



void MainWindow::initNetwork()
{
    clientFront = new QTcpSocket(this);
    clientDataCenter = new QTcpSocket(this);
    serverAgent = new QTcpServer(this);
    clientA12 = new QTcpSocket(this);
    dialogNetworkSet = nullptr;
}




MainWindow::~MainWindow()
{
    delete ui;

    if(db.isOpen()){
        db.commit();
        db.close();
    }

    if(clientFront){
        delete clientFront;
    }

    if(clientDataCenter){
        delete clientDataCenter;
    }
    if(serverAgent){
        delete serverAgent;
    }

    if(clientA12){
        delete clientA12;
    }

    if(dialogNetworkSet){
        delete dialogNetworkSet;
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
    item->setText(0, projectName);
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
            serviceItem->setText(0, serviceName);
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
                    instrumenItem->setText(0, instrumenName);
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


//关闭当前工程
void MainWindow::on_actionCloseProject_triggered()
{
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr){
        qDebug()<<"item == nullptr";
        return;
    }
    if(item->type() != projectLevel){
        qDebug()<<"type != project";
        return;
    }
    ui->treeWidgetProject->removeItemWidget(item, 0);
    delete item;
}

//删除当前选中工程，同时删除所有工程的服务表和仪器
void MainWindow::on_actionDelProject_triggered()
{
    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
    if(item == nullptr){
        qDebug()<<"item == nullptr";
        return;
    }
    if(item->type() != projectLevel){
        qDebug()<<"type != project";
        return;
    }
    QString projectName = item->text(0);
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
//    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
//    if(item == nullptr){
//        QMessageBox::information(this, tr("提示消息"), tr("当前工程为空，请先创建工程！"));
//        return;
//    }
//    if(item->type() != projectLevel){
//        QMessageBox::information(this, tr("提示消息"), tr("当前选择项非工程类型，请选中服务所属工程！"));
//        return;
//    }

    bool ok;
    QString service = QInputDialog::getText(this, tr("新建服务"), tr("服务名："),
                                            QLineEdit::Normal, tr(""), &ok);
    if(!ok || service.isEmpty()){
        return;
    }

//    QTreeWidgetItem *child = new QTreeWidgetItem(serviceLevel);
//    child->setText(0, service);
//    child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
//    item->addChild(child);

//    QString projectName = item->text(0);

    QSqlQuery query;
    QString sql = "insert into serviceTab values(?, ?)";
    query.prepare(sql);
    query.addBindValue(service);
    query.addBindValue(tr(""));
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

    QSqlQuery query;
    QString sql = "delete from instrumentTab where name = ? and serviceName = ? and projectName = ?";
    query.prepare(sql);
    query.addBindValue(instrumentName);
    query.addBindValue(serviceName);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<"delete instrumentTab item failed"<<query.lastError().text();
        return;
    }


}

void MainWindow::addInstrumentToTree(QString projectName, QString serviceName, QString instrumentName){
    int projecCount = ui->treeWidgetProject->topLevelItemCount();
    for(int i = 0; i < projecCount; i++){
        QTreeWidgetItem *pItem = ui->treeWidgetProject->topLevelItem(i);
        if(pItem->text(0) == projectName){
            int serviceCount = pItem->childCount();
            for(int j = 0; j < serviceCount; j++){
                QTreeWidgetItem *sItem = pItem->child(j);
                if(sItem->text(0) == serviceName){
                    QTreeWidgetItem *instrument = new QTreeWidgetItem(instrumentLevel);
                    instrument->setText(0, instrumentName);
                    instrument->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
                    sItem->addChild(instrument);

                    //添加到数据库
                    QSqlQuery query;
                    QString sql = "insert into instrumentTab values(?, ?, ?)";
                    query.prepare(sql);
                    query.addBindValue(instrumentName);
                    query.addBindValue(serviceName);
                    query.addBindValue(projectName);
                    if(!query.exec()){
                        qDebug()<<sql<<query.lastError().text();
                        return;
                    }
                    return;
                }
            }
            break;
        }
    }
}


void MainWindow::on_actionNewInstrument_triggered()
{
//    QTreeWidgetItem *item = ui->treeWidgetProject->currentItem();
//    QTreeWidgetItem *parentItem;
    QString serviceName;
    QString projectName;
//    if(item != nullptr && item->type() == serviceLevel){
//        parentItem = item->parent();
//        if(parentItem != nullptr){
//            serviceName = item->text(0);
//            projectName = parentItem->text(0);
//        }
//    }

    DialogNewInstrument *d = new DialogNewInstrument(this);
    Qt::WindowFlags flags = d->windowFlags();
    d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    int ret = d->exec();
    if (ret != QDialog::Accepted){
        delete d;
        return;
    }
    QString instrumentName = d->getInstrumentName();
//    QString processorName  = d->getProcessorName();
//    QString dumperName     = d->getDumperName();
//    QString commander      = d->getCommanderName();
//    QString descriptor     = d->getDescriptorName();
    delete d;

//    if(!serviceName.isEmpty()){
//        QTreeWidgetItem *child = new QTreeWidgetItem(instrumentLevel);
//        child->setText(0, instrumentName);
//        child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
//        item->addChild(child);
//    }
    QSqlQuery query;
    QString sql = "insert into instrumentTab values(?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(instrumentName);
    query.addBindValue(serviceName);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
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
    if(this->dialogNetworkSet == nullptr)
    {
        NetWorkData networkData;
        networkData.clientA12 = this->clientA12;
        networkData.clientFront = this->clientFront;
        networkData.serverAgent = this->serverAgent;
        networkData.clientDataCenter = this->clientDataCenter;

        DialogNetworkSet *d = new DialogNetworkSet(&networkData, this);
        Qt::WindowFlags flags = d->windowFlags();
        d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
        this->dialogNetworkSet = d;
        d->exec();
    }else {
        this->dialogNetworkSet->initLanguage();
        this->dialogNetworkSet->exec();
    }
}


void MainWindow::on_actionSetLanguageCN_triggered()
{
    if(langType == languageTypeChinese){
        return;
    }
    qApp->removeTranslator(trans);
    delete trans;
    trans = new QTranslator;
    QString curPath = QCoreApplication::applicationDirPath();
    trans->load(curPath + "/gLanguage_cn.qm");
    qApp->installTranslator(trans);
    ui->retranslateUi(this);
    langType = languageTypeChinese;

    QSqlQuery query;
    QString sql = "update languageTab set language = ?";
    query.prepare(sql);
    query.addBindValue(langType);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
        return;
    }

}

void MainWindow::on_actionSetLanguageEN_triggered()
{
    if(langType == languageTypeEnglish){
        return;
    }
    qApp->removeTranslator(trans);
    delete trans;
    trans = new QTranslator;
    QString curPath = QCoreApplication::applicationDirPath();
    trans->load(curPath + "/gLanguage_en.qm");
    qApp->installTranslator(trans);
    ui->retranslateUi(this);
    langType = languageTypeEnglish;

    QSqlQuery query;
    QString sql = "update languageTab set language = ?";
    query.prepare(sql);
    query.addBindValue(langType);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
        return;
    }
}


QStringList MainWindow::getOpenedProjectNames(){
    QStringList list;
    int count = ui->treeWidgetProject->topLevelItemCount();
    for (int i = 0; i < count; i++) {
        QTreeWidgetItem *topItem = ui->treeWidgetProject->topLevelItem(i);
        QString projectName = topItem->text(0);
        list.append(projectName);
    }
    return list;
}

QStringList MainWindow::getProjectServices(QString projectName){
    QStringList list;
    int count = ui->treeWidgetProject->topLevelItemCount();
    for (int i = 0; i < count; i++) {
        QTreeWidgetItem *topItem = ui->treeWidgetProject->topLevelItem(i);
        QString name = topItem->text(0);
        if(name == projectName){
            int serviceCount = topItem->childCount();
            for (int j = 0; j < serviceCount; j++) {
                QTreeWidgetItem *serviceItem = topItem->child(j);
                if(serviceItem){
                    QString serviceName = serviceItem->text(0);
                    list.append(serviceName);
                }
            }
            break;
        }
    }
    return list;
}

void MainWindow::on_actionInstrumentManage_triggered()
{
    DialogInstrumentManager *d = new DialogInstrumentManager(this);
    Qt::WindowFlags flags = d->windowFlags();
    d->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    d->exec();
    delete d;
}
