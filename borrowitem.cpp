#include "borrowitem.h"
#include "ui_borrowitem.h"
#include<QDate>
#include<QSqlQuery>
#include<QMessageBox>

BorrowItem::BorrowItem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BorrowItem)
{
    ui->setupUi(this);
}

BorrowItem::~BorrowItem()
{
    delete ui;
}

void BorrowItem::setBookInfo(QString bookName, QString author, QString press, QString category, QString ISBN, QString bookput, int remain)
{
    bookInfo[0] = bookName;
    bookInfo[1] = author;
    bookInfo[2] = press;
    bookInfo[3] = category;
    bookInfo[4] = ISBN;
    bookInfo[5] = bookput;
    ui->bookNamelabel->setText(bookName);
    ui->authorNamelabel->setText(author);
    ui->pressNamelabel->setText(press);
    ui->caterogyNamelabel->setText(category);
    ui->remainAmountlabel->setText(QString::number(remain));
}

void BorrowItem::setValues(QString BookName, QString Account, QString isbn , int remain)
{
    bookName = BookName;
    borrowerAccount = Account;
    ISBN = isbn;
    Remain = remain;
}

void BorrowItem::on_BorrowButton_clicked()
{
    QSqlQuery query = dbp.DBGetData();
    QDate borrowDate = QDate::currentDate();
    QDate returnDate = borrowDate.addDays(30);
    QString username;
    int borrowBook;
    QString sqlSelect = QString("select * from user;");
    QString sqlSelect1 = QString("select * from borrowlist;");
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error");
    }
    while(query.next())
    {
        QString userAccount = query.value("userAccount").toString();
        if(userAccount == this->borrowerAccount)
        {
            username = query.value("userName").toString();
            borrowBook = query.value("amountOfBook").toInt();
        }
    }
    QString sqlUpdate = QString("update user "
                                 "set amountOfBook = %1 "
                                 "where userAccount = '%2'").arg(borrowBook + 1).arg(this->borrowerAccount);
    if(!query.exec(sqlUpdate))
    {
        QMessageBox::warning(this,"警告","update error");
    }
    if(!query.exec(sqlSelect1))
    {
        QMessageBox::warning(this,"警告","select error");
    }
    while(query.next())
    {
        QString borrowerAccount = query.value("borrowerAccount").toString();
        QString bookName = query.value("bookName").toString();
        if(borrowerAccount == this->borrowerAccount)
        {
            if(bookName == this->bookName)
            {
                QMessageBox::information(this,"提示","您已经借过该书了~");
                return;
            }
        }
    }
    QString sqlUpdate1 = QString("update bookdata "
                                 "set remain = %1 "
                                 "where ISBN = '%2'").arg(this->Remain - 1).arg(this->ISBN);
    QString sqlInsert = QString("insert into borrowlist "
                                "values('%1','%2','%3','%4','%5','%6', %7)").arg(this->borrowerAccount).arg(username)
            .arg(this->bookName).arg(this->ISBN)
            .arg(borrowDate.toString("yyyy/MM/dd")).arg(returnDate.toString("yyyy/MM/dd"))
            .arg(false);
    if(this->Remain > 0)
    {
        if(!query.exec(sqlUpdate1) || !query.exec(sqlInsert))
        {
             QMessageBox::warning(this,"警告","借阅失败，请联系工作人员");
        }
        else
        {    QMessageBox::information(this,"提示","请将借阅的书籍放在借书平台上。");
             QMessageBox::information(this,"恭喜","借阅成功!");
        }
    }
    else
    {
        QMessageBox::warning(this,"警告","书已被借光!");
    }
    emit Reset();
}


void BorrowItem::on_LookButton_clicked()
{   QString headinfo[]= {"书名","作者","出版社","分类","ISBN","索书号"};
    QString info;
    for(int i=0;i<6;i++){
        info+=headinfo[i]+ ":" + bookInfo[i] + "\n";
    }
    QMessageBox information;
    information.setWindowTitle("书籍信息");
    information.setText(info);
    information.setStyleSheet("QLabel{"
                              "font-size : 16px;}");
    information.exec();
}

