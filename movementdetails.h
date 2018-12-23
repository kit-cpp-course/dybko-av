#ifndef MOVEMENTDETAILS_H
#define MOVEMENTDETAILS_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

//класс, отвечающий за передвижение деталей пользователем
class movementDetails
{
private:
    int * xPositions;
    int yPosition;
    int *indexes;
    int curAmmount;
    QPoint fisrtPoint;
    QPoint secondPoint;
    QLabel ** details;


public:
    //конструкторы
    movementDetails(int *xPositions,int yPosition,QLabel **details,int curAmmount);
    const movementDetails operator=(const movementDetails & moveDet);
    //хотел сделать эти функции дружественными, но не задалось, компилятор их не видит вне класса, хотя должен был
    //выполняем обработку нажатия ЛКМ или ПКМ
    void setCursorTap(QMouseEvent * event);
    //обрабатываем передвижение мыши
    void movementCursor(QMouseEvent * event);
    //обрабатываем отпускание ЛКМ или ПКМ
    void setUpCursorTap(QMouseEvent * event);
    //возвращаем текущий порядок деталей
    int *getIndexes();
    ~movementDetails();


};

#endif // MOVEMENTDETAILS_H
