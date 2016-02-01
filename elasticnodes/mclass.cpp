#include "mclass.h"

void NSFunc::dbug(const QVector<QVector<int> > &mtx){
    for(int y=0;y<mtx.size();++y)
        //for(int x=0;x<mtx[y].size();++x)
            qDebug()<< mtx[y];
}


