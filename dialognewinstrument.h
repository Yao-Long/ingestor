#ifndef DIALOGNEWINSTRUMENT_H
#define DIALOGNEWINSTRUMENT_H

#include "acmeplugin.h"

#include <QDialog>

namespace Ui {
class DialogNewInstrument;
}

class DialogNewInstrument : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewInstrument(QList<AcmePlugin> plugins, QWidget *parent = nullptr);
    ~DialogNewInstrument();

    void initLanguage();

    QString getIngestorName();
    QString getDumperName();
    QString getProcessorName();
    QString getCommanderName();
    QString getDescriptorName();
    QString getInstrumentName();

private slots:
    void on_pushButtonOK_clicked();

private:
    Ui::DialogNewInstrument *ui;
    QString instrumentName;
};

#endif // DIALOGNEWINSTRUMENT_H
