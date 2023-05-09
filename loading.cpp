#include "loading.h"
#include "qtimer.h"
#include "qwidget.h"
#include "ui_loading.h"
#include "QMovie"
#include "QDir"

Loading::Loading(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Loading)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QMovie* Load = new QMovie(":/new/load/astronaut.gif");
    ui->Load_gif->setMovie(Load);
    Load->start();
    ui->progressBar->setRange(0, 100);

    QTimer* timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(Progress()));
    timer->start(30);
}

Loading::~Loading()
{
    delete ui;
}

void Loading::Progress()
{
    static int numpro = 0;
    static int gifpro = -15;
    ui->progressBar->setValue(numpro++);
    ui->Load_gif->move(gifpro, 58);
    gifpro += 3;
}
