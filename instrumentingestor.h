#ifndef INTRUMENT_INGESTOR_H
#define INTRUMENT_INGESTOR_H

#include <QString>
#include <QVariant>


//数据前摄器
class InstrumentIngestor
{
public:
    virtual ~InstrumentIngestor();
    //解析数据
    virtual bool parse_data(const QVariant &data) = 0;
    //重组数据
    virtual bool rebuild_data(const QVariant &data) = 0;

    //测试接口
    virtual void test() = 0;
private:
    QString name;
    int id;
    QString description;
};

#define InstrumentIngestorIID "instrument.ingestor.iid"
Q_DECLARE_INTERFACE(InstrumentIngestor, InstrumentIngestorIID)


#endif // INTRUMENT_INGESTOR_H
