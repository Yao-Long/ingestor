#include "dialognewinstrument.h"
#include "ui_dialognewinstrument.h"



#include "dialognewplugin.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>

DialogNewInstrument::DialogNewInstrument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewInstrument)
{
    ui->setupUi(this);
    initLanguage();
    initExistInstrument();
}





DialogNewInstrument::~DialogNewInstrument()
{
    delete ui;
}

extern QTranslator *trans;


void DialogNewInstrument::initLanguage()
{
    qApp->installTranslator(trans);
    ui->retranslateUi(this);
}

void DialogNewInstrument::initExistInstrument(){
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
            switch(type){
            case pluginTypeIngestor:
                ui->comboBoxIngestor->addItem(name);
                break;
            case pluginTypeProcessor:
                ui->comboBoxProcessor->addItem(name);
                break;
            case pluginTypeDumper:
                ui->comboBoxDumper->addItem(name);
                break;
            case pluginTypeCommander:
                ui->comboBoxCommander->addItem(name);
                break;
            case pluginTypeDescriptor:
                ui->comboBoxDescriptor->addItem(name);
                break;
            default:
                break;
            }
        }
    }
}



//QString DialogNewInstrument::getIngestorName(){
//    return ui->comboBoxIngestor->currentText();
//}

//QString DialogNewInstrument::getDumperName(){
//    return ui->comboBoxDumper->currentText();
//}

//QString DialogNewInstrument::getProcessorName(){
//    return ui->comboBoxProcessor->currentText();
//}

//QString DialogNewInstrument::getCommanderName(){
//    return ui->comboBoxCommander->currentText();
//}

//QString DialogNewInstrument::getDescriptorName(){
//    return ui->comboBoxDescriptor->currentText();
//}

QString DialogNewInstrument::getInstrumentName(){
    return ui->lineEditInstrumentName->text();
}


void DialogNewInstrument::on_pushButtonOK_clicked()
{
    if(ui->lineEditInstrumentName->text().isEmpty()){
        QMessageBox::information(this, tr("提示消息"), tr("仪器名不能为空！"));
        return;
    }
    this->accept();
}
