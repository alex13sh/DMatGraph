#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMenuBar>

#include "GraphicsView/graph_graphicsview.h"
#include "mframeaddedge.h"
#include "mograph.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private:
    QMenu menuV; // Меню для моих классов Представлений (View)
    QVector<QMenu*> arrMenu;
    void initMenu();
};

#endif // MAINWINDOW_H
