#ifndef RETURNITEM_H
#define RETURNITEM_H

#include <QMainWindow>
#include"dboperator.h"
#include<QDate>
namespace Ui {
class test;
}

class ReturnItem : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReturnItem(QWidget *parent = nullptr);
    ~ReturnItem();
     void setReturnBookinfo(QString bookName, QDate borrowDate, QDate returnDate);
     void setValues(QString bookname, QString borroweraccount, QString isbn , int remain, bool isTeacher);
private slots:
     void on_pushButton_clicked();
signals:
     void Clear();
private:
    Ui::test *ui;
    DBOperator dbp;
    QString bookName;
    QString borrowerAccount;
    QString ISBN;
    int Remain;
    QDate currentDate;
    int daysremain;
    bool unreturned;
    bool isteacher;
};

#endif // RETURNITEM_H
