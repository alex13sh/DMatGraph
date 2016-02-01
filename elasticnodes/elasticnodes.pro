QT += widgets
CONFIG += c++11
#QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    mclass.h \
    mframeaddedge.h \
    mograph.h \
    graph_graphicsview.h \
    mainwindow.h \
    GraphicsView/edge.h \
    GraphicsView/node.h \
    GraphicsView/graph_graphicsview.h \
    mfileview.h

SOURCES += \
        main.cpp \
    mframeaddedge.cpp \
    mclass.cpp \
    mograph.cpp \
    mainwindow.cpp \
    GraphicsView/edge.cpp \
    GraphicsView/node.cpp \
    GraphicsView/graph_graphicsview.cpp \
    mfileview.cpp

## install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/elasticnodes
#INSTALLS += target

FORMS += \
    mframeaddedge.ui
