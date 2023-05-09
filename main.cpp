#include "mainwindow.h"
#include "loading.h"
#include <QApplication>
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Loading load;
    MainWindow w;
    load.show();
    float delayTime = 3.1;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime*1000))
    {
        a.processEvents();
    }
    w.show();
    load.close();

    return a.exec();
}
