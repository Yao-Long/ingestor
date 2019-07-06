#ifndef ACMEPROJECT_H
#define ACMEPROJECT_H

#include <QString>
#include <QStringList>


class AcmeProject
{
public:
    AcmeProject(){};
    AcmeProject(QString projectName, QString fileName);
    QString getProjectName();
    QString getFileName();
    void addService(QString service);
    bool delService(QString service);
private:
    QString projectName;
    QString fileName;
    QStringList services;
};

#endif // ACMEPROJECT_H
