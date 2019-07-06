#ifndef INSTRUMENT_DUMPER_H
#define INSTRUMENT_DUMPER_H


#include <QString>
#include <QVariant>


//数据导出器
class InstrumentDumper
{
public:
    virtual ~InstrumentDumper();
    //导出曲线
    virtual bool dump_graph(const QVariant &data) = 0;

    //测试接口
    virtual void test() = 0;
private:
    QString name;
    int id;
    QString description;
};

#define InstrumentDumperIID "instrument_dumper_iid"
Q_DECLARE_INTERFACE(InstrumentDumper, InstrumentDumperIID)


#endif // INSTRUMENT_DUMPER_H
