#ifndef JOHNSONALGORITHM_H
#define JOHNSONALGORITHM_H

#include <QWidget>
#include <QPoint>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QPaintEvent>

//заранее объявляем класс, который будет отвечать за рисование графа
class graph;

//класс, который хранит в себе значения времени для двух станков, индексы,
//текущее количество деталей
//высчитывает оптимальный порядок
//высчитывает время по заданному порядку
class johnsonAlgorithm
{
private:
    int * firstStan;
    int * secondStan;
    int * indexes;
    int ammountOfStan=0;
public:
    johnsonAlgorithm();
    johnsonAlgorithm(int *firstStan, int *secondStan, int ammountOfStan);
    johnsonAlgorithm(const johnsonAlgorithm & algo);
    const johnsonAlgorithm operator=(const johnsonAlgorithm & algo);
    ~johnsonAlgorithm();

    //объявляем класс граф дружественным, чтобы дальше было удобнее брать переменные
    friend graph;

    //функция, которая высчитывает оптимальный порядок
    void solve();
    //возвращает индексы оптимального порядка
    int * getIndexes();
    //возвращает время по заданным индексам
    int getTime(int *index);
};

//рисует граф выполнения алгоритма
class graph: public QWidget
{
private:
    //хранит элементы, текущее количество элементов, индексы порядка
    int *fElem;
    int *sElem;
    int curNumber;
    int *indexes;
    //начальная позиция, откуда будет рисоваться граф
    QPoint startPos;
    //масштаб используется для регулирования размера графа, чтобы какое время бы не было
    //весь граф будет растянут или сжат
    float mashtab=10;



public:
    //функция, которая вызывается в самом начале запуска и при вызове функции repaint()
    virtual void paintEvent(QPaintEvent* event);
    //так как класс наследует класс QWidget, то мы должны передать объект, на котором будет рисоваться
    //граф, а так как я преобразую определенный объект на сцене в этот класс,то он автоматически будет передан конструктору
    explicit graph(QWidget *parent = 0);
    //рисует граф
    void paintGraph();
    //стирает все
    void clearGraph();
    //так как paintEvent мы передаем QPaintEvent*, то все необходимые настройки должны задать через другую
    //функцию, и через конструктор не вариант, так как класс уже преобразован
    void setSettings(int *indexes,QPoint startPos,johnsonAlgorithm *algo);


};


#endif // JOHNSONALGORITHM_H
