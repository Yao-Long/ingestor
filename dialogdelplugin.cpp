#include "dialogdelplugin.h"
#include "dialognewplugin.h"
#include "ui_dialogdelplugin.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QFileInfo>

DialogDelPlugin::DialogDelPlugin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDelPlugin)
{
    ui->setupUi(this);

    initPluginTable();


}

DialogDelPlugin::~DialogDelPlugin()
{
    ui->tableWidget->clear();
    delete ui;
}



void DialogDelPlugin::initPluginTableRow(int pluginRow, int plugintype, QString pluginName, QString pluginFileName)
{
    QTableWidgetItem *item;
    QString str;

    item = new QTableWidgetItem(pluginName, ctPluginName);
    Qt::ItemFlags flag = item->flags();
    //不可编辑
    item->setFlags(flag & ~Qt::ItemIsEditable);
    //文本对齐格式
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(pluginRow, colPluginName, item);

    item = new QTableWidgetItem(DialogNewPlugin::pluginTypeToString(plugintype), ctPluginType);
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(pluginRow, colPluginType, item);

    item = new QTableWidgetItem(pluginFileName, ctPluginFileName);
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(pluginRow, colPluginFileName, item);

    item = new QTableWidgetItem(tr("否"), ctIsDel);
    item->setFlags(flag & ~Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    item->setCheckState(Qt::Unchecked);
    //设置背景颜色
    item->setBackgroundColor(Qt::green);
    ui->tableWidget->setItem(pluginRow, colIsDel, item);
}


void DialogDelPlugin::initPluginTable()
{
    //设置表头
    QTableWidgetItem *headerItem;
    QStringList headerText;
    headerText<<"插件名"<<"插件类型"<<"插件文件路径"<<"是否删除";
//    ui->tableWidget->setHorizontalHeaderLabels(headerText);
    int columnCount = headerText.count();
    ui->tableWidget->setColumnCount(columnCount);//列数设置为与 headerText的行数相等
    for (int i = 0; i < columnCount; i++)
    {
       headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
       QFont font=headerItem->font();//获取原有字体设置
       font.setBold(true);//设置为粗体
       font.setPointSize(12);//字体大小
//       headerItem->setTextColor(Qt::black);//字体颜色
       headerItem->setFont(font);//设置字体
       ui->tableWidget->setHorizontalHeaderItem(i, headerItem); //设置表头单元格的Item
    }

    //设置成行选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //初始化插件表数据
    QSqlQuery query;
    QString sql = "select * from pluginTab";
    if(!query.exec(sql)){
        qDebug()<<sql<<query.lastError().text();
        return;
    }
    while(query.next()){
        if(query.isValid()){
            QString name = query.value(0).toString();
            int type = query.value(1).toInt();
            QString fileName = query.value(2).toString();
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            initPluginTableRow(row, type, name, fileName);
        }
    }
    //自适应大小
//    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}

//把插件从插件表中删除
void DialogDelPlugin::delPluginFromDB(QString pluginName){

    QSqlQuery query;
    QString sql = "delete from pluginTab where name = ?";
    query.prepare(sql);
    query.addBindValue(pluginName);
    if(!query.exec()){
        qDebug()<<sql<<query.lastError().text();
    }
}


//删除系统中的插件文件
void DialogDelPlugin::delPluginFromSystem(QString pluginName){
    QSqlQuery query;
    QString sql = "select * from pluginTab while name = ?";
    query.prepare(sql);
    query.addBindValue(pluginName);
    if(!query.exec(sql)){
        qDebug()<<sql<<query.lastError().text();
        return;
    }
    QString pluginFileName;
    while(query.next()){
        if(query.isValid()){
            pluginFileName = query.value(2).toString();
            break;
        }
    }
    if(!pluginFileName.isEmpty()){
        //删除插件文件
        QFile::remove(pluginFileName);
    }
}


void DialogDelPlugin::on_pushButtonDoDel_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    for (int i = 0; i < rowCount;) {
        QTableWidgetItem *item = ui->tableWidget->takeItem(i, colIsDel);
        QString delText = item->text();
        //必须重设item
        ui->tableWidget->setItem(i, colIsDel, item);
        if(delText == tr("是")){
            QTableWidgetItem *nameItem = ui->tableWidget->takeItem(i, colPluginName);
            QString pluginName = nameItem->text();
            //必须重设item
            ui->tableWidget->setItem(i, colPluginName, nameItem);
            ui->tableWidget->removeRow(i);
            delPluginFromDB(pluginName);
            delPluginFromSystem(pluginName);
            rowCount--;

        }else {
            i++;
        }
    }
}

void DialogDelPlugin::on_tableWidget_cellDoubleClicked(int row, int column)
{
    qDebug()<<"row="<<row<<"column="<<column;
    QTableWidgetItem *item = ui->tableWidget->takeItem(row, colIsDel);
    QString delText = item->text();
    if(delText == tr("是")){
//        item->setCheckState(Qt::Unchecked);
        item->setBackgroundColor(Qt::green);
        item->setText("否");
    }else {
//        item->setCheckState(Qt::Checked);
        item->setBackgroundColor(Qt::red);
        item->setText("是");
    }
    ui->tableWidget->setItem(row, colIsDel, item);
}


