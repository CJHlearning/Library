#ifndef ENSURE_H
#define ENSURE_H

#include <QDialog>

namespace Ui {
class Ensure;
}

class Ensure : public QDialog
{
    Q_OBJECT

public:
    explicit Ensure(QWidget *parent = nullptr);
    ~Ensure();

private slots:
    void on_Y_clicked();

    void on_N_clicked();

private:
    Ui::Ensure *ui;

signals:
    void Exit();
};

#endif // ENSURE_H
