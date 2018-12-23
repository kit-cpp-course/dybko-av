#include "useri.h"
#include "ui_useri.h"

//подключаем необходимые библиотеки для работы с текстом и рандомным рандомом
#include <QString>
#include <ctime>

//отвечает за первый запуск
bool isFirst = true;


UserI::UserI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserI)
{

    srand(time(0));
    ui->setupUi(this);

    //запускаем функции, которые делают дизайн стартового окна и окна, где вся работа программы
    setDesignSettitgsStart();
    setDesignSettingsProc();
    //подключаем слоты и сигналы кнопок с функциями, которые вызываются при нажатии
    connect(ui->setButton,SIGNAL(clicked(bool)),this,SLOT(setValues()));
    connect(ui->setButtonForCurrentOrder,SIGNAL(clicked(bool)),this,SLOT(setOrder()));
    connect(ui->exitButton,SIGNAL(clicked(bool)),this,SLOT(exitFromAplication()));
    connect(ui->exitButton_2,SIGNAL(clicked(bool)),this,SLOT(exitFromAplication()));
    connect(ui->setRandomElem,SIGNAL(clicked(bool)),this,SLOT(randomElem()));
    connect(ui->optimalGraph,SIGNAL(pressed()),this,SLOT(drawOptimalGraph()));
    connect(ui->optimalGraph,SIGNAL(released()),this,SLOT(drawGraph()));

    //создаем элементы деталей для работы с пользователем, чтобы с помощью перестановки их пользователем
    //можно было менять порядок поступления деталей на станок
    setDetails();

    //соединяем сигналы изменения текста в полях текстового вода с функцией
    for(int i=0;i<ammountOfInput;i++)
    {
        connect(inputNumA[i],SIGNAL(textChanged(QString)),this,SLOT(valueTextCh()));
        connect(inputNumB[i],SIGNAL(textChanged(QString)),this,SLOT(valueTextCh()));
    }
    //запускает меню
    startMenu();
}

void UserI::startMenu()
{
    //смещает окно меню в начало, а окно plane вдаль
    ui->menu->setGeometry(0,0,739,425);
    ui->plane->setGeometry(1000,0,0,0);
}

void UserI::turnToPlane()
{
    //вызывается при нажатии кнопки start в меню
    //то же, что и с меню, но наоборот
    ui->menu->setGeometry(1000,0,0,0);
    ui->plane->setGeometry(0,0,739,425);
}
void UserI::changeNumDetails(int value)
{
    //вызывается при изменения значения слайдера, который отвечает за количество деталей
    //во первых, записывает в текстовое поле информацию, о текущем значении количества
    ui->numInpSlT->setText(QString::number(value));
    //убираем все поля, которые больше требуемого количества, оставляем лишь его количество
    for(int i=0;i<ammountOfInput;i++)
    {
        if(i<value)
        {
            inputNumA[i]->show();
            inputNumB[i]->show();
            inpNumPodp[i]->show();
        }
        else
        {
            inputNumA[i]->hide();
            inputNumB[i]->hide();
            inpNumPodp[i]->hide();
        }
    }
    //устанавливаем текущее количество деталей
    currentAmDetais = value;
    //изменяем так же и сами "динамические" детали
    setDetails();
    //сбрасываем все готовые графики, высчитанные значения для предыдущих настроек и прочее
    resetSet();
}

bool UserI::checkFields()
{
    //проверяет каждый текстовый input на наличие отсутствия текста или наличие символом, не являющихся числами
    bool isOneOf = false;
    bool isText =true;
    for(int i=0;i<currentAmDetais;i++)
    {
        if(inputNumA[i]->text()=="" || inputNumB[i]->text()=="")
        {
            isOneOf=true;
            ui->instruction_2->show();
            break;
        }
        else
        {
            //присваевает булевской переменной false, если текст не int
            inputNumA[i]->text().toInt(&isText);
            if(!isText)
            {
                isOneOf = true;
                ui->instruction_3->show();
                break;
            }
        }
    }
    if(isOneOf)
    {

        return false;
    }
    return true;
}

void UserI::setValues()
{
    //вызывается, когда пользователь нажимает на кнопку, которая устанавливает все введенные значения
    //и запускает прогон алгоритма, чтобы высчитать время и показать порядок оптимального
    if(!checkFields())
        return;
    resetSet();
    int * first = new int[currentAmDetais];
    int *second = new int[currentAmDetais];
    for(int i = 0;i<currentAmDetais;i++)
    {
            first[i]=inputNumA[i]->text().toInt();
            second[i]=inputNumB[i]->text().toInt();
    }
    //присваеваем экземпляру класса johnsonAlgorithm, текущие значения
    algo = new johnsonAlgorithm(first,second,currentAmDetais);
    //решаем задачу
    algo->solve();
    //получаем оптимальные индексы
    int * indexes = algo->getIndexes();
    //получаем оптимальное время
    int result = algo->getTime(indexes);
    //показываем пользователю на текстовых полях нужный порядок, немного изменяя их дизайн
    for(int i=0;i<currentAmDetais;i++)
    {
        inpNumPodp[i]->setText(inpNumPodp[i]->text() + " | " + QString::number(indexes[i]+1));
        inpNumPodp[i]->setStyleSheet("QLabel {font: bold 11px;"
                                     "border: 1px solid white;"
                                     "border-radius: 9px;"
                                     "color: rgb(200,255,200);}");
        inpNumPodp[i]->setGeometry(inpNumPodp[i]->geometry().x()-10,inpNumPodp[i]->geometry().y(),inpNumPodp[i]->geometry().width()+20,inpNumPodp[i]->geometry().height());
    }
    //открываем для пользователя информацию об оптимальном времени и даём доступ к кнопке прогона
    //алгоритма для порядка деталей, который выберет пользователь
    ui->minTimeTSec->setText(QString::number(result) + "s");
    ui->minTimeT->show();
    ui->minTimeTSec->show();
    ui->setButtonForCurrentOrder->show();
}

void UserI::valueTextCh()
{
    //всякий раз, когда пользователь изменит значения текстовых input, убираем ифнормацию о том,
    //что пользователь ввёл не те значения или вовсе что то не ввёл
    ui->instruction_2->hide();
    ui->instruction_3->hide();
}

void UserI::drawOptimalGraph()
{
    //рисует график для оптимального порядка при удержании на кнопку
    ui->lathe1T->show();
    ui->lathe2T->show();
    ui->sumTimeT->show();
    ui->sumTimeT->setText(ui->minTimeTSec->text());
    //ui->paintWidget - виджет на сцене, который имеет класс graph, который отвечает за рисовку графа
    //поэтому он как экземпляр этого класса и все обращения ведутся через этот виджет
    ui->paintWidget->setSettings(algo->getIndexes(),QPoint(20,10),algo);
    ui->paintWidget->paintGraph();
}

void UserI::setDesignSettitgsStart()
{
    //устанавливаем дизайн для меню

    ui->startButton->setStyleSheet("QPushButton {background-color: rgb(240,240,240,0);"
                                   "border: none;"
                                   "font: bold 24px;"
                                   "color: rgb(30,200,140)}"
                                   "QPushButton:pressed {background-color: rgb(80,90,80,0);"
                                   "border: none;"
                                   "color: rgb(200,100,180)}");


}


void UserI::setDesignSettingsProc()
{
    //устанавливаем дизайн для поля работы с программой
    //присваеваем массивам элементы графических элементов, чтобы в последствии было удобнее работать
    //а также создаем новые
    ui->plane->setParent(this);
    inputNumA[0] = ui->inpNumA1;
    inputNumA[1] = ui->inpNumA1_2;
    inputNumA[2] = ui->inpNumA1_3;
    inputNumA[3] = ui->inpNumA1_4;
    inputNumA[4] = ui->inpNumA1_5;
    inputNumA[5] = ui->inpNumA1_6;

    inputNumB[0] = ui->inpNumA1_7;
    inputNumB[1] = ui->inpNumA1_8;
    inputNumB[2] = ui->inpNumA1_9;
    inputNumB[3] = ui->inpNumA1_10;
    inputNumB[4] = ui->inpNumA1_11;
    inputNumB[5] = ui->inpNumA1_12;


    labels = new QLabel*[currentAmDetais];

    ui->minTimeT->hide();
    ui->minTimeTSec->hide();
    ui->instruction_2->hide();
    ui->instruction_3->hide();
    ui->minTimeT_2->hide();
    ui->minTimeTSec_2->hide();
    ui->setButtonForCurrentOrder->hide();
    ui->lathe1T->hide();
    ui->lathe2T->hide();
    ui->sumTimeT->hide();

    ui->numberDetailSl->setStyleSheet("QSlider::groove:horizontal {border: none;"
                                      "height: 2px;"
                                      "background-color: rgb(1,1,1);"
                                      "margin: 2px 0;}"
                                      "QSlider::handle:horizontal {background-color: white;"
                                      "border: none;"
                                      "width: 20px;"
                                      "margin: -20px 0px;}");
    ui->numInpSlT->setStyleSheet("QLabel {font: bold 14px;"
                                 "border: 2px solid white;"
                                 "border-color: rgb(255, 29, 78);"
                                 "border-radius: 4px;"
                                 "color: rgb(255, 29, 78);}");

    for (int i=0;i<ammountOfInput;i++)
    {
        inputNumA[i]->move(QPoint(10+(i*(inputNumA[i]->width()+5)),80));
        inputNumB[i]->move(QPoint(10+(i*(inputNumA[i]->width()+5)),90+inputNumA[i]->height()));

        inpNumPodp[i] = new QLabel(QString::number(i+1)+QString(" detail"),ui->plane);
        inpNumPodp[i]->show();
        inpNumPodp[i]->setStyleSheet("QLabel {font: bold 11px;"
                                     "border: 1px solid white;"
                                     "border-radius: 9px;"
                                     "color: rgb(255,255,255);}");
        inpNumPodp[i]->setAlignment(Qt::AlignCenter);
        inpNumPodp[i]->setGeometry(10+inputNumA[i]->width()/2+(i*(inputNumA[i]->width()+5)),100+2*inputNumA[i]->height(),inputNumA[i]->width()/1.25,inputNumA[i]->height());
        inpNumPodp[i]->move(inpNumPodp[i]->pos().x()-inpNumPodp[i]->width()/2,inpNumPodp[i]->pos().y());
    }

}

void UserI::setDetails()
{
    //создает настройки для класса movementDetails, который отвечает за изменение порядка
    //деталей пользователем путём перетаскивания деталей
    //создаем массив иксов для каждой детали и статическую позицию по высоте
    int yPosition;
    yPosition = 250;
    int width =60;
    xPositions = new int[currentAmDetais];
    for(int i=0;i<currentAmDetais;i++)
    {
        xPositions[i]=20+i*(width+20);
    }
    //если первый запуск, создаем детали на сцене
    if(isFirst)
    {
       for(int i=0;i<currentAmDetais;i++)
       {
           labels[i] = new QLabel(QString::number(i+1));
           labels[i]->setStyleSheet("background-color: rgb(250, 255, 187);"
                                    "border: 1px solid white;"
                                    "border-color: rgb(255, 29, 78,0);"
                                    "border-radius: 10px;"
                                    "color: rgb(255, 0, 0);");
           labels[i]->setParent(ui->plane);
           labels[i]->setAlignment(Qt::AlignCenter);
           QFont fon;
           fon.setBold(true);
           fon.setPointSize(16);
           fon.setStyleName("MS Shell Dlg 2");
           labels[i]->setFont(fon);
           labels[i]->setObjectName("det" + QString::number(i+1));

           labels[i]->show();
       }
    }
    //ставим их в соответствующие положения
    for(int i=0;i<currentAmDetais;i++)
    {
        labels[i]->setGeometry(xPositions[i],yPosition,width,width);
    }
    //скрываем те, чьи номера больше количества текущих деталей
    for(int i=0;i<ammountOfInput;i++)
    {
        if(i<currentAmDetais)
            labels[i]->show();
        else
            labels[i]->hide();
    }
    //задаём настройки экземпляру, с которым работаем через конструктор
    moveDet = new movementDetails(xPositions,yPosition,labels,currentAmDetais);
    isFirst=false;

}

void UserI::resetSet()
{
    //ставим обратно все на свои места, скрываем все, что нужно, в общем, задаем форме первоначальный вид
    ui->minTimeT->hide();
    ui->minTimeTSec->hide();
    ui->instruction_2->hide();
    ui->instruction_3->hide();
    ui->minTimeT_2->hide();
    ui->minTimeTSec_2->hide();
    ui->setButtonForCurrentOrder->hide();
    ui->lathe1T->hide();
    ui->lathe2T->hide();
    ui->sumTimeT->hide();
    ui->paintWidget->clearGraph();
    ui->optimalGraph->hide();
    for(int i=0;i<currentAmDetais;i++)
    {
        inpNumPodp[i]->setText(QString::number(i+1)+QString(" detail"));
        inpNumPodp[i]->setGeometry(10+inputNumA[i]->width()/2+(i*(inputNumA[i]->width()+5)),100+2*inputNumA[i]->height(),inputNumA[i]->width()/1.25,inputNumA[i]->height());
        inpNumPodp[i]->move(inpNumPodp[i]->pos().x()-inpNumPodp[i]->width()/2,inpNumPodp[i]->pos().y());
        inpNumPodp[i]->setStyleSheet("QLabel {font: bold 11px;"
                                     "border: 1px solid white;"
                                     "border-radius: 9px;"
                                     "color: rgb(255,255,255);}");
    }
}

void UserI::drawGraph()
{
   //рисуем граф для порядка деталей, заданного пользователем
   ui->lathe1T->show();
   ui->lathe2T->show();
   ui->sumTimeT->show();
   ui->sumTimeT->setText(ui->minTimeTSec_2->text());
   ui->paintWidget->setSettings(moveDet->getIndexes(),QPoint(20,10),algo);
   ui->paintWidget->paintGraph();

}

void UserI::mousePressEvent(QMouseEvent *ex)
{
    //срабатывает, когда ма нажимаем мышкой по форме
    moveDet->setCursorTap(ex);
}
void UserI::mouseMoveEvent(QMouseEvent *ex)
{
    //когда двигаем мышью
    moveDet->movementCursor(ex);
}
void UserI::mouseReleaseEvent(QMouseEvent *ex)
{
    //когда отпускаем кнопку мыши
    moveDet->setUpCursorTap(ex);
}

void UserI::setOrder()
{
    //посчитываем время для порядка, установленного пользователем
    //забираем индексы порядка
    int *indexes = moveDet->getIndexes();

    //получаем время для данных индексов
    int result = algo->getTime(indexes);
    //выводим результат и граф
    ui->minTimeTSec_2->setText(QString::number(result) + "s");
    ui->minTimeT_2->show();
    ui->minTimeTSec_2->show();
    ui->optimalGraph->show();
    drawGraph();
}

void UserI::exitFromAplication()
{
    //выходим из программы по нажатию кнопки
    exit(0);
}

void UserI::randomElem()
{
    //задаём рандомное количество деталей и значений для них
    int ran = 1+rand()%6;
    ui->numberDetailSl->setValue(ran);
    for(int i=0;i<currentAmDetais;i++)
    {
        inputNumA[i]->setText(QString::number(1+rand()%5));
        inputNumB[i]->setText(QString::number(1+rand()%5));
    }
}


UserI::~UserI()
{
    //удаляем все переменные, использующие динамическую память
    delete[] inputNumA;
    delete[] inpNumPodp;
    delete[] inputNumB;
    delete algo;
    delete ui;
}
