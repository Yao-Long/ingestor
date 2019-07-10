#ifndef DIALOGNEWPLUGIN_H
#define DIALOGNEWPLUGIN_H

#include <QDialog>

namespace Ui {
class DialogNewPlugin;
}


enum  pluginType
{
    pluginTypeIngestor, pluginTypeProcessor,
    pluginTypeDumper,   pluginTypeCommander,
    pluginTypeDescriptor,
};

class DialogNewPlugin : public QDialog
{
    Q_OBJECT



public:
    explicit DialogNewPlugin(QWidget *parent = nullptr);
    ~DialogNewPlugin();

    void initLanguage();

    int getPluginTypeIndex();
    QString getPluginName();
    QString getPluginFileName();


    static QString pluginTypeToString(int type){
        QString ret;
        switch (type) {
        case pluginTypeIngestor:
            return tr("数据前摄器");
        case pluginTypeProcessor:
            return tr("数据处理器");
        case pluginTypeDumper:
            return tr("数据导出器");
        case pluginTypeCommander:
            return tr("命令适配器");
        case pluginTypeDescriptor:
            return tr("仪器描述器");
        default:
            return tr("");
        }
    }

private slots:
    void on_pushButtonBrowse_clicked();

//    void on_lineEditPluginName_editingFinished();

    void on_pushButtonAdd_clicked();

private:
    Ui::DialogNewPlugin *ui;
};

#endif // DIALOGNEWPLUGIN_H
