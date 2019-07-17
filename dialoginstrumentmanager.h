#ifndef DIALOGINSTRUMENTMANAGER_H
#define DIALOGINSTRUMENTMANAGER_H

#include <QDialog>

namespace Ui {
class DialogInstrumentManager;
}

class DialogInstrumentManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInstrumentManager(QWidget *parent = nullptr);
    ~DialogInstrumentManager();

    void initLanguage();
    void initTable();
    void initTableRow(int row, QString name, QString serviceName, QString projectName);
    void initProjectComboBox();
    void delInstrumentFromDB(QString instrumentName, QString serviceName, QString projectName);

private slots:
    void on_comboBoxProject_currentIndexChanged(const QString &arg1);

    void on_pushButtonDoAdd_clicked();

    void on_pushButtonDoDelete_clicked();

private:
    Ui::DialogInstrumentManager *ui;
};

#endif // DIALOGINSTRUMENTMANAGER_H
