#ifndef USERI_H
#define USERI_H

#include <QWidget>
#include <QLineEdit>
#include <QPoint>
#include <QLabel>
#include "johnsonalgorithm.h"
#include "movementdetails.h"


namespace Ui {
class UserI;
}

class UserI : public QWidget
{
    Q_OBJECT

public:
    explicit UserI(QWidget *parent = 0);

    ~UserI();

private:
    //функции, которые описываются в cpp файле
    void startMenu();
    void setDesignSettitgsStart();
    void setDesignSettingsProc();
    bool checkFields();
    void setDetails();
    void resetSet();

    virtual void mousePressEvent(QMouseEvent * ex);
    virtual void mouseMoveEvent(QMouseEvent * ex);
    virtual void mouseReleaseEvent(QMouseEvent * ex);

    //максимальное количество деталей
    const static int ammountOfInput=6;
    //текущее количество деталей
    int currentAmDetais=6;
    //экземпляры классов
    johnsonAlgorithm *algo;
    movementDetails *moveDet;

    //графические переменные
    Ui::UserI *ui;
    QLineEdit *inputNumA[ammountOfInput];
    QLineEdit * inputNumB[ammountOfInput];
    QLabel * inpNumPodp[ammountOfInput];
    QLabel ** labels;
    int * xPositions;

    //слоты, функции, которые вызываются по какому либо взаимодействию пользователя с ПИ
    //каждая описывается в cpp
    public slots:
    void setOrder();
    void exitFromAplication();
    void randomElem();
    void turnToPlane();
    void changeNumDetails(int value);
    void setValues();
    void valueTextCh();
    void drawOptimalGraph();
    void drawGraph();
};

#endif // USERI_H
