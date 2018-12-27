#include "movementdetails.h"

//первая точка, это позиция курсора, когда мы в первый раз нажали КМ
QPoint firstPoint;
//вторая точка, это позиция курсора, когда мы сдвинули его на определенный вектор
//по сравнению с первой точкой
QPoint secondPoint;
int curDetail;
//если зажата КМ на детали, и мы двигаем мышью, то принимает значение true
bool isDrag = false;
int width;
int height;


movementDetails::movementDetails(int *xPositions,int yPosition,QLabel **details,int curAmmount)
{
    this->curAmmount = curAmmount;
    this->xPositions = new int[curAmmount];
    this->details = new QLabel*[curAmmount];
    this->yPosition = yPosition;
    indexes = new int[curAmmount];
    for(int i=0;i<curAmmount;i++)
    {
        this->xPositions[i] = xPositions[i];
        this->details[i] = details[i];
        indexes[i] = i;
    }
    width = details[0]->geometry().width();
    height = details[0]->geometry().height();
}

const movementDetails movementDetails::operator =(const movementDetails & moveDet)
{
    this->curAmmount = moveDet.curAmmount;
    this->xPositions = new int[curAmmount];
    this->details = new QLabel*[curAmmount];
    this->yPosition = moveDet.yPosition;
    indexes = new int[curAmmount];
    for(int i=0;i<curAmmount;i++)
    {
        this->xPositions[i] = moveDet.xPositions[i];
        this->details[i] = moveDet.details[i];
        indexes[i] = i;
    }
    width = details[0]->geometry().width();
    height = details[0]->geometry().height();
}

//устанавливаем первую точку и проверяем, лежит ли она на какой то детали
void movementDetails::setCursorTap(QMouseEvent* event)
{
   firstPoint = event->pos();

   for(int i=0;i<curAmmount;i++)
   {
       if(event->pos().x()>=xPositions[i]&&event->pos().x()<=xPositions[i]+width && event->pos().y()>=yPosition && event->pos().y()<=yPosition+height)
       {
           curDetail = i;
           isDrag = true;
           break;
       }
   }
}


void movementDetails::setUpCursorTap(QMouseEvent *event)
{
    //при отпускании КМ мы проверяем, текущее положение курсора лежит на какой то области детали или нет
    //если да, меняем детали и возвращаем цветовые характеристики
    //если нет, возвращаем ее на прежнее место и возвращаем цветовые характеристики
    firstPoint = event->pos();
    bool isChosen = false;
    for(int i=0;i<curAmmount;i++)
    {
        if(event->pos().x()>=xPositions[i]&&event->pos().x()<=xPositions[i]+width && event->pos().y()>=yPosition && event->pos().y()<=yPosition+height && !isChosen)
        {
            details[i]->move(xPositions[curDetail],yPosition);
            details[curDetail]->move(xPositions[i],yPosition);
            int temp = xPositions[i];
            xPositions[i] = xPositions[curDetail];
            xPositions[curDetail] = temp;
            isChosen = true;
//            temp = indexes[i];
//            indexes[i] = indexes[curDetail];
//            indexes[curDetail] = temp;
            QList<int> ap;

            for(int e=0;e<curAmmount;e++)
            {
                ap << indexes[e];
            }
            temp = indexes[ap.indexOf(i)];
            indexes[ap.indexOf(i)] = indexes[ap.indexOf(curDetail)];
            indexes[ap.indexOf(curDetail)] = temp;
        }
        details[i]->setStyleSheet("background-color: rgb(250, 255, 187);"
                                  "border: 1px solid white;"
                                  "border-color: rgb(255, 29, 78,0);"
                                  "border-radius: 10px;"
                                  "color: rgb(255, 0, 0);");
    }
    if(!isChosen)
    {
        details[curDetail]->move(xPositions[curDetail],yPosition);
    }
    isDrag=false;
}

int *movementDetails::getIndexes()
{
    return indexes;
}
bool isFirs=false;
void movementDetails::movementCursor(QMouseEvent *event)
{
    if(!isDrag)
        return;
    //что ж, расчитываем вектор, для смещения положения детали, куда мы сдвинули курсор
    //проверяем так же, лежит ли он на позиции какой либо детали, и если да, временно меняем
    //их местами и цветовые характеристики, чтобы показать пользователю, что вот такой будет порядок
    //если он сейчас отпустит мышь
    //(сделано для красоты и удобства)
    secondPoint = event->pos();
    details[curDetail]->move(details[curDetail]->pos()+(event->pos()-firstPoint));
    firstPoint = secondPoint;
    bool isChosen = false;
    for(int i=0;i<curAmmount;i++)
    {
        if(event->pos().x()>=xPositions[i]&&event->pos().x()<=xPositions[i]+width && event->pos().y()>=yPosition && event->pos().y()<=yPosition+height)
        {
            details[i]->move(xPositions[curDetail],yPosition);
            details[curDetail]->move(xPositions[i],yPosition);
            details[i]->setStyleSheet("background-color: rgb(187, 187, 187,100);"
                                      "border: 1px solid white;"
                                      "border-color: rgb(255, 29, 78,0);"
                                      "border-radius: 10px;"
                                      "color: rgb(255, 0, 0,100);");
            details[curDetail]->setStyleSheet(details[i]->styleSheet());
            isFirs=true;
            isChosen = true;
            break;
        }
    }
    //переменная isFirs отвечает за что то (не помню), но существует она для оптимизации
    //вспомнил, чтобы каждый раз обратно не перекрашивать детали и ставить их в свои позиции,
    //если мы не двигаем деталь в области
    //других деталей, мы храним значение в этой переменной о первом выходе курсора из области положения
    //детали, чтобы единожды поменять обратно позиции и цвет
    if(!isChosen && isFirs)
    {
        for(int i=0;i<curAmmount;i++)
        {
            if(i!=curDetail)
            {
                details[i]->move(xPositions[i],yPosition);
            }

            details[i]->setStyleSheet("background-color: rgb(250, 255, 187);"
                                      "border: 1px solid white;"
                                      "border-color: rgb(255, 29, 78,0);"
                                      "border-radius: 10px;"
                                      "color: rgb(255, 0, 0);");
        }
        details[curDetail]->move(event->pos().x()-details[curDetail]->geometry().width()/2,event->pos().y()-details[curDetail]->geometry().height()/2);
        isFirs=false;
    }
}

movementDetails::~movementDetails()
{
    delete[] xPositions;
    for(int i=0;i<curAmmount;i++)
    {
        delete details[i];
    }
    delete[] details;
}

