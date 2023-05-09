#include "returnitem.h"
#include "ui_test.h"
#include<QSqlQuery>
#include<QMessageBox>

ReturnItem::ReturnItem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    currentDate = QDate::currentDate();
    this->unreturned = false;
    this->isteacher =false;
}

ReturnItem::~ReturnItem()
{
    delete ui;
}

void ReturnItem::setValues(QString bookname, QString borroweraccount ,QString isbn, int remain, bool isTeacher)
{
    isteacher=isTeacher;
    bookName = bookname;
    borrowerAccount = borroweraccount;
    ISBN = isbn;
    Remain = remain;
}

void ReturnItem::setReturnBookinfo(QString bookName, QDate borrowDate, QDate returnDate)
{
    QSqlQuery query = dbp.DBGetData();
    QString sqlUpdate = QString("update borrowlist "
                                "set unreturned = %1 "
                                "where ISBN = '%2' and borrowerAccount = '%3' ").arg(true).arg(this->ISBN).arg(this->borrowerAccount);
    ui->bookNameLabel->setText(bookName);
    ui->borrowDateLabel->setText(borrowDate.toString("yyyy/MM/dd"));
    ui->ReturnDateLabel->setText(returnDate.toString("yyyy/MM/dd"));
    int days = currentDate.daysTo(returnDate);
    daysremain=days;
    if(days >= 0)
    {
        ui->RemainTimelabel->setStyleSheet("QLabel {"
                                           "border: 2px solid #EE82EE;" /* 边框宽度为1px，颜色为#A0A0A0 */
                                           "border-radius: 10px;" /* 边框圆角 */
                                           "padding-left: 5px;" /* 文本距离左边界有5px */
                                           "font-size: 10pt;" /* 文本字体大小 */
                                           "background:#ffffff;"
                                           "color:#000000"
                                           "}");
        ui->RemainTimelabel->setText(QString::number(days)+"天");
    }
    else
    {
        if(!query.exec(sqlUpdate))
        {
            QMessageBox::warning(this,"警告","update error!");
            return;
        }
        this->unreturned = true;
        ui->RemainTimelabel->setStyleSheet("QLabel{"
                                           "border: 2px solid #EE82EE;" /* 边框宽度为1px，颜色为#A0A0A0 */
                                           "border-radius: 10px;" /* 边框圆角 */
                                           "padding-left: 5px;" /* 文本距离左边界有5px */
                                           "font-size: 10pt;" /* 文本字体大小 */
                                           "background:#ff0000;"
                                           "color:#ffffff"
                                           "}");
        ui->RemainTimelabel->setText("逾期未还！");
    }
}

void ReturnItem::on_pushButton_clicked()
{       //还书按钮
    QSqlQuery query = dbp.DBGetData();
    int borrowBook;
    int bookUnreturned;
    QString sqlSelect = QString("select * from user");
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error");
        return;
    }
    while(query.next())
    {
        QString account = query.value("userAccount").toString();
        if(account == this->borrowerAccount)
        {
            borrowBook = query.value("amountOfBook").toInt();
            bookUnreturned = query.value("bookUnreturned").toInt();
        }
    }
    QString sqlDelete = QString("delete from [borrowlist]"
                                "where [bookName] = '%1' and [borrowerAccount] = '%2';").arg(this->bookName).arg(this->borrowerAccount);
    if(!query.exec(sqlDelete))
    {
        QMessageBox::warning(this,"警告","归还失败!");
        return;
    }
    else
    {
        QString sqlUpdate = QString("update bookdata "
                                    "set remain = %1 "
                                    "where ISBN = '%2'").arg(this->Remain + 1).arg(this->ISBN);
        if(!query.exec(sqlUpdate))
        {
            QMessageBox::warning(this,"警告","update error");
        }
        else
        {
            if(this->unreturned)
            {
                 sqlUpdate = QString("update user "
                                     "set bookUnreturned = %1, "
                                     "amountOfBook = %2 "
                                     "where userAccount = '%3'").arg(bookUnreturned - 1).arg(borrowBook - 1).arg(this->borrowerAccount);
            }
            else
            {
                sqlUpdate = QString("update user "
                                    "set amountOfBook = %1 "
                                    "where userAccount = '%2'").arg(borrowBook - 1).arg(this->borrowerAccount);
            }
            if(!query.exec(sqlUpdate))
            {
                QMessageBox::warning(this,"警告","error");
            }
            if(daysremain<0 && isteacher==false){
                QMessageBox::warning(this,"警告",QString("图书已逾期！请找管理员还书，缴纳逾期费%1元！").arg(daysremain));
            }
            else if(daysremain>=0 && isteacher==false){
                QMessageBox::information(this,"恭喜","归还成功!");
            }
            else{
                QMessageBox::warning(this,"提示",QString("请管理员根据情况进行处理（收缴逾期费或赔偿费）"));
                QMessageBox::information(this,"提示","归还成功!");
            }
        }
    }
    emit Clear();
}
