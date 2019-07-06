#ifndef DIALOGDELPLUGIN_H
#define DIALOGDELPLUGIN_H

#include <QDialog>

namespace Ui {
class DialogDelPlugin;
}

class DialogDelPlugin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDelPlugin(QWidget *parent = nullptr);
    ~DialogDelPlugin();

    void initPluginTable();
    void initPluginTableRow(int pluginRow, int pluginType, QString pluginName, QString pluginFileName);

    //  自定义单元格Type的类型，在创建单元格的Item时使用
    enum  CellType{ctPluginName = 1000, ctPluginType, ctPluginFileName, ctIsDel}; //各单元格的类型

    //  各字段在表格中的列号
    enum  FieldColNum{colPluginName, colPluginType, colPluginFileName, colIsDel};

    void delPluginFromDB(QString pluginName);
    void delPluginFromSystem(QString pluginName);

private slots:
    void on_pushButtonDoDel_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::DialogDelPlugin *ui;
};

#endif // DIALOGDELPLUGIN_H
