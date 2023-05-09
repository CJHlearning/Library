#ifndef BORROWITEM_H
#define BORROWITEM_H

#include <QMainWindow>
#include"dboperator.h"

namespace Ui {
class BorrowItem;
}

class BorrowItem : public QMainWindow
{
    Q_OBJECT

public:
    explicit BorrowItem(QWidget *parent = nullptr);
    ~BorrowItem();
    void setBookInfo(QString bookName, QString author, QString press, QString category, QString ISBN, QString bookput, int remain);
    void setValues(QString BookName, QString Account, QString isbn, int remain);
private slots:
    void on_BorrowButton_clicked();
    void on_LookButton_clicked();

signals:
     void Reset();

private:
    Ui::BorrowItem *ui;
    QString bookName;
    QString borrowerAccount;
    QString ISBN;
    int Remain;
    DBOperator dbp;
    QString bookInfo[6];
};

#endif // BORROWITEM_H
