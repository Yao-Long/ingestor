#ifndef ACMESERVICE_H
#define ACMESERVICE_H

#include "acmeinstrument.h"

#include <QString>
#include <QList>


class AcmeService
{
public:
    AcmeService(QString serviceName);
    void addInstrument(AcmeInstrument ins);
    bool delInStrument(AcmeInstrument ins);
private:
    QString serviceName;
    QList<AcmeInstrument> instruments;
};

#endif // ACMESERVICE_H
