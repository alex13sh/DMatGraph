#ifndef MOGRAPH_H
#define MOGRAPH_H

#include <QObject>
#include <QVector>
#include <QMenu>

#include <QDataStream>

//class Node;

typedef void (*t_void_int)(int val) ;
typedef void (*t_void_int_int)(int val, int ur) ;
typedef void (*t_bool_int_int)(int val, int ur) ;

struct MGraph_Ind{
    enum eMGI{
        eiLinks, eiGraph, eiTree
    } typ;

    struct MNode_ind{
        int ind;
        QVector<int> arrL;
        MNode_ind(int ind=0):ind(ind){}
    };
    QVector<MNode_ind> arrN;

    void addLink(int src, int dst){
        for(int i=0;i<arrN.size();++i){
            if(arrN[i].ind==src){
                for(int j=0;j<arrN[i].arrL.size();++j){
                    if(arrN[i].arrL[j]==dst) return;
                }
                arrN[i].arrL.append(dst); return;
            }
        }
        arrN.append(MNode_ind(src)); arrN.last().arrL.append(dst); return;
    }
};

class MGraph{
public:
    struct SLink{
        int ind, w;
        SLink():ind(0),w(0){}
        SLink(int ind,int w=0):ind(ind),w(w){}
    };
    enum eChange{
        esNone,esClear,
        esAddNode, esAddEdge,
        esChangeNodes, esChangeEdges,esChangeGraph
    };

//private:
    QVector<QVector<SLink> > arrN;
public:

    void addNode();
    void addLink(int x, int y); // Связь x->y

    const SLink &link(int x,int y)const;

    void clear(){this->arrN.resize(0); emit graph_change(esClear);}

    QVector<QVector<int> > getMatrixS(); // Недоработан

    void swap(int x, int y); // Меняем местами x <-> y
private:
    int linkInd(int x,int y);
    SLink *linkP(int x,int y);

    void selectLinks(const MGraph_Ind &mgi);

    void creat_fmost();
protected:
    virtual void graph_change(eChange , int=0 , SLink  = SLink()){}
    virtual void graph_selectLinks(const QVector<int> &src, const QVector<int> &dst){Q_UNUSED(src);Q_UNUSED(dst);}
public:
    MGraph_Ind f_FindBridg();

    template<typename Compare>void BFS(int ind, Compare func, bool rest=0);
    template<typename Compare>void DFS(int ind, Compare func, bool rest=0);
    template<typename Compare>void DFS_r(int ind, Compare func);

};

#include <QColor>
class MOGraph : public QObject, public MGraph
{
    Q_OBJECT
    QColor _color;
public:
    explicit MOGraph(QObject *parent = 0);

    QMenu *initMenu();
    enum eExec{
        eeNone,
        eeAddEdge
    };

//    friend QDataStream  &operator<<(QDataStream  &f, const MOGraph &grf);
//    friend QDataStream  &operator>>(QDataStream  &f, MOGraph &grf);

    void setMatrix(const QVector<QVector<int> > &mtx, int siz);
signals:
    void sg_addNode(int ind);
    void sg_graph_change(MGraph::eChange t, int sourc, MGraph::SLink lnk);
    void sg_graph_selectLinks(const QVector<int> &src, const QVector<int> &dst);
    void sg_graph_selectLink(const int src, const int dst);
    void sg_graph_selectClear();
    void sg_exec(eExec typ);
public slots:
    void sl_addNode();
    void sl_addEdge(int sourc,int dest){MGraph::addLink(sourc,dest);}
    void selectFS(int src,int dst,int ur,int metk);
protected:
    void graph_change(eChange t, int sourc, SLink lnk);
    void graph_selectLinks(const QVector<int> &src, const QVector<int> &dst);

    //void fileOpen(); void fileSave();

private:
    void toLinkAll();
    void random();
};

// IODevice
QDataStream  &/*MOGraph::*/operator<<(QDataStream  &f, const MOGraph &grf);

QDataStream  &/*MOGraph::*/operator>>(QDataStream  &f, MOGraph &grf);
//SLink
QDataStream  &/*MOGraph::*/operator<<(QDataStream  &f, const MOGraph::SLink &lnk);
QDataStream  &/*MOGraph::*/operator>>(QDataStream  &f, MOGraph::SLink &lnk);



#endif // MOGRAPH_H
