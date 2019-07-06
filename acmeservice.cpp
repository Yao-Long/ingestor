#include "acmeservice.h"

AcmeService::AcmeService(QString serviceName)
{
    this->serviceName = serviceName;
}


void AcmeService::addInstrument(AcmeInstrument ins){
    instruments.append(ins);
}


bool AcmeService::delInStrument(AcmeInstrument ins){
//    return instruments.removeOne(ins);
}
