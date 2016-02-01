#include "mainwindow.h"
#include <QVBoxLayout>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),menuV("View")
{

    Graph_GraphicsView *GraphV = new Graph_GraphicsView();
    MOGraph *graph =new MOGraph(this);
    menuV.addMenu(GraphV->initMenu());
    arrMenu.append(graph->initMenu());
    MFrameAddEdge *mf=new MFrameAddEdge(); mf->hide();
        // GraphV -> mf
        //QObject::connect(GraphV,&Graph_GraphicsView::showFrame_AddEdge,mf,&MFrameAddEdge::show);
        //+QObject::connect(GraphV,&Graph_GraphicsView::sgChoseNode,mf,&MFrameAddEdge::selectNode);
        GraphV->setGraph(graph);
//        // GraphV -> graph ***
//        QObject::connect(GraphV,&Graph_GraphicsView::sgAddNode,[graph](){graph->addNode();});

//        // graph -> GraphV ***
//        QObject::connect(graph,&MOGraph::sg_graph_change,
//                 [GraphV,graph](MGraph::eChange t, int sourc, MGraph::SLink lnk){
//                    if(t==MGraph::esAddNode)GraphV->addNode(sourc);
//                    else if(t==MGraph::esAddEdge) GraphV->addEdge(sourc,lnk.ind,2);
//                    else if(t==MGraph::esClear) GraphV->slt_clear();
//                    else if(t==MGraph::esChangeGraph) GraphV->slt_updateGraph(graph->getMatrixS());
//                });


        // mf -> graph
        //QObject::connect(mf,&MFrameAddEdge::addEdge,&graph,&MOGraph::addLink);
        //QObject::connect(mf,&MFrameAddEdge::addEdge,&graph.addLink);
        QObject::connect(mf,&MFrameAddEdge::sg_ok, [graph](int src,int dst){graph->addLink(src,dst);});

        // graph -> mf
        QObject::connect(graph,&MOGraph::sg_exec,
                 [mf,graph](MOGraph::eExec t){
                    if(t==MOGraph::eeAddEdge){ mf->exec(mf->eeAddEdge); mf->connect(mf,&MFrameAddEdge::sg_ok,graph,&MOGraph::sl_addEdge);}
                });


    QVBoxLayout *vl=new QVBoxLayout(this);
    vl->addWidget(mf); vl->addWidget(GraphV);

    QWidget *cw=new QWidget; cw->setLayout(vl);
       this->setCentralWidget(cw);
    //this->setLayout(vl);
    initMenu();

}

void MainWindow::initMenu(){
    QMenuBar *menu=new QMenuBar(this); menu->show();
    menu->addMenu(&menuV);

    foreach (QMenu* mn, this->arrMenu) {
        menu->addMenu(mn);
    }
    this->setMenuBar(menu);
}

