#ifndef DBOPERATOR_H
#define DBOPERATOR_H

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
class DBOperator
{
private:
    QSqlDatabase db;
    bool openflag;
public:
    DBOperator();
    void DBopen();
    void DBclose();
    QSqlQuery DBGetData();
};

#endif // DBOPERATOR_H
