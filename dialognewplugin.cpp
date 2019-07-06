#include "dialognewplugin.h"
#include "mainwindow.h"
#include "ui_dialognewplugin.h"


#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>
#include <ui_mainwindow.h>


DialogNewPlugin::DialogNewPlugin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewPlugin)
{
    ui->setupUi(this);
    ui->textEditPluginFileName->setReadOnly(true);
}

DialogNewPlugin::~DialogNewPlugin()
{
    delete ui;
}


int DialogNewPlugin::getPluginTypeIndex()
{
    return ui->comboBoxPluginType->currentIndex();
}
QString DialogNewPlugin::getPluginName()
{
    return ui->lineEditPluginName->text();
}
QString DialogNewPlugin::getPluginFileName()
{
    return ui->textEditPluginFileName->toPlainText();
}

void DialogNewPlugin::on_pushButtonBrowse_clicked()
{
    QString curPath = QCoreApplication::applicationDirPath(); //获取应用程序的路径
    QString title = tr("选择插件文件"); //对话框标题
    QString filter = tr("插件库(*.dll);;所有文件(*.*)"); //文件过滤器
    QString fileName = QFileDialog::getOpenFileName(this, title, curPath, filter);
    if(!fileName.isEmpty())
    {
        ui->textEditPluginFileName->setText(fileName);
    }
}


//void DialogNewPlugin::on_lineEditPluginName_editingFinished()
//{
//    int pluginTypeIndex = getPluginTypeIndex();
//    QString pluginName = getPluginName();

//    if(pluginName.isEmpty())
//    {
//        QMessageBox::warning(this, tr("警告"), tr("插件名不能为空"));
//        return;
//    }

//    QString curPath = QCoreApplication::applicationDirPath(); //获取应用程序的路径
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
//    fileName += "/";
//    fileName += pluginName;
//    QFile aFile(fileName);
//    if(aFile.exists())
//    {
//        QMessageBox::warning(this, tr("警告"), tr("插件名重复"));
//    }
//}
