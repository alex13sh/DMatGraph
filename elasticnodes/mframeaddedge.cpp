#include "mframeaddedge.h"
#include "ui_mframeaddedge.h"

MFrameAddEdge::MFrameAddEdge(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MFrameAddEdge)
{
    ui->setupUi(this);
    ui->txtSrc->installEventFilter(this);ui->txtDst->installEventFilter(this);
}

MFrameAddEdge::~MFrameAddEdge(){
    delete ui;
}

#include <QDebug>

void MFrameAddEdge::showEvent(QShowEvent *){
    ui->txtSrc->setFocus();
}

bool MFrameAddEdge::eventFilter(QObject *obj, QEvent *ev){
    //qDebug()<<"className: "<<obj->metaObject()->className();
    if( obj->metaObject()->className()==QString("QLineEdit"))
        if(ev->type()==QEvent::FocusIn){txt=static_cast<QLineEdit*>(obj);qDebug()<<"chose_txt"; return true;}
    return QFrame::eventFilter(obj, ev);

}

bool MFrameAddEdge::exec(MFrameAddEdge::eExec typ){
    switch(typ){
    case eeAddEdge: this->setWindowTitle("AddEdge");
    default:this->setWindowTitle("None");;
    }
    this->show();
}


void MFrameAddEdge::on_cmdOk_clicked(){
    int src=ui->txtSrc->text().toInt(), dst=ui->txtDst->text().toInt();
    //int typ=ui->cmbTyp->currentIndex();
    emit sg_ok(src,dst);
    //this->hide();
}

void MFrameAddEdge::on_cmdCancel_clicked(){
    this->hide(); disconnect(this,SIGNAL(sg_ok(int,int)));
}

void MFrameAddEdge::selectNode(int ind){
    if(txt==ui->txtSrc || txt==ui->txtDst) txt->setText(QString::number(ind));
    if(txt==ui->txtSrc ) ui->txtDst->setFocus();
    qDebug()<<"select: "<<ind;
}


void MFrameAddEdge::on_pushButton_clicked(){
    QString tmp=ui->txtSrc->text();
    ui->txtSrc->setText(ui->txtDst->text());
    ui->txtDst->setText(tmp);
}
