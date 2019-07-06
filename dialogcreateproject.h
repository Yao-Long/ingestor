#ifndef DIALOGCREATEPROJECT_H
#define DIALOGCREATEPROJECT_H

#include <QDialog>

namespace Ui {
class DialogCreateProject;
}

class DialogCreateProject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreateProject(QWidget *parent = nullptr);
    ~DialogCreateProject();
    QString get_project_name();
    QString get_well_name();
    QString get_task_name();
    QString get_task_type();
    QString get_task_content();
    QString get_save_location();
private slots:

    void on_pushButtonBrowse_clicked();

//    void on_lineEdit_project_name_editingFinished();

private:
    Ui::DialogCreateProject *ui;
};

#endif // DIALOGCREATEPROJECT_H
