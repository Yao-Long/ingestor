#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QSqlDataBase>
#include <QSqlTableModel>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}



//项目树视图层级
enum projectTreeLevel
{
    projectLevel = 1001,
    serviceLevel,
    instrumentLevel,
};
//项目树列编号
enum projectTreeColumn
{
    projectColumn,
    serviceColumn,
    instrumentColumn,
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initdb();
    void initNetwork();
private slots:

    void on_treeWidgetProject_customContextMenuRequested(const QPoint &pos);

    void on_actionNewPlugin_triggered();

    void on_actionOpenProject_triggered();

    void on_actionNewProject_triggered();

    void on_actionNewInstrument_triggered();

    void on_actionCloseProject_triggered();

    void on_actionDelProject_triggered();

    void on_actionDelService_triggered();

    void on_actionDelInstrument_triggered();

    void on_actionNewService_triggered();

    void on_actionDelPlugin_triggered();

    void on_actionNetworkSet_triggered();



    void onConnectFrontServer();
    void onDisConnectFrontServer();
    void onSocketConnectFrontServerStateChange(QAbstractSocket::SocketState socketState);
    void onSocketConnectFrontServerReadyRead();

private:
    Ui::MainWindow *ui;
//    QList<AcmeProject> project;
//    QList<AcmePlugin> plugins;
    QSqlDatabase db;
//    QSqlTableModel *tabModel;  //数据模型
//    QSqlQueryModel *queryModel;

    QTcpSocket *clientFront;
    QTcpSocket *clientDataCenter;
    QTcpSocket *serverAgent;
    QTcpSocket *clientA12;
};

#endif // MAINWINDOW_H
