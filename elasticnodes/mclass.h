#ifndef MCLASS
#define MCLASS

#include <QDebug>
#include <QTimer>

namespace NSFunc {
  void dbug(const QVector<QVector<int> > &mtx);
}

class MTimer : public QTimer
{
    bool _enable;

public:
    MTimer():_enable(false){}

    void setEnable(bool enb){_enable=enb;}
    bool enabled(){return _enable;}
    void turn(){if(_enable)stop();else start(); }
    void start(){_enable=true; QTimer::start();}
    void stop(){_enable=false; QTimer::stop();}
};





#endif // MCLASS




