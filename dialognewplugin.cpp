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
    initLanguage();
    ui->textEditPluginFileName->setReadOnly(true);
}

DialogNewPlugin::~DialogNewPlugin()
{
    delete ui;
}


extern QTranslator *trans;


void DialogNewPlugin::initLanguage()
{
    qApp->installTranslator(trans);
    ui->retranslateUi(this);
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


void DialogNewPlugin::on_pushButtonAdd_clicked()
{
    if(ui->lineEditPluginName->text().isEmpty()){
        QMessageBox::information(this, tr("提示消息"), tr("插件名不能为空！"));
        return;
    }
    if(ui->textEditPluginFileName->toPlainText().isEmpty()){
        QMessageBox::information(this, tr("提示消息"), tr("插件路径不能为空！"));
        return;
    }
    this->accept();
}
