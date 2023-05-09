#ifndef TEACHER_H
#define TEACHER_H

#include <QMainWindow>
#include "dboperator.h"
#include <QSqlQueryModel>
#include<QDate>
#include<QTimer>
#include<QSqlTableModel>
namespace Ui {
class Teacher;
}

class Teacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Teacher(QWidget *parent = nullptr);
    ~Teacher();
    void SendSlot();
    void queryTableView();
    void userTableView();

private slots:
    void on_Book_clicked();

    void on_User_clicked();

    void on_BookInsertButton_clicked();

    void on_BookDeleteButton_clicked();

    void on_BookSearchButton_clicked();

    void on_UserSearch_clicked();

    void on_RemindToReturnBooks_clicked();

    void on_Freeze_clicked();

    void on_Unfreeze_clicked();

    void updateDate();

    void on_BookSaveButton_clicked();

    void slot_tooltip(QModelIndex index);

    void on_BookWriteAll_clicked();

    void on_ALLBookSure_clicked();

    void on_return_2_clicked();

    void on_userWrite_clicked();

    void on_sureToWrite_clicked();

    void on_returnToUser_clicked();

    void on_UserInfosearch_clicked();

    void on_Exit_clicked();

    void on_dealbook_clicked();

    void on_return_3_clicked();

    void buttonUpdate();

    void on_deleteAccount_clicked();

signals:
    void Back();

private:
    Ui::Teacher *ui;
    DBOperator dbp;
    QSqlQueryModel* model;
    QDate currentDate;
    QTimer* qtimer;
    QTimer* timer1;
    QSqlTableModel* model1;
    bool User;
    bool Book;
};

#endif // TEACHER_H
