#include "student.h"
#include "ensure.h"
#include "qnamespace.h"
#include "ui_student.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include "returnitem.h"
#include <QListWidgetItem>
#include <QDebug>
#include "borrowitem.h"
#include <QTimer>
#include <QLocale>
#include <teacher.h>
Student::Student(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Student)
{
    ui->setupUi(this);
    connect(&ensure, &Ensure::Exit, this, &Student::Back);
    ui->stackedWidget->setCurrentWidget(ui->Default);
    dbp.DBopen();
    QTimer* timer = new QTimer(this);
    QTimer* timer1 = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(buttonUpdate()));
    timer->start(1000);
    timer1->start(10);

    QPalette pa(this->palette());
    QImage img = QImage(":/new/load/window.png");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);
    AS = false;
    LL = false;
    BS = false;
    pa.setBrush(QPalette::Window,*pic);
    this->setPalette(pa);
}

Student::~Student()
{
    dbp.DBclose();
    delete ui;
}
void Student::setAccount(QString account)
{
    Account = account;
}


void Student::on_AccountSettings_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->AccountSettingsWidget);
    AS = true;
    LL = false;
    BS = false;
    ui->AccountSettings->setDown(AS);
    ui->LendList->setDown(LL);
    ui->BookSearch->setDown(BS);
    QSqlQuery query = dbp.DBGetData();
    QString sqlSelect = QString("select * from user;");
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error");
    }
    while(query.next())
    {
        QString userName = query.value("userName").toString();
        QString userAccount = query.value("userAccount").toString();
        QString sQ1 = query.value("sQ1").toString();
        QString sA1 = query.value("sA1").toString();
        QString sQ2 = query.value("sQ2").toString();
        QString sA2 = query.value("sA2").toString();
        QString sQ3 = query.value("sQ3").toString();
        QString sA3 = query.value("sA3").toString();
        QString phone = query.value("phone").toString();
        QString email = query.value("email").toString();
        if(userAccount == this->Account)
        {
            ui->Name->setText(userName);
            ui->Name->setReadOnly(true);
            ui->Question1->setText(sQ1);
            ui->Question1->setReadOnly(true);
            ui->AnswerL1->setText(sA1);
            ui->AnswerL1->setReadOnly(true);
            ui->Question2->setText(sQ2);
            ui->Question2->setReadOnly(true);
            ui->AnswerL2->setText(sA2);
            ui->AnswerL2->setReadOnly(true);
            ui->Question3->setText(sQ3);
            ui->Question3->setReadOnly(true);
            ui->AnswerL3->setText(sA3);
            ui->AnswerL3->setReadOnly(true);
            ui->PhoneNum->setText(phone);
            ui->PhoneNum->setReadOnly(true);
            ui->lineEdit->setText(email);
            ui->lineEdit->setReadOnly(true);
            break;
        }
    }
}


void Student::on_BookSearch_clicked()
{
    ui->BorrowListWidget->clear();
    ui->stackedWidget->setCurrentWidget(ui->BookSearchWidget);
    AS = false;
    LL = false;
    BS = true;
    ui->AccountSettings->setDown(AS);
    ui->LendList->setDown(LL);
    ui->BookSearch->setDown(BS);
}


void Student::on_LendList_clicked()
{
    ui->listWidget->clear();
    ui->stackedWidget->setCurrentWidget(ui->LendListWidget);
    AS = false;
    LL = true;
    BS = false;
    ui->AccountSettings->setDown(AS);
    ui->LendList->setDown(LL);
    ui->BookSearch->setDown(BS);
    QSqlQuery query = dbp.DBGetData();
    QSqlQuery query1 = dbp.DBGetData();
    QString sqlSelect = QString("select * from borrowlist;");
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error");
    }

    while(query.next())
    {
        QString bookName = query.value("bookName").toString();
        QDate borrowDate = query.value("borrowDate").toDate();
        QDate returnDate = query.value("returnDate").toDate();
        QString borrowerAccount = query.value("borrowerAccount").toString();
        QString ISBN = query.value("ISBN").toString();
        int remain;
        QString sqlSelect1 = QString("select * from bookdata;");
        if(!query1.exec(sqlSelect1))
        {
             QMessageBox::warning(this,"警告","selectremain error");
        }
        while(query1.next())
        {
            QString isbn = query1.value("ISBN").toString();
            if(isbn == ISBN)
            {
                remain = query1.value("remain").toInt();
                break;
            }
        }
        if(borrowerAccount == this->Account)
        {
            ReturnItem* returnlistitem = new ReturnItem;

            returnlistitem->setValues(bookName,this->Account,ISBN,remain,false);

            returnlistitem->setReturnBookinfo(bookName,borrowDate,returnDate);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(QSize(856,58));

            ui->listWidget->addItem(item);

            ui->listWidget->setItemWidget(item,returnlistitem);

            connect(returnlistitem,&ReturnItem::Clear,this,&Student::on_LendList_clicked);         
        }
    }
}


void Student::on_Exit_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Default);
    //ensure.show();
    QMessageBox messageBox(QMessageBox::Question, "提示", "是否确定退出登录", QMessageBox::Yes|QMessageBox::No, this);
    if(messageBox.exec() == QMessageBox::Yes)
    {
        emit Back();
    }
    else
        return;
}


void Student::on_SearchButton_clicked()
{
    ui->AccountSettings->setDown(AS);
    ui->LendList->setDown(LL);
    ui->BookSearch->setDown(BS);
    ui->BorrowListWidget->clear();
    QString bookName = ui->bookNameEdit->text();
    QString authorName = ui->authorEdit->text();
    QString pressName = ui->pressEdit->text();
    QString category = ui->categoryEdit->text();
    QSqlQuery query = dbp.DBGetData();
    QString sqlSelect;

    if(bookName == "" && authorName == "" && pressName == "" && category =="" )
    {
        sqlSelect = QString("select * from bookdata");

    }
    else
    {
        sqlSelect = QString("select * from bookdata "
                                "where (bookname like '%"+bookName+"%') "
                                "and (author like '%"+authorName+"%') "
                                "and (press like '%"+pressName+"%') "
                                "and (category like '%"+category+"%')"
                                );
    }
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","查询失败！如无法解决请联系工作人员");
        return;
    }
    while(query.next())
    {
        QString BookName = query.value("bookname").toString();
        QString AuthorName = query.value("author").toString();
        QString PressName = query.value("press").toString();
        QString Category = query.value("category").toString();
        QString isbn = query.value("ISBN").toString();
        QString bookput = query.value("bookput").toString();
        int remainAmount = query.value("remain").toInt();
        BorrowItem* borrowItem = new BorrowItem;
        borrowItem->setBookInfo(BookName,AuthorName,PressName,Category,isbn,bookput,remainAmount);
        borrowItem->setValues(BookName,this->Account,isbn, remainAmount);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(829,44));
        ui->BorrowListWidget->addItem(item);
        ui->BorrowListWidget->setItemWidget(item,borrowItem);
        connect(borrowItem,&BorrowItem::Reset,this,&Student::on_SearchButton_clicked);
    }
    ui->bookNameEdit->clear();
    ui->authorEdit->clear();
    ui->pressEdit->clear();
    ui->categoryEdit->clear();
}


void Student::on_EditButton_clicked()
{
    ui->AccountSettings->setDown(AS);
    ui->LendList->setDown(LL);
    ui->BookSearch->setDown(BS);
    ui->Name->setReadOnly(false);
    ui->Question1->setReadOnly(false);
    ui->AnswerL1->setReadOnly(false);
    ui->Question2->setReadOnly(false);
    ui->AnswerL2->setReadOnly(false);
    ui->Question3->setReadOnly(false);
    ui->AnswerL3->setReadOnly(false);
    ui->PhoneNum->setReadOnly(false);
    ui->lineEdit->setReadOnly(false);
}


void Student::on_SaveButton_clicked()
{
    ui->AccountSettings->setDown(AS);
    ui->LendList->setDown(LL);
    ui->BookSearch->setDown(BS);
    QString userName = ui->Name->text();
    QString sQ1 = ui->Question1->text();
    QString sA1 = ui->AnswerL1->text();
    QString sQ2 = ui->Question2->text();
    QString sA2 = ui->AnswerL2->text();
    QString sQ3 = ui->Question3->text();
    QString sA3 = ui->AnswerL3->text();
    QString phone = ui->PhoneNum->text();
    QString email = ui->lineEdit->text();
    QSqlQuery query = dbp.DBGetData();
    QString sqlUpdate = QString("update user "
                                "set userName = '%1', sQ1 = '%2', sA1 = '%3', "
                                "sQ2 = '%4', sA2 = '%5', sQ3 = '%6', "
                                "sA3 = '%7', phone = '%8', email = '%9' "
                                "where userAccount = '%10' ").arg(userName).arg(sQ1).arg(sA1).arg(sQ2).arg(sA2).arg(sQ3)
            .arg(sA3).arg(phone).arg(email).arg(this->Account);
    if(!query.exec(sqlUpdate))
    {
        QMessageBox::warning(this,"警告","保存失败!");
        return;
    }
    else
    {
        QMessageBox::information(this,"恭喜","保存成功!");
    }

    ui->Name->setReadOnly(true);
    ui->Question1->setReadOnly(true);
    ui->AnswerL1->setReadOnly(true);
    ui->Question2->setReadOnly(true);
    ui->AnswerL2->setReadOnly(true);
    ui->Question3->setReadOnly(true);
    ui->AnswerL3->setReadOnly(true);
    ui->PhoneNum->setReadOnly(true);
    ui->lineEdit->setReadOnly(true);
}

void Student::buttonUpdate()
{
    if(AS)
    {
        if(!(ui->AccountSettings->isDown()))
        {
            ui->AccountSettings->setDown(AS);
        }
    }
    else if(LL)
    {
        if(!(ui->LendList->isDown()))
        {
            ui->LendList->setDown(LL);
        }
    }
    else if(BS)
    {
        if(!(ui->BookSearch->isDown()))
        {
            ui->BookSearch->setDown(BS);
        }
    }
}

void Student::timeUpdate()
{
    // 显示时间
    QFont font("Microsoft YaHei",15,50);
    QDateTime time = QDateTime::currentDateTime();
    QString str = "yyyy/MM/dd hh:mm:ss dddd";
    QLocale locale = QLocale::Chinese;
    QString Cnstr = locale.toString(time, str);
    ui->Time->setFont(font);
    this->ui->Time->setText(Cnstr);

}

void Student::on_changeButton_clicked()
{
    ui->AccountSettings->setDown(AS);
    ui->LendList->setDown(LL);
    ui->BookSearch->setDown(BS);
    change.GetAccout(this->Account);
    change.setWindowFlags(Qt::Dialog);
    change.setWindowModality(Qt::ApplicationModal);
    change.show();
}

