#ifndef DIALOGNEWINSTRUMENT_H
#define DIALOGNEWINSTRUMENT_H


#include <QDialog>

namespace Ui {
class DialogNewInstrument;
}

class DialogNewInstrument : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewInstrument(QWidget *parent = nullptr);
    ~DialogNewInstrument();

    void initLanguage();
    void initExistInstrument();

//    QString getIngestorName();
//    QString getDumperName();
//    QString getProcessorName();
//    QString getCommanderName();
//    QString getDescriptorName();
    QString getInstrumentName();

private slots:
    void on_pushButtonOK_clicked();

private:
    Ui::DialogNewInstrument *ui;
//    QString instrumentName;
};

#endif // DIALOGNEWINSTRUMENT_H
