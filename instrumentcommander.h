#ifndef INSTRUMENT_COMMANDER_H
#define INSTRUMENT_COMMANDER_H



#include <QString>
#include <QVariant>


//仪器命令适配器
class InstrumentCommander
{
public:
    virtual ~InstrumentCommander();
    //发送命令
    virtual bool sendCommand(const QVariant &data) = 0;

    //测试接口
    virtual void test() = 0;
private:
    QString name;
    int id;
    QString description;
};

#define InstrumentCommanderIID "instrument_commander_iid"
Q_DECLARE_INTERFACE(InstrumentCommander, InstrumentCommanderIID)



#endif // INSTRUMENT_COMMANDER_H
