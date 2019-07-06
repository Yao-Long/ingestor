#ifndef ACMEINSTRUMENT_H
#define ACMEINSTRUMENT_H




#include "instrumentcommander.h"
#include "instrumentdescriptor.h"
#include "instrumentdumper.h"
#include "instrumentingestor.h"
#include "instrumentprocessor.h"



class AcmeInstrument
{
public:
    AcmeInstrument();
private:
    InstrumentIngestor *ingestor;
    InstrumentProcessor *processor;
    InstrumentCommander *commander;
    InstrumentDumper *dumper;
    InstrumentDescriptor *descriptor;
};

#endif // ACMEINSTRUMENT_H
