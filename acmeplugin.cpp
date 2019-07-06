#include "acmeplugin.h"

AcmePlugin::AcmePlugin(QString name, int type)
{
    this->name = name;
    this->type = type;
}


QString AcmePlugin::getName(){
    return name;
}
int AcmePlugin::getType(){
    return type;
}
