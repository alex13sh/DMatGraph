#ifndef MFRAMEADDEDGE_H
#define MFRAMEADDEDGE_H

#include <QFrame>

class QLineEdit;
namespace Ui {
class MFrameAddEdge;
}

class MFrameAddEdge : public QFrame
{
    Q_OBJECT

public:
    explicit MFrameAddEdge(QWidget *parent = 0);
    ~MFrameAddEdge();
    bool eventFilter(QObject *obj, QEvent *ev);

    enum eExec{
        eeNone,
        eeAddEdge, eeSetPath
    };

    bool exec(eExec typ=eeAddEdge);
signals:
    void sg_ok(int src,int dst);
public slots:
    void selectNode(int ind);
private slots:
    void on_cmdOk_clicked();
    void on_cmdCancel_clicked();


    void showEvent(QShowEvent *);


    void on_pushButton_clicked();

private:
    Ui::MFrameAddEdge *ui;
    QLineEdit *txt;


};

#endif // MFRAMEADDEDGE_H
