#include "useri.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //создаем форму
    QApplication a(argc, argv);
    UserI w;
    w.setWindowTitle("Johnson Algorithm");
    w.show();

    return a.exec();
}
