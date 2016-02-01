/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graph_graphicsview.h"
#include "edge.h"
#include "node.h"

#include <math.h>

#include <QKeyEvent>

//! [0]
Graph_GraphicsView::Graph_GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{

    Scene = new MScene(this);
    Scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    Scene->setSceneRect(-300, -200, 600, 400);
    setScene(Scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Elastic Nodes"));
//! [0]

    connect(Scene,&MScene::scene_mousePressEvent,this,&Graph_GraphicsView::scene_mousePressEvent);
    //addNode(100,0); addNode(200,-20);
    //emit sgAddNode(); emit sgAddNode();

    connect(&mtim,&QTimer::timeout,this,&timerEvent);
    mtim.setInterval(1000/25);
    //mtim.start();

}
//! [1]

//! [2]



//! [3]
void Graph_GraphicsView::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Plus:zoomIn();break;
    case Qt::Key_Minus:zoomOut();break;
    case Qt::Key_Space:mtim.turn();break;
    case Qt::Key_Enter:shuffle();break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
//! [3]

//! [4]
void Graph_GraphicsView::timerEvent(){
    //Q_UNUSED(event);
    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) mtim.stop();
}
//! [4]

#ifndef QT_NO_WHEELEVENT
//! [5]
void Graph_GraphicsView::wheelEvent(QWheelEvent *event){
    scaleView(pow((double)2, event->delta() / 240.0));
}
//! [5]
#endif

//! [6]
void Graph_GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
//    QString message(tr("Click and drag the nodes around, and zoom with the mouse "
//                       "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}
//! [6]

//! [7]
void Graph_GraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void Graph_GraphicsView::init(){

}

QMenu* Graph_GraphicsView::initMenu(){
    static int count=0; ++count;
    Q_ASSERT_X(count<=1,"Graph_GraphicsView::initMenu","Count>1");

    QMenu *menu=new QMenu("&GraphicsView",this); menu->show();
    QMenu *mn; QList<QAction *>lact;

    lact.clear();
    lact.append(new QAction("Show/Hi&de",this)); connect(lact.last(),&QAction::triggered,[&](){if(this->isVisible()) this->hide(); else this->show();});
    lact.append(new QAction("&Update",this)); connect(lact.last(),&QAction::triggered,[&](){this->update();}); lact.last()->setShortcut(tr("Ctrl+R"));

    menu->addActions(lact);

    mn= new QMenu("&Animation",menu); menu->addMenu(mn); lact.clear();
    lact.append(new QAction("Sta&rt",this)); connect(lact.last(),&QAction::triggered,&mtim,&MTimer::start);
    lact.append(new QAction("Sto&p",mn)); connect(lact.last(),&QAction::triggered,&mtim,&MTimer::stop);
    lact.append(new QAction("&Turn",mn)); connect(lact.last(),&QAction::triggered,&mtim,&MTimer::turn);      lact.last()->setShortcut(tr("Ctrl+A"));//( Qt::CTRL + Qt::Key_A) & (Qt::CTRL + Qt::Key_T));
    mn->addActions(lact);

    mn= new QMenu("Edit",menu); menu->addMenu(mn); lact.clear();
    QMenu *mn2;
    //mn2= new QMenu("Scene",mn); mn->addMenu(mn2); lact.clear();
    lact.append(new QAction("AddNode",this)); connect(lact.last(),&QAction::triggered,this,[this]{this->mstatus=esAddNods;});//if(this->mstatus!=esAddNods)this->mstatus=esAddNods;else this->mstatus=esNone;});
    lact.append(new QAction("None",this)); connect(lact.last(),&QAction::triggered,this,[this]{this->mstatus=esNone;});
    lact.append(new QAction("Clear",this)); connect(lact.last(),&QAction::triggered,this,[this]{this->Scene->clear();});
    mn->addActions(lact);

    mn= new QMenu("Other",menu); menu->addMenu(mn); lact.clear();
    lact.append(new QAction("Select clear",this)); connect(lact.last(),&QAction::triggered,[&](){this->slt_selectClear();});
    mn->addActions(lact);

    return menu;
}

void Graph_GraphicsView::setGraph(MOGraph *graph){
    // GraphV -> graph ***
    QObject::connect(this,&Graph_GraphicsView::sgAddNode,[graph](){graph->addNode();});

    // graph -> GraphV ***
    QObject::connect(graph,&MOGraph::sg_graph_selectLinks,
             this,&Graph_GraphicsView::slt_selectLinks);
    QObject::connect(graph,&MOGraph::sg_graph_selectLink,
             this,&Graph_GraphicsView::slt_selectLink);
    QObject::connect(graph,&MOGraph::sg_graph_selectClear,
             this,&Graph_GraphicsView::slt_selectClear);

    QObject::connect(graph,&MOGraph::sg_graph_change,
             [this,graph](MGraph::eChange t, int sourc, MGraph::SLink lnk){
                if(t==MGraph::esAddNode)this->addNode(sourc);
                else if(t==MGraph::esAddEdge) this->addEdge(sourc,lnk.ind,2);
                else if(t==MGraph::esClear) this->slt_clear();
                else if(t==MGraph::esChangeGraph) this->slt_updateGraph(graph->getMatrixS());
            });
}


void Graph_GraphicsView::addNode(int x, int y){
    addNode(0);
}

Node *Graph_GraphicsView::at(int i){
    foreach (QGraphicsItem *itm, this->Scene->items())
        if(itm->type()==QGraphicsItem::UserType + 1) {
            Node *nd=static_cast<Node*>(itm);
            if(nd->getInd()==i) return nd;
        }
    return 0;
}

void Graph_GraphicsView::setColorLink(Node *src, Node *dst, const QColor &clr){
    // if( src && dst )
    Edge *edg=src->findEdge(dst);
    if(edg) {
        edg->setColor(clr); this->update();
//        qDebug()<<"setColor: Ok; "
//                  "ind1:"<<edg->sourceNode()->getInd()
//               <<" ind2:"<<edg->destNode()->getInd();
    } else qDebug()<<"setColor: Not";
}

void Graph_GraphicsView::slt_selectClear(){
    foreach (QGraphicsItem *itm, this->Scene->items())
        if(itm->type()==QGraphicsItem::UserType + 2) {
            Edge *ed=static_cast<Edge*>(itm);
            ed->setColor(QColor(0,0,0));
        }
}


//! [7]

void Graph_GraphicsView::shuffle(){
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void Graph_GraphicsView::zoomIn(){
    scaleView(qreal(1.2));
}
void Graph_GraphicsView::zoomOut(){
    scaleView(1 / qreal(1.2));
}

void Graph_GraphicsView::addEdge(int src, int dst, int typ){
    mstatus=esNone;
    qDebug()<<"Src: "<<src<<"; Dst:"<<dst<<"; Typ:"<<typ;
    Node *nsrc=at(src), *ndst=at(dst);

    if(nsrc && ndst){
        this->Scene->addItem(new Edge(nsrc,ndst,typ));
        //grf.addLink(nsrc,ndst,typ);
    }
}
void Graph_GraphicsView::addEdge(Node *nsrc, Node* ndst, int typ){
    //mstatus=esNone;
    if(nsrc && ndst)this->Scene->addItem(new Edge(nsrc,ndst,typ));
}

Node* Graph_GraphicsView::addNode(int ind){
    Node *tmp=new Node(this,ind);
    int sw=this->Scene->sceneRect().width(), sh=this->Scene->sceneRect().height();
    if(this->mstatus==esAddNods) tmp->setPos(mpk); else tmp->setPos(QPointF(rand()%sw-sw/2,rand()%sh-sh/2));
    scene()->addItem(tmp);
    //emit sgAddNode();
    return tmp;
}

void Graph_GraphicsView::slt_clear(){
    this->Scene->clear();
}

void Graph_GraphicsView::slt_selectLinks(const QVector<int> &src, const QVector<int> &dst){
    qDebug()<<"Graph_GraphicsView::slt_selectLinks: "<<src.size();
    // src.size() == dst.size()
    Node *nsrc,*ndst;
    for(int i=0;i<src.size();++i){
        qDebug()<<i<<") Link:"<<src[i]<<"<->"<<dst[i];
        nsrc=at(src[i]); ndst=at(dst[i]);
        if( nsrc && ndst){ setColorLink(nsrc,ndst,QColor(200,10,0)); qDebug()<<"setColor Link:"<<src[i]<<"<->"<<dst[i];}
    }
}

void Graph_GraphicsView::slt_selectLink(const int src, const int dst){
    qDebug()<<"Graph_GraphicsView::slt_selectLink: ";
    // src.size() == dst.size()
    Node *nsrc,*ndst;
        qDebug()<<"Link:"<<src<<"<->"<<dst;
        nsrc=at(src); ndst=at(dst);
        if( nsrc && ndst){ setColorLink(nsrc,ndst,QColor(200,10,0)); qDebug()<<"setColor Link:"<<src<<"<->"<<dst;}
}

void Graph_GraphicsView::slt_updateGraph(const QVector<QVector<int> > &mtx){
    qDebug()<<"Graph_GraphicsView::slt_updateGraph: "<<mtx.size();
    Node *nsrc,*ndst;
    Edge *edg;
    QGraphicsItemGroup delGroup;
    for(int i=0;i<mtx.size();++i){
        nsrc=at(i); if(!nsrc) nsrc=addNode(i);
        for(int j=0;j<mtx[i].size();++j){
            ndst=at(j); if(!ndst) ndst=addNode(j);
            edg=nsrc->findEdge(ndst);
            if(mtx[i][j]==0){
                if(edg)if(edg->sourceNode()==nsrc){qDebug()<<"\tdelete";  delete edg;}//delGroup.addToGroup(edg);}
            }else if(!edg) addEdge(nsrc,ndst,2); else if(edg->sourceNode()==ndst) edg->setNap(3);
        }
    }
    //qDebug()<<"destroyItemGroup: "<<delGroup.childItems().count();
    //if(delGroup.childItems().count()>0)this->Scene->destroyItemGroup(&delGroup);
    //this->Scene->items()
}

void Graph_GraphicsView::mousePressEvent(QMouseEvent *ev){QGraphicsView::mousePressEvent(ev);}

void Graph_GraphicsView::scene_mousePressEvent(QGraphicsSceneMouseEvent *ev){
    switch(this->mstatus){
    case esAddNods:
        if(ev->button()==Qt::LeftButton) {
            mpk=ev->scenePos();emit sgAddNode(); //addNode(ev->scenePos().x() ,ev->scenePos().y());
        }
        break;
    case esSelectNode: if(ev->button()==Qt::LeftButton) {
            QGraphicsItem *itm=this->Scene->itemAt(ev->scenePos(),this->transform());
            if(!itm)break;
            qDebug()<<"Select: Typ:"<<itm->type();
            if(itm->type()!=(QGraphicsItem::UserType+1))break;//qDebug()<<"Select: Node";
            Node *nd=static_cast<Node*>(itm);
            if(nd){emit sgChoseNode(nd->getInd()); qDebug()<<"RCM: "<<nd->getInd();}//emit sgChoseNode(nd);

        }
        break;
    default:;

    }
}

//void GraphWidget::setMatrixS(const QVector<int> &arr, int n){
//    if(n>0); else return;
//    Scene->clear();
//    QVector<Node*> arrN(n);
//    for(int i=0;i<n;++i) scene()->addItem(arrN[i]=new Node(this,QString::number(i)));

//    for(int i=0; i<arr.size();++i)
//        if(arr[i]!=0)if(i/n<=i%n){
//            scene()->addItem(new Edge(arrN[i%n],arrN[i/n],0,QString("E%1").arg(i)));
//        }
//    shuffle();
//}

//void GraphWidget::setMatrixI(const QVector<int> &arr, int n){
//    if(n>0); else return;
//    Scene->clear();
//    QVector<Node*> arrN(n);
//    for(int i=0;i<n;++i) scene()->addItem(arrN[i]=new Node(this,QString::number(i)));


//    for(int i=0,p=0, i2=0; i<arr.size();++i)
//        if(arr[i]!=0 && arr[i2=i%n*n+i/n]!=0)if(i/n>=i%n){
//            scene()->addItem(new Edge(arrN[i%n],arrN[i/n],(arr[i]+3)/2 | 3-(arr[i2]+3)/2,QString("E%1").arg(p++)));
//        }
//    shuffle();
//}
//! [2]
