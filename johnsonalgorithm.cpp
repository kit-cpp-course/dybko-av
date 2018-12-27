#include "johnsonalgorithm.h"

johnsonAlgorithm::johnsonAlgorithm(){}

johnsonAlgorithm::johnsonAlgorithm(int * firstStan,int * secondStan, int ammountOfStan)
{
   this->ammountOfStan = ammountOfStan;
   this->firstStan = new int[ammountOfStan];
   this->secondStan = new int[ammountOfStan];
   indexes = new int[ammountOfStan];
   for(int i=0;i<ammountOfStan;i++)
   {
       this->firstStan[i] = firstStan[i];
       this->secondStan[i] = secondStan[i];
       indexes[i] = i;
   }
}

johnsonAlgorithm::johnsonAlgorithm(const johnsonAlgorithm & algo)
{
    this->ammountOfStan = algo.ammountOfStan;
    this->firstStan = new int[ammountOfStan];
    this->secondStan = new int[ammountOfStan];
    indexes = new int[ammountOfStan];
    for(int i=0;i<ammountOfStan;i++)
    {
        this->firstStan[i] = algo.firstStan[i];
        this->secondStan[i] = algo.secondStan[i];
        indexes[i] = i;
    }
}
const johnsonAlgorithm johnsonAlgorithm::operator =(const johnsonAlgorithm & algo)
{
    this->ammountOfStan = algo.ammountOfStan;
    this->firstStan = new int[ammountOfStan];
    this->secondStan = new int[ammountOfStan];
    indexes = new int[ammountOfStan];
    for(int i=0;i<ammountOfStan;i++)
    {
        this->firstStan[i] = algo.firstStan[i];
        this->secondStan[i] = algo.secondStan[i];
        indexes[i] = i;
    }
}

johnsonAlgorithm::~johnsonAlgorithm()
{
    delete[] firstStan;
    delete[] secondStan;
    delete[] indexes;
}

//минимум
bool min(int*a,int*b)
{
    return *a<=*b;
}


//алгоритм джонсона
void johnsonAlgorithm::solve()
{
    int leftEdge = 0;
    int rightEdge = ammountOfStan;
    int minA=firstStan[0];
    int minB=secondStan[0];
    int iA=0;
    int iB=0;
    //заводим временные массивы, чтобы не стирать изначальный их порядок, но меняем порядок индексов
    int *fTemp = new int[ammountOfStan];
    int *sTemp = new int[ammountOfStan];
    for(int i=0;i<ammountOfStan;i++)
    {
        fTemp[i] = firstStan[i];
        sTemp[i] = secondStan[i];
    }
    while(leftEdge<rightEdge)
    {
        minA=fTemp[leftEdge];
        minB=sTemp[leftEdge];
        iA=leftEdge;
        iB=leftEdge;
        for(int i=leftEdge;i<rightEdge-1;i++)
        {
            if(!min(&minA,&fTemp[i+1]))
            {
                minA=fTemp[i+1];
                iA=i+1;
            }
            if(!min(&minB,&sTemp[i+1]))
            {
                minB=sTemp[i+1];
                iB=i+1;
            }
        }
        if(min(&minA,&minB))
        {
            int temp = fTemp[leftEdge];
            fTemp[leftEdge] = fTemp[iA];
            fTemp[iA] = temp;
            temp = sTemp[leftEdge];
            sTemp[leftEdge] = sTemp[iA];
            sTemp[iA] = temp;

            temp = indexes[leftEdge];
            indexes[leftEdge] = indexes[iA];
            indexes[iA] = temp;
            leftEdge++;
        }
        else
        {
            int temp = fTemp[rightEdge-1];
            fTemp[rightEdge-1] = fTemp[iB];
            fTemp[iB] = temp;
            temp = sTemp[rightEdge-1];
            sTemp[rightEdge-1] = sTemp[iB];
            sTemp[iB] = temp;

            temp = indexes[rightEdge-1];
            indexes[rightEdge-1] = indexes[iB];
            indexes[iB] = temp;
            rightEdge--;
        }
    }

}

int *johnsonAlgorithm::getIndexes()
{
    return indexes;

}
//подсчитываем время через формулу
int johnsonAlgorithm::getTime(int *index)
{
    int xSum=0,aSum=0,bSum=0,chSum=secondStan[index[0]];
    xSum = firstStan[index[0]];
    aSum = firstStan[index[0]];
    for(int i=1;i<ammountOfStan;i++)
    {
        chSum+=secondStan[index[i]];
        aSum+=firstStan[index[i]];
        bSum+=secondStan[index[i-1]];
        int temp = aSum-bSum-xSum,
                temp2 = 0;
        if(!min(&temp,&temp2))
            xSum+=temp;
    }
    return  xSum+chSum;
}
//заводим ширину кисточки, булевскую переменную, отвечающую за стирание или рисование графа, и первый запуск
bool isPaintF = true;
bool isClearing=false;
int paintWidth=15;
//рисуем граф, или же ничего не рисуем и при вызове от функции repaint(), у нас просто сотрется старый граф
void graph::paintEvent(QPaintEvent *event)
{
    if(isClearing)
        return;
    if(!isPaintF)
    {
            //по сути алгоритм тот же, что и когда мы высчитываем сумму времени, просто на каждом шаге
        //находим значения для позиций наших линий на графе
            QPointF *fXPos = new QPointF[curNumber];
            QPointF *sXPos = new QPointF[curNumber];
            //создаем painter и pen, которые отвечают за простое рисование
                QPainter painter(this);
                QPen pen;
                pen.setColor(QColor(255,255,255));
                pen.setWidth(paintWidth);
                painter.setPen(pen);
                painter.setRenderHint(QPainter::Antialiasing, true);
            int xSum=0,aSum=0,bSum=0,chSum=sElem[indexes[0]];
            xSum = fElem[indexes[0]];
            aSum = fElem[indexes[0]];
            fXPos[0] = QPointF(startPos.x(),startPos.y());
            sXPos[0] = QPointF(startPos.x() + xSum*mashtab+paintWidth+3,startPos.y()+pen.width()+3);
            for(int i=1;i<curNumber;i++)
            {

                chSum+=sElem[indexes[i]];
                aSum+=fElem[indexes[i]];
                fXPos[i] = QPointF(fXPos[i-1].x()+fElem[indexes[i-1]]*mashtab+paintWidth+3,startPos.y());
                bSum+=sElem[indexes[i-1]];
                int temp = aSum-bSum-xSum,
                        temp2 = 0;
                if(!min(&temp,&temp2))
                {
                    sXPos[i] = QPointF(sXPos[i-1].x()+sElem[indexes[i-1]]*mashtab+paintWidth+3+temp*mashtab,startPos.y()+pen.width()+3);
                    xSum+=temp;
                }
                else
                {
                    sXPos[i] = QPointF(sXPos[i-1].x()+sElem[indexes[i-1]]*mashtab+paintWidth+3+temp2*mashtab,startPos.y()+pen.width()+3);
                }

            }
            //рисуем полосы временные
            for(int i=0;i<curNumber;i++)
            {
                painter.drawLine(QLineF(fXPos[i],QPointF(fXPos[i].x()+fElem[indexes[i]]*mashtab,fXPos[i].y())));
                painter.drawLine(QLineF(sXPos[i],QPointF(sXPos[i].x()+sElem[indexes[i]]*mashtab,sXPos[i].y())));
            }

            pen.setColor(QColor(255,0,0));
            pen.setWidth(5);
            painter.setPen(pen);
            //рисуем временную шкалу
            int result = xSum+chSum;
            float mash = (739 - startPos.x()*2)/result;
            for(int i=0;i<=result;i++)
            {
                painter.drawLine(startPos.x()+mash*i-pen.width()/2,43,startPos.x()+mash*i+0.5-pen.width()/2,43);
            }
    }
    isPaintF =false;
}



graph::graph(QWidget *parent) : QWidget(parent)
{

}

void graph::paintGraph()
{
    isClearing=false;
    repaint();
}

void graph::clearGraph()
{
    isClearing=true;
    repaint();
}

//задаем настройки для графа
void graph::setSettings(int *indexes, QPoint startPos, johnsonAlgorithm *algo)
{
    curNumber = algo->ammountOfStan;
    this->startPos = startPos;
    fElem = new int[curNumber];
    sElem = new int[curNumber];
    this->indexes = new int[curNumber];
    for(int i=0;i<curNumber;i++)
    {
        fElem[i] = algo->firstStan[i];
        sElem[i] = algo->secondStan[i];
        this->indexes[i] = indexes[i];
    }
    mashtab = (739-(paintWidth+3)*curNumber - startPos.x()*2)/algo->getTime(indexes);
}

