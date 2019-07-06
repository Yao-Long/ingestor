#include "dialogcreateproject.h"
#include "ui_dialogcreateproject.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>

DialogCreateProject::DialogCreateProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreateProject)
{
    ui->setupUi(this);
}

DialogCreateProject::~DialogCreateProject()
{
    delete ui;
}


QString DialogCreateProject::get_project_name()
{
    return ui->lineEditProjectName->text();

}
QString DialogCreateProject::get_well_name()
{
    return ui->lineEditWellName->text();
}
QString DialogCreateProject::get_task_name()
{
    return ui->lineEditTaskName->text();
}
QString DialogCreateProject::get_task_type()
{
    return ui->lineEditTaskType->text();
}
QString DialogCreateProject::get_task_content()
{
    return ui->lineEditTaskContent->text();
}
QString DialogCreateProject::get_save_location()
{
    return ui->textEditSaveLocation->toPlainText();
}



void DialogCreateProject::on_pushButtonBrowse_clicked()
{
    QString curPath = QCoreApplication::applicationDirPath(); //获取应用程序的路径
    QString title = tr("选择保存目录"); //对话框标题
//    QString filter = tr("所有文件(*.*)"); //文件过滤器
//    QString fileName = QFileDialog::getSaveFileName(this, d, curPath, filter);
    QString dir=QFileDialog::getExistingDirectory(this, title, curPath, QFileDialog::ShowDirsOnly);
//    qDebug()<<dir;
//    ui->comboBox_save_location->setCurrentText("fileName");
    if (!dir.isEmpty())
    {
        QString fileName = dir + "/" + get_project_name() + ".pp";
        qDebug()<<fileName;
        ui->textEditSaveLocation->setText(fileName);
//        ui->comboBox_save_location->setEditable(false);
    }
}

//void DialogCreateProject::on_lineEdit_project_name_editingFinished()
//{
//    QString projectName = get_project_name();
//    if(projectName.isEmpty()){
//        QMessageBox::warning(this, tr("警告"), tr("工程名不能为空"));
//        return;
//    }
//}
