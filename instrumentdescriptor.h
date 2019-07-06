#ifndef INSTRUMENT_DESCRIPTOR_H
#define INSTRUMENT_DESCRIPTOR_H


#include <QString>
#include <QVariant>


//仪器描述器
class InstrumentDescriptor
{
public:
    virtual ~InstrumentDescriptor();
    //设置仪器描述
    virtual bool set_desription(const QVariant &data) = 0;

    //测试接口
    virtual void test() = 0;
private:
    QString name;
    int id;
    QString description;
};

#define InstrumentDescriptorIID "instrument_descriptor_iid"
Q_DECLARE_INTERFACE(InstrumentDescriptor, InstrumentDescriptorIID)




#endif // INSTRUMENT_DESCRIPTOR_H
