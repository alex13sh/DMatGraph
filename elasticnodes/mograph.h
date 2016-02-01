#ifndef MOGRAPH_H
#define MOGRAPH_H

/* Модель графа
 * 1 структура и 2 класса
 * MGraph_Ind - для хранение идексов, в основном используется для Выделение графа. Для это мы передаём индексы
 * MGraph - Чистый класс на С++. В нём реализованны функции для обработки графа. И через виртуальные функции говорит об изменений.
 * MOGraph - Класс "Обёртка" над классом MGraph , чтобы использовать сигналы-слоты и нетолько
 * */

#include <QObject>
#include <QVector>
#include <QMenu>

#include <QDataStream>

//class Node;

// это для объявление указателей на функцию с такой типизацией
typedef void (*t_void_int)(int val) ;
typedef void (*t_void_int_int)(int val, int ur) ;
typedef void (*t_bool_int_int)(int val, int ur) ;

struct MGraph_Ind{
    enum eMGI{ // Для чего мы используем (полностью не раскрыта суть, заложил на будуще)
        eiLinks, eiGraph, eiTree
    } typ;
    // MGraph *graph; // Думаю ещё хранить указатель на сам граф, для индификации
    struct MNode_ind{ // Структура одного узла
        int ind; // Храним индекс самого узла
        QVector<int> arrL; // И индексы узлов связанных с ним
        MNode_ind(int ind=0):ind(ind){}
    };
    QVector<MNode_ind> arrN; // Список этих узлов. Размер списка не обязан соответствовать количеству всех узлов, а соответствует лишь количеству выделенных узлов

    void addLink(int src, int dst){ // метод для добавление связки 2ух узлов. Направление src -> dst;
        for(int i=0;i<arrN.size();++i){ // Обход узлов
            if(arrN[i].ind==src){
                for(int j=0;j<arrN[i].arrL.size();++j){
                    if(arrN[i].arrL[j]==dst) return; // Находим эту связку, если она есть то пусть и остаётся
                }
                arrN[i].arrL.append(dst); return; // Иначе мы создаём связ с dst узлом
            }
        }
        arrN.append(MNode_ind(src)); arrN.last().arrL.append(dst); return; // Или создаём узел src и добавлям первую связь с dst
    }
};

class MGraph{
public:
    struct SLink{// - Дуга (Связь) мужду 2мя узлами
        int ind, w; // Имеет индекс (dest) и растояние )вес)
        SLink():ind(0),w(0){} // пустой конструктор
        SLink(int ind,int w=0):ind(ind),w(w){} // Инициализирущий конструктор
    };
    enum eChange{ // Типы изменений, которые мы передаём через виртуальные методы нашему наследнику
        esNone,esClear,
        esAddNode, esAddEdge,
        esChangeNodes, esChangeEdges,esChangeGraph
    };

//private:
    QVector<QVector<SLink> > arrN; // Список узлов
public:

    void addNode();
    void addLink(int src, int dst); // Связь src->dst

    const SLink &link(int src,int dst)const; // get

    void clear(){this->arrN.resize(0); emit graph_change(esClear);}

    QVector<QVector<int> > getMatrixS(); //[Недоработан]// перевод в Матрицу

    void swap(int src, int dst); // Меняем местами src <-> dst
private:
    int linkInd(int src,int dst); // берём индекс дуги
    SLink *linkP(int src,int dst); // или указатель на него

    void selectLinks(const MGraph_Ind &mgi); // Выделение Дуг, по Индексному графу

    // void creat_fmost();
protected:
    // Общаемся с наследниками
    virtual void graph_change(eChange , int=0 , SLink  = SLink()){}
    virtual void graph_selectLinks(const QVector<int> &src, const QVector<int> &dst){Q_UNUSED(src);Q_UNUSED(dst);}
public:
    MGraph_Ind f_FindBridg(); // нахождение мостов в графе (По заданию)

    // Шаблонны методы обходов. В Compare - указываем лямбду функцию обратного вызова (CallBack )
    template<typename Compare>void BFS(int ind, Compare func, bool rest=0);
    template<typename Compare>void DFS(int ind, Compare func, bool rest=0);
        template<typename Compare>void DFS_r(int ind, Compare func);
    // [Потом сделаю эти методы с CallBack на указатель]

};

#include <QColor>
class MOGraph : public QObject, public MGraph
{
    Q_OBJECT
    QColor _color; // Даж не знаю нафиг я сделал цвет для него тут. [Думаю потом уберётся]
public:
    explicit MOGraph(QObject *parent = 0);

    QMenu *initMenu(); // Создание меню для главного окна.
    enum eExec{ // [Задел на будущее. Возможно уберётся.] Тип Вызовов функции с ожиданием.
        eeNone,
        eeAddEdge
    };

//    friend QDataStream  &operator<<(QDataStream  &f, const MOGraph &grf);
//    friend QDataStream  &operator>>(QDataStream  &f, MOGraph &grf);

    void setMatrix(const QVector<QVector<int> > &mtx, int siz); // Меняем граф по матрице смежности
signals:
    void sg_addNode(int ind); // [Уберётся] Так как он есть в сигнале sg_graph_change
    void sg_graph_change(MGraph::eChange t, int sourc, MGraph::SLink lnk); // Сигнал об изменений графа и его частей: узлов и дуг
    // Выделения Для Graph_GraphicsView. [Возможно есчо для каких нить Вьюшек пригодится]
    void sg_graph_selectLinks(const QVector<int> &src, const QVector<int> &dst);  // Выделение Дуг
    void sg_graph_selectLink(const int src, const int dst); // Выделение 1ой дуги
    void sg_graph_selectClear(); // Очистка выделений [Думаю переработаю]
    void sg_exec(eExec typ); // [Задел  на будущее]
public slots:
    void sl_addNode();
    void sl_addEdge(int sourc,int dest){MGraph::addLink(sourc,dest);}
    void selectFS(int src,int dst,int ur,int metk); // Выделение после выполнение одного из обхода: DFS, BFS
protected:
    // Переобределяем виртуальные методы. Общение с предком
    void graph_change(eChange t, int sourc, SLink lnk);
    void graph_selectLinks(const QVector<int> &src, const QVector<int> &dst);

    //void fileOpen(); void fileSave();

private: // Доп. Функции.
    void toLinkAll(); // Связываем каждый узел с остальными
    void random(); // Рандомный граф
};

// Для записи/чтение  файла и не только
// IODevice
QDataStream  &/*MOGraph::*/operator<<(QDataStream  &f, const MOGraph &grf);

QDataStream  &/*MOGraph::*/operator>>(QDataStream  &f, MOGraph &grf);
//SLink
QDataStream  &/*MOGraph::*/operator<<(QDataStream  &f, const MOGraph::SLink &lnk);
QDataStream  &/*MOGraph::*/operator>>(QDataStream  &f, MOGraph::SLink &lnk);



#endif // MOGRAPH_H
