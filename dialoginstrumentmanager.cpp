#include "dialoginstrumentmanager.h"
#include "mainwindow.h"
#include "ui_dialoginstrumentmanager.h"

#include <QSqlError>
#include <QSqlQuery>

DialogInstrumentManager::DialogInstrumentManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInstrumentManager)
{
    ui->setupUi(this);
    initLanguage();
    initTable();
    initProjectComboBox();
}

DialogInstrumentManager::~DialogInstrumentManager()
{
    delete ui;
}



extern QTranslator *trans;
void DialogInstrumentManager::initLanguage()
{
    qApp->installTranslator(trans);
    ui->retranslateUi(this);
}

void DialogInstrumentManager::initTableRow(int row, QString name, QString serviceName, QString projectName)
{
    QTableWidgetItem *item;
    QString str;

    item = new QTableWidgetItem(name, 1000);
    Qt::ItemFlags flag = item->flags();
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget->setItem(row, 0, item);

    item = new QTableWidgetItem(serviceName, 1001);
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget->setItem(row, 1, item);

    item = new QTableWidgetItem(projectName, 1002);
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget->setItem(row, 2, item);

    item = new QTableWidgetItem(tr(""), 1003);
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignLeft);
    item->setCheckState(Qt::Unchecked);
    ui->tableWidget->setItem(row, 3, item);

    item = new QTableWidgetItem(tr(""), 1004);
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignLeft);
    item->setCheckState(Qt::Unchecked);
    ui->tableWidget->setItem(row, 4, item);
}


void DialogInstrumentManager::initTable()
{
    //设置表头
    QTableWidgetItem *headerItem;
    QStringList headerText;
    headerText<<tr("仪器名")<<tr("所属服务表")<<tr("所属工程")<<tr("是否添加")<<tr("是否删除");
    int columnCount = headerText.count();
    ui->tableWidget->setColumnCount(columnCount);
    for (int i = 0; i < columnCount; i++)
    {
       headerItem=new QTableWidgetItem(headerText.at(i));
       QFont font=headerItem->font();
       font.setBold(true);
       font.setPointSize(12);
       headerItem->setTextAlignment(Qt::AlignLeft);
       headerItem->setFont(font);
       QSize size(100, 40);
       headerItem->setSizeHint(size);
       ui->tableWidget->setHorizontalHeaderItem(i, headerItem);
    }

    QSqlQuery query;
    QString sql = "select * from instrumentTab";
    if(!query.exec(sql)){
        qDebug()<<sql<<query.lastError().text();
        return;
    }
    while(query.next()){
        if(query.isValid()){
            QString name = query.value(0).toString();
            QString serviceName = query.value(1).toString();
            QString projectName = query.value(2).toString();
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            initTableRow(row, name, serviceName, projectName);
        }
    }
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->resizeColumnsToContents();
}

void DialogInstrumentManager::initProjectComboBox(){
    QStringList projectNames;
    QSqlQuery query;
    QString sql = "select * from projectTab";
    query.prepare(sql);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
        return;
    }
    while (query.next()) {
        if(query.isValid()){
            QString projectName = query.value(0).toString();
            projectNames.append(projectName);
        }
    }
    ui->comboBoxProject->addItems(projectNames);
}

void DialogInstrumentManager::on_comboBoxProject_currentIndexChanged(const QString &arg1)
{
    ui->comboBoxService->clear();
    QWidget *parent = this->parentWidget();
    QStringList serviceNames = ((MainWindow *)parent)->getProjectServices(arg1);
    ui->comboBoxService->addItems(serviceNames);
}

void DialogInstrumentManager::on_pushButtonDoAdd_clicked()
{
    QString projectName = ui->comboBoxProject->currentText();
    QString serviceName = ui->comboBoxService->currentText();

    int rowCount = ui->tableWidget->rowCount();
    for (int i = 0; i < rowCount; i++) {
        QTableWidgetItem *item = ui->tableWidget->takeItem(i, 3);
        QString str = item->text();
        //必须重设item
        ui->tableWidget->setItem(i, 3, item);
        if(str == tr("是")){
            QTableWidgetItem *nameItem = ui->tableWidget->takeItem(i, 0);
            QString instrumentName = nameItem->text();
            //必须重设item
            ui->tableWidget->setItem(i, 0, nameItem);
            QWidget *parent = this->parentWidget();
            ((MainWindow *)parent)->addInstrumentToTree(projectName, serviceName, instrumentName);
        }
    }
}


void DialogInstrumentManager::delInstrumentFromDB(QString instrumentName, QString serviceName, QString projectName){
    QSqlQuery query;
    QString sql = "delete from instrumentTab where name = ? and serviceName = ? and projectName = ?";
    query.prepare(sql);
    query.addBindValue(instrumentName);
    query.addBindValue(serviceName);
    query.addBindValue(projectName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
    }
}

void DialogInstrumentManager::on_pushButtonDoDelete_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    for (int i = 0; i < rowCount;) {
        QTableWidgetItem *item = ui->tableWidget->item(i, 4);
        if(item->checkState() == Qt::Checked){
            QTableWidgetItem *nameItem = ui->tableWidget->item(i, 0);
            QString instrumentName = nameItem->text();
            QTableWidgetItem *serviceItem = ui->tableWidget->item(i, 1);
            QString serviceName = serviceItem->text();
            QTableWidgetItem *projectItem = ui->tableWidget->item(i, 2);
            QString projectName = projectItem->text();
            ui->tableWidget->removeRow(i);
            delInstrumentFromDB(instrumentName, serviceName, projectName);
            rowCount--;
        }else {
            i++;
        }
    }
}
