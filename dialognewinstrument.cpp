#include "dialognewinstrument.h"
#include "ui_dialognewinstrument.h"



#include "dialognewplugin.h"

DialogNewInstrument::DialogNewInstrument(QList<AcmePlugin> plugins, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewInstrument)
{
    ui->setupUi(this);
    int size = plugins.size();
    for(int i = 0; i < size; i++){
        switch(plugins[i].getType()){
        case pluginTypeIngestor:
            ui->comboBoxIngestor->addItem(plugins[i].getName());
            break;
        case pluginTypeProcessor:
            ui->comboBoxProcessor->addItem(plugins[i].getName());
            break;
        case pluginTypeDumper:
            ui->comboBoxDumper->addItem(plugins[i].getName());
            break;
        case pluginTypeCommander:
            ui->comboBoxCommander->addItem(plugins[i].getName());
            break;
        case pluginTypeDescriptor:
            ui->comboBoxDescriptor->addItem(plugins[i].getName());
            break;
        default:
            break;
        }
    }
}





DialogNewInstrument::~DialogNewInstrument()
{
    delete ui;
}


QString DialogNewInstrument::getIngestorName(){
    return ui->comboBoxIngestor->currentText();
}

QString DialogNewInstrument::getDumperName(){
    return ui->comboBoxDumper->currentText();
}

QString DialogNewInstrument::getProcessorName(){
    return ui->comboBoxProcessor->currentText();
}

QString DialogNewInstrument::getCommanderName(){
    return ui->comboBoxCommander->currentText();
}

QString DialogNewInstrument::getDescriptorName(){
    return ui->comboBoxDescriptor->currentText();
}

QString DialogNewInstrument::getInstrumentName(){
    return ui->lineEditInstrumentName->text();
}

