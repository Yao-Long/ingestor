#ifndef ACMEPLUGIN_H
#define ACMEPLUGIN_H

#include <QtDebug>
#include <QString>

class AcmePlugin
{
public:
    AcmePlugin(QString name, int type);
    QString getName();
    int getType();

private:
    QString name;
    int type;
};

#endif // ACMEPLUGIN_H
