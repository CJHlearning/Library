#include "dboperator.h"
#include <QApplication>
DBOperator::DBOperator()
{
    openflag = false;
}

void DBOperator::DBopen()
{
    QString dbn;
    if(!(openflag))
    {
        db = QSqlDatabase::addDatabase("QODBC");
        dbn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C:/LIBDB/libManaSys.mdb");
        db.setDatabaseName(dbn);
        if(!db.open())
        {
            qDebug() << "数据库打开失败！" << db.lastError();
        }
        openflag = true;
    }
}

void DBOperator::DBclose()
{
    db.close();
    openflag = false;
}

QSqlQuery DBOperator::DBGetData()
{
    QSqlQuery query = QSqlQuery(db);
    return query;
}

