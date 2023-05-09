#include "ensure.h"
#include "ui_ensure.h"

Ensure::Ensure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ensure)
{
    ui->setupUi(this);
}

Ensure::~Ensure()
{
    delete ui;
}

void Ensure::on_Y_clicked()
{
    this->close();
    emit Exit();
}


void Ensure::on_N_clicked()
{
    this->close();
}

