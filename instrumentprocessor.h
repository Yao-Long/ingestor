#ifndef INSTRUMENT_PROCESSOR_H
#define INSTRUMENT_PROCESSOR_H

#include <QString>
#include <QVariant>


//数据处理器
class InstrumentProcessor
{
public:
    virtual ~InstrumentProcessor();
    //处理数据
    virtual bool process_data(const QVariant &data) = 0;
//    virtual bool get_scale(const QVariant &data) = 0;
//    virtual bool set_scale(const QVariant &data) = 0;

    //测试接口
    virtual void test() = 0;
private:
    int scale_info;
    int process_info;

    QString name;
    int id;
    QString description;
};

#define InstrumentProcessorIID "instrument_processorr_iid"
Q_DECLARE_INTERFACE(InstrumentProcessor, InstrumentProcessorIID)













#endif // INSTRUMENT_PROCESSOR_H
