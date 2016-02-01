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

#ifndef GRAPH_GRAPHICSVIEW_H
#define GRAPH_GRAPHICSVIEW_H

#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QMenuBar>
#include <QVector>
#include "mclass.h"
//#include "edge.h"

#include "mograph.h"

class Node;
class MScene: public QGraphicsScene
{Q_OBJECT
public: MScene(QObject *parent = 0):QGraphicsScene(parent){}
signals:void scene_mousePressEvent(QGraphicsSceneMouseEvent *ev);
protected:void mousePressEvent(QGraphicsSceneMouseEvent *ev){emit scene_mousePressEvent(/*static_cast<QMouseEvent*>*/(ev));QGraphicsScene::mousePressEvent(ev);}
};

//! [0]
class Graph_GraphicsView : public QGraphicsView
{
    Q_OBJECT

    enum MStatus{esNone,esAddNods, esSelectNode}mstatus;
public:
    Graph_GraphicsView(QWidget *parent = 0);

    QMenu *initMenu();
    void setGraph(MOGraph *graph);
signals:

    void sgChoseNode(int ind);
    //void sgChoseNode(Node *nd);

    void showFrame_AddEdge(); // ????

    void sgAddNode();

public slots:
    void shuffle();
    void zoomIn(); void zoomOut();
    void addEdge(int src, int dst, int typ );
    void addEdge(Node *nsrc, Node* ndst, int typ );
    Node *addNode(int ind);
    void slt_clear();
    void slt_selectLinks(const QVector<int> &src, const QVector<int> &dst);
    void slt_selectLink(const int src, const int dst);
    void slt_selectClear();
    void slt_updateGraph(const QVector<QVector<int>> &mtx);
protected:
    void mousePressEvent(QMouseEvent *ev);
    void scene_mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void timerEvent();
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    void scaleView(qreal scaleFactor);

private:
    MTimer mtim; QPointF mpk;
    Node *centerNode;
    MScene *Scene;
    void init();
    void addNode(int x, int y);
    Node *at(int i);
    void setColorLink(Node *src, Node *dst, const QColor &clr);

private:
    QString message;

};
//! [0]

#endif // GRAPH_GRAPHICSVIEW_H
