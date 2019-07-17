#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include "dialognetworkset.h"

#include <QMainWindow>
//#include <QSqlDataBase>
#include <QSqlTableModel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTranslator>

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
//enum projectTreeColumn
//{
//    projectColumn,
//    serviceColumn,
//    instrumentColumn,
//};

enum languageType
{
    languageTypeEnglish = 100,
    languageTypeChinese,
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initdb();
    void initNetwork();
    void initLanguage();

    void keyPressEvent(QKeyEvent *event);

    void addInstrumentToTree(QString projectName, QString serviceName, QString instrumentName);


    QStringList getOpenedProjectNames();
    QStringList getProjectServices(QString projectName);

    bool initSuccess;
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



//    void onConnectFrontServer();
//    void onDisConnectFrontServer();
//    void onFrontServerStateChange(QAbstractSocket::SocketState socketState);
//    void onSocketConnectFrontServerReadyRead();

    void on_actionSetLanguageCN_triggered();

    void on_actionSetLanguageEN_triggered();

    void on_actionInstrumentManage_triggered();

private:
    bool isFullScreen;


    Ui::MainWindow *ui;
//    QList<AcmeProject> project;
//    QList<AcmePlugin> plugins;
    QSqlDatabase db;
//    QSqlTableModel *tabModel;  //数据模型
//    QSqlQueryModel *queryModel;

    DialogNetworkSet *dialogNetworkSet;

    QTcpSocket *clientFront;
    QTcpSocket *clientDataCenter;
    QTcpServer *serverAgent;
    QTcpSocket *clientA12;

//    bool frontIsConnected;
//    bool dataCenterIsConnected;
//    bool localAgentIsStarted;
//    bool a12Isconnected;

//    languageType langType;
//    QTranslator *trans;
};

#endif // MAINWINDOW_H
