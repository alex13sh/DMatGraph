#include "mograph.h"
#include <QDebug>
#include <QFile>

MOGraph::MOGraph(QObject *parent) : QObject(parent)
{

}

QMenu *MOGraph::initMenu(){
    static int count=0; ++count;
    Q_ASSERT_X(count<=1,"MOGraph::initMenu","Count>1");

    QMenu *menu=new QMenu("MGsraph"); menu->show();
    QMenu *mn; QList<QAction *>lact;


    mn= new QMenu("File",menu); menu->addMenu(mn); lact.clear();
    //lact.append(new QAction("Open",mn)); connect(lact.last(),&QAction::triggered,this,[this]{this->fileOpen();});
    //lact.append(new QAction("Save",mn)); connect(lact.last(),&QAction::triggered,this,[this]{this->fileSave();});
    mn->addActions(lact);

    mn= new QMenu("Edit",menu); menu->addMenu(mn); lact.clear();
    lact.append(new QAction("AddNode",mn)); connect(lact.last(),&QAction::triggered,this,[this]{this->addNode();});             lact.last()->setShortcut(QKeySequence( Qt::CTRL + Qt::Key_N));
    lact.append(new QAction("AddEdge",mn)); connect(lact.last(),&QAction::triggered,this,[this]{emit sg_exec(eeAddEdge);});     lact.last()->setShortcut( Qt::CTRL + Qt::Key_E);
    lact.append(new QAction("Clear",mn));   connect(lact.last(),&QAction::triggered,this,[this]{this->clear();});
    mn->addActions(lact);

    mn= new QMenu("Functions",menu); menu->addMenu(mn); lact.clear();
    lact.append(new QAction("Find Bring",mn));          connect(lact.last(),&QAction::triggered,this,[this]{this->f_FindBridg();});
    lact.append(new QAction("Find cut out way",mn));    connect(lact.last(),&QAction::triggered,this,[this]{;});
    lact.append(new QAction("BFS",mn));
        connect(lact.last(),&QAction::triggered,this,[this]{
                sg_graph_selectClear();
                MGraph::BFS(0,[&](int src,int dst,int ur,int metk){selectFS(src,dst,ur,metk);},1);
            });
    lact.append(new QAction("DFS",mn));
        connect(lact.last(),&QAction::triggered,this,[this]{
            sg_graph_selectClear();
            MGraph::DFS(0,[&](int src,int dst,int ur,int metk){selectFS(src,dst,ur,metk);},1);
        });
    mn->addActions(lact);

    mn= new QMenu("Other",menu); menu->addMenu(mn); lact.clear();
    lact.append(new QAction("To Link all",mn));          connect(lact.last(),&QAction::triggered,this,[this]{this->toLinkAll();});
    lact.append(new QAction("Random",mn));          connect(lact.last(),&QAction::triggered,this,[this]{this->random();});
    mn->addActions(lact);

    return menu;
}

void MOGraph::setMatrix(const QVector<QVector<int> >& mtx, int siz){
    this->arrN.resize(siz);
    for(int y=0; y<mtx.size();++y){
        for(int x=0;x<mtx[y].size();++x){
            arrN[y].append(SLink(x));
        }
    }
}

void MOGraph::sl_addNode(){
    this->arrN.resize(arrN.size()+1);
}

void MOGraph::selectFS(int src, int dst, int ur, int metk){ Q_UNUSED(ur);
    if(metk==0)sg_graph_selectLink(src,dst);
}

// Сигналы виртуальных методов
void MOGraph::graph_change(MGraph::eChange t, int sourc, MGraph::SLink lnk){
    emit sg_graph_change(t,sourc,lnk);
}
void MOGraph::graph_selectLinks(const QVector<int> &src, const QVector<int> &dst){
    qDebug()<<"MOGraph::graph_selectLinks";
    emit sg_graph_selectLinks(src,dst);
}



void MOGraph::toLinkAll(){
    qDebug()<<"MOGraph::toLinkAll";
    for(int i=0;i<arrN.size();++i){
        arrN[i].clear(); //arrN[i].resize(arrN.size());
        for(int j=i+1;j<arrN.size();++j)
            arrN[i].append(SLink(j)); //arrN[i][j].ind=j;
    }
    emit sg_graph_change(esChangeGraph,0,SLink(0));
}

void MOGraph::random(){

    if(arrN.size()<4) arrN.resize(4);
    int ind,siz=arrN.size(); Q_UNUSED(ind);
    for(int i=0;i<siz;++i){
        for(int j=0;j<2;++j){
            
            arrN[i].append(qrand()%siz);
        }
    }
    sg_graph_change(esChangeGraph,0,0);
}
// -- End

void MGraph::addNode(){
    arrN.append(QVector<SLink>());
    graph_change(esAddNode,arrN.count()-1);  // Говорим наследникам об изменениях в графе
}

void MGraph::addLink(int src, int dst){
    Q_ASSERT_X(src<arrN.size() && dst<arrN.size(),"MGraph::addLink","index out of range");
    for(int i=0;i<arrN[src].size();++i)
        if(arrN[src][i].ind==dst) return;
    arrN[src].append(SLink(dst));
    graph_change(esAddEdge,src,SLink(dst)); // Говорим наследникам об изменениях в графе
}

const MGraph::SLink &MGraph::link(int src, int dst) const{
    Q_ASSERT_X(src<arrN.size() && dst<arrN.size(),"MGraph::link","index out of range");
    const QVector <SLink> &arrL1=arrN[src];
    for (int i=0;i<arrL1.size();++i)
        if(arrL1.at(i).ind==dst) return arrL1.at(i);
    const QVector <SLink> &arrL2=arrN[src];
    for (int i=0;i<arrL2.size();++i)
        if(arrL2.at(i).ind==src) return arrL2.at(i);
    return SLink(0);
}

QVector<QVector<int> > MGraph::getMatrixS(){
    qDebug()<<"MGraph::getMatrixS";
    QVector<QVector<int> >  res;
    int siz=arrN.size();
    res.resize(siz);
    for(int x=0;x<siz;++x){ res[x].resize(siz);
        for(int i=0,y=0; i<arrN[x].size();++i){
            y=arrN[x][i].ind;
            res[x][y]=1;
        }
    }
    qDebug()<<res;
    return res;
}

void MGraph::swap(int src, int dst){
    Q_ASSERT_X(src<arrN.size() && dst<arrN.size(),"MGraph::swap","index out of range");
    QVector<SLink> &tmp=arrN[src];
    arrN[src]=arrN[dst]; arrN[dst]=tmp;
    graph_change(esChangeNodes,src); graph_change(esChangeNodes,dst);
}

MGraph::SLink *MGraph::linkP(int src, int dst){
    Q_ASSERT_X(src<arrN.size() && dst<arrN.size(),"MGraph::linkP","index out of range");
     QVector <SLink> *arrL=&arrN[src];
    for (int i=0;i<arrL->size();++i)    if(arrL->at(i).ind==dst)  return arrL->begin()+i;
    return 0;
}

void MGraph::selectLinks(const MGraph_Ind &mgi){
    QVector<int> arrSourc, arrDest;
    qDebug()<<"MGraph::selectLinks";
    for(int i=0; i<mgi.arrN.size();++i){
        for(int j=0; j<mgi.arrN[i].arrL.size();++j){
            arrSourc.append(mgi.arrN[i].ind);
            arrDest.append(mgi.arrN[i].arrL[j]);
        }
    }
    graph_selectLinks(arrSourc,arrDest);
}

//void MGraph::creat_fmost(){ // Создавал граф с мостом.  Теперь не нужен, т.к. я уже научился сохранять и открывать файлы с графами

//    if(arrN.size()<8)arrN.resize(8);
//    arrN[0].append(1); arrN[2].append(0); arrN[1].append(2);
//    arrN[3].append(4); arrN[5].append(3); arrN[4].append(5);
//    arrN[2].append(3);
//    arrN[5].append(6); arrN[0].append(7);
//    graph_change(esChangeGraph);
//}

//#include <QStack>
#include <QQueue>
//#include <QVector>
//#include <QList>

MGraph_Ind MGraph::f_FindBridg(){
    qDebug()<<"MGraph::f_FindBridg";
    creat_fmost();
    MGraph_Ind mgi;
    mgi.typ=MGraph_Ind::eiLinks;

    int siz=arrN.size();
    QVector<int> fup(siz),tin(siz); int tim=0;
    //QVector<bool> arrMetk(arrN.size()),stk;
    auto func=[&](int src, int dst,int ur, int mark){ Q_UNUSED(ur);
        //qDebug()<<"func: src="<<src<<" _dst="<<dst<<" _ur="<<ur<<" _mark="<<mark;
        //qDebug()<<"fup: "<<fup; qDebug()<<"tin: "<<tin;
        if(mark==1){fup[src] = std::min (fup[src], tin[dst]);}
        else if(mark==0){
            //qDebug()<<"src: "<<src<<" _dst: "<<dst<<" _ur: "<<ur;
            tin[dst]=fup[dst]=++tim;
        }else if(mark==-1){
            fup[src] = std::min (fup[src], fup[dst]);
            if (fup[dst] > tin[src]){
                mgi.addLink(src,dst);
                //qDebug()<< "bridge: (" << src << " , " << dst << ")\n";
            }
        }
            //mgi.addLink(src,dst);
//        if(stk.size()<=ur) stk.resize(ur+1);
//        stk[ur]=ind;
    };
    //arrMetk[0]=true; stk.append(0);
    DFS(0,func,1);
    for(int i=1;i<arrN.size();++i)
        DFS(i,func);
    qDebug()<<"fup: "<<fup;
    qDebug()<<"tin: "<<tin;
    selectLinks(mgi);
    return mgi;
}

//void MOGraph::selectFS(int src, int dst, int ur, int metk){
    //sg_graph_selectLink();

//}

template<typename Compare>
void MGraph::BFS(int ind, Compare func, bool rest){
    //Q_UNUSED(ind); Q_UNUSED(func);
    qDebug()<<"MGraph::BFS";
     struct s_ic{
        int  ind,ur,current; QVector<SLink> *p;
        s_ic(){}
        s_ic(QVector<SLink> *p,int ind=0,int ur=0):p(p),ind(ind),ur(ur),current(0){}
        int next(){return (*p)[current++].ind;}
        bool is_end(){return current==p->size();}
    };
    QQueue<s_ic> arr; arr.append(s_ic(&arrN[ind],ind,0));
    //qDebug()<<"Restart: "<<rest;
    static QVector<bool> arrMetk;  if(rest){arrMetk.clear(); arrMetk.resize(arrN.size()); }
    //qDebug()<<"Continue";
    int src=ind,dst=0; s_ic curP;
    while(arr.size()){
        //qDebug()<<"\tStake::size = "<<arr.size();
            curP=arr.first(); src=curP.ind;
            while(!curP.is_end()){
                dst=curP.next();
         //       qDebug()<<"\tCurrent: "<<curP.current<<" _Value:"<<dst;
                func(src,dst,curP.ur,arrMetk[dst]);
                if(!arrMetk[dst]){
                    arrMetk[dst]=true;
                    arr.append(s_ic(&arrN[dst],dst,curP.ur+1 ));
                }
            }
            curP=arr.first();
            func(curP.ind,-1,curP.ur,-1);
            arr.removeFirst();
    }

}

template<typename Compare>
void MGraph::DFS_r(int ind, Compare func){
    static int ur=0; ++ur;
    qDebug()<<"MGraph::DFS_r: ind="<<ind<<" _ur="<<ur<<" _siz="<<arrN[ind].size();
    for(int i=0;i<arrN[ind].size();++i){
        if(func(arrN[ind][i].ind,ur))
            DFS_r(arrN[ind][i].ind,func);
    }
    --ur;
}

template<typename Compare>
void MGraph::DFS(int ind, Compare func, bool rest){
    //qDebug()<<"MGraph::DFS";
     struct s_ic{
         int  ind,ur,current; QVector<SLink> *p;
         s_ic(){}
         s_ic(QVector<SLink> *p,int ind=0,int ur=0):p(p),ind(ind),ur(ur),current(0){}
         int next(){return (*p)[current++].ind;}
         bool is_end(){return current==p->size();}
    };
    QVector<s_ic> stk; stk.append(s_ic(&arrN[ind],ind,0));
    //qDebug()<<"Restart: "<<rest;
    static QVector<bool> arrMetk;  if(rest){arrMetk.clear(); arrMetk.resize(arrN.size()); }
    if(arrMetk[ind]) return;else arrMetk[ind]=1;  func(-1,ind,0,0);
    //qDebug()<<"Continue";
    int src=ind,dst=0;  s_ic curP;
    while(stk.size()){
        //qDebug()<<"\tStake::size = "<<stk.size();
            curP=stk.last();
            //g1:if(curP.is_end()) {stk.removeLast(); continue;}
            while(!curP.is_end()){
                src=curP.ind; dst=curP.next();
                //qDebug()<<"\tCurrent: "<<curP.current<<" _Value:"<<dst;
                func(src,dst,curP.ur,arrMetk[dst]);
                if(!arrMetk[dst]){
                    arrMetk[dst]=true;
                    //func(src,dst,curP.ur);
                    stk.append(s_ic(&arrN[dst],dst,curP.ur+1));
                    goto g2;
                }//else goto g1;
            } dst=stk.last().ind; stk.removeLast(); curP=stk.last(); src=curP.ind;
            func(src,dst,curP.ur,-1);
            g2:;
    }

}

int MGraph::linkInd(int src, int dst){
    Q_ASSERT_X(src<arrN.size() && dst<arrN.size(),"Node::at","index out of range");
     QVector <SLink> &arrL=arrN[src];
    for (int i=0;i<arrL.size();++i)
        if(arrL.at(i).ind==dst) return i;
    return -1;
}




QDataStream &operator>>(QDataStream &f, MOGraph &grf){
    f>>grf.arrN;
    return f;
}


QDataStream &operator<<(QDataStream &f, const MGraph::SLink &lnk){
    f<<lnk.ind<<lnk.w;
    return f;
}


QDataStream &operator>>(QDataStream &f, MGraph::SLink &lnk){
    f>>lnk.ind>>lnk.w;
    return f;
}


QDataStream &operator<<(QDataStream &f, const MOGraph &grf){
    f<<grf.arrN;
    return f;
}
