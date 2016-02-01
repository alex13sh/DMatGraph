#include "mfileview.h"

#include "mograph.h"

MFileView::MFileView()
{

}

void MFileView::saveTxt(){
    QFile fl("save.txt");
    if(!fl.open(QFile::WriteOnly | QFile::Text)) return;

    //QTextStream stream(&fl);
//    stream<<this->arrN.size();
//    for(int i=0;i<arrN.size();++i){

    //    }
}

void MFileView::open(){

    QFile fl("<Graph_save.bin");
    fl.open(QIODevice::WriteOnly);
    QDataStream out(&fl);
    out<<*this->graph;

    //if(out)
    fl.close();
}



