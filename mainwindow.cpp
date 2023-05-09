#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(600,300);
    connect(&TeacherLogin,&TeacherLogin::BackFromTea, this, &MainWindow::BackFromTeacherLogin);
    connect(&StudentLogin, &StuLogin::BackToMenu, this, &MainWindow::BackFromStudentLogin);
    connect(&StudentLogin, &StuLogin::Exit, this, &MainWindow::show);
    connect(&TeacherLogin, &TeacherLogin::TeacherLoginExit, this, &MainWindow::BackFromTeacherLogin);
    QPalette pa(this->palette());
    QImage img = QImage(":/new/load/main.jpg");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);
    pa.setBrush(QPalette::Window,*pic);
    this->setPalette(pa);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_StudentBtn_clicked()
{
    StudentLogin.show();
    this->hide();

}


void MainWindow::on_TeacherBtn_clicked()
{
    TeacherLogin.show();
    this->hide();

}


void MainWindow::BackFromTeacherLogin()
{
    this->show();
    TeacherLogin.hide();
}

void MainWindow::BackFromStudentLogin()
{
    this->show();
    StudentLogin.hide();
}
