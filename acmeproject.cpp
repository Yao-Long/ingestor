#include "acmeproject.h"

AcmeProject::AcmeProject(QString projectName, QString fileName)
{
    this->projectName = projectName;
    this->fileName = fileName;
}


QString AcmeProject::getProjectName(){
    return projectName;
}

QString AcmeProject::getFileName(){
    return fileName;
}


void AcmeProject::addService(QString service){
    this->services.append(service);
}

bool AcmeProject::delService(QString service){
    return this->services.removeOne(service);
}
