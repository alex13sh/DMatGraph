#ifndef MFILEVIEW_H
#define MFILEVIEW_H

#include <QFile>
#include <QFileDialog>
#include "mograph.h"

class MFileView
{
    MOGraph *graph;
public:
    MFileView();
    QMenu *initMenu();
    void setGraph(MOGraph *graph){this->graph=graph;}

    void saveTxt();
    void open(); void save();


};

#endif // MFILEVIEW_H
