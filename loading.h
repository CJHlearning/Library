#ifndef LOADING_H
#define LOADING_H

#include <QDialog>

namespace Ui {
class Loading;
}

class Loading : public QDialog
{
    Q_OBJECT

public:
    explicit Loading(QWidget *parent = nullptr);
    ~Loading();

private:
    Ui::Loading *ui;

private slots:
    void Progress();
};

#endif // LOADING_H
