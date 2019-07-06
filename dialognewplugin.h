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

//    enum    pluginType
//    {pluginTypeIngestor, pluginTypeProcessor,
//    pluginTypeDumper, pluginTypeCommander, pluginTypeDescriptor};

public:
    explicit DialogNewPlugin(QWidget *parent = nullptr);
    ~DialogNewPlugin();
    int getPluginTypeIndex();
    QString getPluginName();
    QString getPluginFileName();

private slots:
    void on_pushButtonBrowse_clicked();

    void on_lineEdit_plugin_name_editingFinished();

private:
    Ui::DialogNewPlugin *ui;
};

#endif // DIALOGNEWPLUGIN_H
