#include "teacher.h"
#include "ui_teacher.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QToolTip>
#include <QTextDocument>
#include <QTextBlock>
#include <QtSql/QSqlRecord>
#include <QList>
#include <QItemSelectionModel>
#include <QSet>
#include <returnitem.h>
#include <QListWidgetItem>
Teacher::Teacher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Teacher)
{
    ui->setupUi(this);
    connect(ui->Exit,&QPushButton::clicked,this,&Teacher::SendSlot);

   dbp.DBopen();

   model = new QSqlQueryModel(this);
   model1 = new QSqlTableModel(this);
   qtimer = new QTimer(this);
   timer1 = new QTimer(this);

   connect(qtimer,SIGNAL(timeout()),this,SLOT(updateDate()));
   connect(timer1, SIGNAL(timeout()), this, SLOT(buttonUpdate()));
   qtimer->start(1000);
   timer1->start(10);

   ui->UserTableView->setMouseTracking(true);
   connect(ui->UserTableView,SIGNAL(entered(QModelIndex)),this,SLOT(slot_tooltip(QModelIndex)));

   ui->BookTableView->setMouseTracking(true);
   connect(ui->BookTableView,SIGNAL(entered(QModelIndex)),this,SLOT(slot_tooltip(QModelIndex)));

   ui->stackedWidget->setCurrentWidget(ui->page_3);

   QPalette pa(this->palette());
   QImage img = QImage(":/new/load/window.png");
   img = img.scaled(this->size());
   QBrush *pic = new QBrush(img);

   pa.setBrush(QPalette::Window,*pic);
   //this->setAutoFillBackground(true);
   this->setPalette(pa);
   User = false;
   Book = false;
}

Teacher::~Teacher()
{
    dbp.DBclose();
    delete ui;
}

void Teacher::SendSlot()
{
    QMessageBox messageBox(QMessageBox::Question, "提示", "是否确定退出登录", QMessageBox::Yes|QMessageBox::No, this);
    if(messageBox.exec() == QMessageBox::Yes)
    {
        model1->clear();
        model->clear();
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        emit Back();
    }
    else
        return;
}


void Teacher::slot_tooltip(QModelIndex index)
{
    QToolTip::showText(QCursor::pos(),index.data().toString());
}

void Teacher::on_Book_clicked()
{

    ui->stackedWidget->setCurrentWidget(ui->BookPage);
    User = false;
    Book = true;
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    queryTableView();
}


void Teacher::on_User_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->UserPage);
    User = true;
    Book = false;
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    userTableView();
}

void Teacher::userTableView(){//显示所有用户信息
    QSqlQuery query = dbp.DBGetData();//按照逾期未还书排序来显示
    QString sqlSelect = QString("select userAccount, userName, amountOfBook, bookUnreturned, freezed, activated from user order by bookUnreturned DESC;");
    model->setQuery(sqlSelect);
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error");
        return;
    }
    //设置标题栏样式
    QString head[]={"用户账号","用户名称","借阅书籍总数","逾期未还书籍数量","是否冻结","是否激活"};
    for(int i=0;i<6;i++){
       model->setHeaderData(i,Qt::Horizontal,head[i]);
    }
    ui->UserTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                          "color: brown;font: 12pt;"
                                                          "background-color:rgba(255,230,230,210);"
                                                          "border: 1px solid white;"
                                                         "selection-background-color:rgba(255,200,200,210);"
                                                         "}"
                                                         );
    ui->UserTableView->setModel(model);
    ui->UserTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->UserTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->UserTableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);

}


void Teacher::queryTableView()
{
    model1 = new QSqlTableModel(this);//连接数据库
    model1->setTable("bookdata");
    model1->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if(!model1->select()) qDebug()<<"select error";
    ui->BookTableView->setModel(model1);
    //ui->BookTableView->setSortingEnabled(true);//开启排序
    //ui->BookTableView->verticalHeader()->setVisible(false);
//    ui->BookTableView->resizeRowsToContents();
//    ui->BookTableView->resizeColumnsToContents();
    QString head[]={"书名","作者","出版社","分类","ISBN","索书号","总数量","库存量"};
    for(int i=0;i<8;i++){
       model1->setHeaderData(i,Qt::Horizontal,head[i]);
    }
    ui->BookTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                          "color: brown;font: 12pt;"
                                                          "background-color:rgba(255,230,230,210);"
                                                          "border: 1px solid white;"
                                                         "selection-background-color:rgba(255,200,200,210);"
                                                         "}"
                                                         );
    ui->BookTableView->verticalHeader()->setStyleSheet("QHeaderView::section{"
                                                          "color: black;font: 10pt;"
                                                          "background-color:rgba(255,255,255,210);"
                                                          "}"
                                                         );
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
}

void Teacher::on_BookInsertButton_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    QMessageBox::StandardButton reply= QMessageBox::question(this,"是否插入","是否确认插入？");
    if (reply==QMessageBox::Yes){
        bool isFound = false;
        QString bookName = ui->BookNameEdit->text();
        QString author  = ui->AuthorEdit->text();
        QString press = ui->PressEdit->text();
        QString ISBN = ui->ISBNEdit->text();
        QString category = ui->BookTypeEdit->text();
        QString bookput =ui ->BookPutEdit->text();
        int number = ui->BookNumberEdit->text().toInt();
        if(bookName == "" || author == "" || press== "" || ISBN == "" || category == "" || number ==0 || bookput ==""){
            QMessageBox::warning(this,"提示","输入信息不全或数量为0！");
        }
        else{
            QSqlQuery query = dbp.DBGetData();
            QString sqlSelect = QString("select * from bookdata;");
            if(!query.exec(sqlSelect))
            {
                qDebug() << "select error";
            }
            while(query.next())
            {
                QString BookName = query.value("bookName").toString();
                QString Author  = query.value("author").toString();
                QString Press = query.value("press").toString();
                QString isbn = query.value("ISBN").toString();
                QString Category = query.value("category").toString();
                QString Bookput =query.value("bookput").toString();
                int Number = query.value("number").toInt();
                int Remain = query.value("remain").toInt();
                if(BookName == bookName && Author == author &&
                        Press == press && ISBN == isbn &&
                        Category == category && Bookput == bookput)
                {
                    isFound = true;
                    QString sqlUpdate = QString("update [bookdata]"
                                                "set [number] = %1, [remain] = %2 "
                                                "where [ISBN] = '%3';").arg(Number + number).arg(Remain + number).arg(isbn);
                    if(!query.exec(sqlUpdate))
                    {
                        QMessageBox::warning(this, "警告", "数据更新失败！");
                    }
                    else
                    {
                        QMessageBox::information(this, "提示", "数据更新成功！");
                    }
                    break;
                }
            }
            if(!isFound){
                QString insert = QString("INSERT INTO bookdata VALUES('%1', '%2', '%3', '%4', '%5', '%6', %7, %8)"
                                         ).arg(bookName).arg(author).arg(press).arg(category).arg(ISBN).arg(bookput).arg(number).arg(number);

                if(bookName!="" && author!="" && press!="" && ISBN!="" && category!="" && number!=0)
                {
                    if(query.exec(insert)==false)
                    {
                        QMessageBox::warning(this, "警告", "数据插入失败！");
                    }
                    else
                    {
                        QMessageBox::information(this, "提示", "数据插入成功！");
                    }
                }
            }
            ui->BookNameEdit->clear();
            ui->BookNumberEdit->clear();
            ui->BookTypeEdit->clear();
            ui->AuthorEdit->clear();
            ui->ISBNEdit->clear();
            ui->PressEdit->clear();
            ui->BookPutEdit->clear();
            queryTableView();
            }
      }

}


void Teacher::on_BookDeleteButton_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    QModelIndexList indexlist = ui->BookTableView->selectionModel()->selectedIndexes();
    QSet<int> rowlist;

    for(int i=0;i<indexlist.size();i++){
        rowlist.insert(indexlist[i].row());
    }
    int count = rowlist.size();
    int row = ui->BookTableView->currentIndex().row();
    if(count >1){
        QMessageBox::StandardButton answer= QMessageBox::question(this,"确认删除", QString("确定要删除选中的%1行图书信息吗？").arg(count));
        if (answer==QMessageBox::Yes){
            QSet<int>::iterator j=rowlist.begin();
            while(j!=rowlist.end()){
                model1 -> removeRow(*j);
                j++;
            }
                model1 -> submitAll();
            }
        }
    else if(count ==1 ){
        QString bookname= model1->record(row).value("bookName").toString();
        QString ISBN =  model1->record(row).value("ISBN").toString();
        QMessageBox::StandardButton answer= QMessageBox::question(this,"确认删除", "确定要删除《" +bookname+ "》 ISBN"+ISBN+"的这行图书信息吗？");
        if(answer == QMessageBox::Yes){
            model1 -> removeRow(row);
            model1 -> submitAll();
        }
    }
    else {
        QMessageBox::warning(this,"提示","请选择需要删除的图书信息!");
    }
}


void Teacher::on_BookSearchButton_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    QString bookName = ui->BookNameEdit->text();
    QString author  = ui->AuthorEdit->text();
    QString press = ui->PressEdit->text();
    QString ISBN = ui->ISBNEdit->text();
    QString category = ui->BookTypeEdit->text();
    int number = ui->BookNumberEdit->text().toInt();
    model1 = new QSqlTableModel(this);
    QSqlQuery query = dbp.DBGetData();
    QString sqlSelect = QString("select * from bookdata;");
    if(!query.exec(sqlSelect))
    {
        qDebug() << "select error";
    }

    //未输入，点击查询，显示所有书籍
    if(bookName == "" && author == "" && press == "" && ISBN == "" && category =="" && number == 0)
    {
        ui->BookTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        queryTableView();
    }
    else        //根据输入数据模糊进行查询
    {
        QString sqlSelect = QString(
                                    "(bookname like '%"+bookName+"%') "
                                    "and (author like '%"+author+"%') "
                                    "and (press like '%"+press+"%') "
                                    "and (category like '%"+category+"%')"
                                    );

        model1->setTable("bookdata");
        model1->setFilter(sqlSelect);
        model1->setEditStrategy(QSqlTableModel::OnManualSubmit);
        if(!(model1->select()))
        {
            QMessageBox::warning(this,"警告", "查询失败");
        }
        ui->BookTableView->setModel(model1);
        ui->BookTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        QString head[]={"书名","作者","出版社","分类","ISBN","索书号","总数量","库存量"};
        for(int i=0;i<8;i++){
           model1->setHeaderData(i,Qt::Horizontal,head[i]);
        }
        ui->BookTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                              "color: brown;font: 12pt;"
                                                              "background-color:rgba(255,230,230,210);"
                                                              "border: 1px solid white;"
                                                             "selection-background-color:rgba(255,200,200,210);"
                                                             "}"
                                                             );
        ui->BookTableView->verticalHeader()->setStyleSheet("QHeaderView::section{"
                                                              "color: black;font: 10pt;"
                                                              "background-color:rgba(255,255,255,210);"
                                                              "}"
                                                             );
        ui->BookTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->BookTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
        ui->BookTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
        ui->BookTableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
        ui->BookTableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
    }

    QString head[]={"书名","作者","出版社","分类","ISBN","索书号","总数量","库存量"};
    for(int i=0;i<8;i++){
       model1->setHeaderData(i,Qt::Horizontal,head[i]);
    }
    ui->BookTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                          "color: brown;font: 12pt;"
                                                          "background-color:rgba(255,230,230,210);"
                                                          "border: 1px solid white;"
                                                         "selection-background-color:rgba(255,200,200,210);"
                                                         "}"
                                                         );
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    ui->BookTableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);


//    QString head[]={"书名","作者","出版社","分类","ISBN","索书号","总数量","库存量"};
//    for(int i=0;i<8;i++){
//       model1->setHeaderData(i,Qt::Horizontal,head[i]);
//    }
//    ui->BookTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{"
//                                                          "color: brown;font: 12pt;"
//                                                          "background-color:rgba(255,230,230,210);"
//                                                          "border: 1px solid white;"
//                                                         "selection-background-color:rgba(255,200,200,210);"
//                                                         "}"
//                                                         );

    ui->BookNameEdit->clear();
    ui->BookNumberEdit->clear();
    ui->BookTypeEdit->clear();
    ui->AuthorEdit->clear();
    ui->ISBNEdit->clear();
    ui->PressEdit->clear();
}


void Teacher::on_UserSearch_clicked()
{//显示用户的借阅情况
    ui->Book->setDown(Book);
    ui->User->setDown(User);

    QString userName = ui->UserName->text();
    QSqlQuery query = dbp.DBGetData();
    QString sqlSelect1 = QString("select * from borrowlist");
    bool exist = false;
    query.exec(sqlSelect1);
    if(userName==""){//如果为空，显示所有借书情况
        QString sqlSelect = QString("select * from borrowlist");
        if(!query.exec(sqlSelect))
        {
            QMessageBox::warning(this, "警告", "查询错误！");
        }
        model->setQuery(sqlSelect);
    }

    else{//如果不为空，显示搜索用户的情况
        while(query.next())
        {
            QString Borrower = query.value("borrower").toString();
            QString BorrowerAccount = query.value("borrowerAccount").toString();
            if(userName == BorrowerAccount || userName == Borrower)
            {
                exist = true;
                break;
            }
        }

        if(exist == true)
        {
            QString sqlSelect = QString("select * from borrowlist where borrower = '%1' or borrowerAccount = '%1';").arg(userName);
            if(!query.exec(sqlSelect))
            {
                QMessageBox::warning(this, "警告", "查询错误！");
            }
            model->setQuery(sqlSelect);
        }
        else
        {
            model->clear();
            QMessageBox::information(this, "提醒", "该用户不存在或未借书！");
        }
    }
        //设置标题栏样式
    ui->UserTableView->setModel(model);
    QString head[]={"账号","名称","书名","ISBN","借阅时间","应还时间"};
    for(int i=0;i<6;i++){
       model->setHeaderData(i,Qt::Horizontal,head[i]);
    }
    ui->UserTableView->horizontalHeader()->hideSection(6);
    ui->UserTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                          "color: brown;font: 12pt;"
                                                          "background-color:rgba(255,230,230,210);"
                                                          "border: 1px solid white;"
                                                         "selection-background-color:rgba(255,200,200,210);"
                                                         "}"
                                                         );
    ui->UserTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->UserName->clear();
}


void Teacher::on_RemindToReturnBooks_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    QString userName = ui->UserName->text();
    QSqlQuery query = dbp.DBGetData();
    QString sqlRemind = QString("update user set bookUnreturned = true where userAccount = '%1';").arg(userName);
    if(!query.exec(sqlRemind))
    {
        QMessageBox::warning(this, "警告","提醒失败");
    }
    else
    {
        QMessageBox::information(this, "提示", "提醒成功");
    }
    userTableView();
}


void Teacher::on_Freeze_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    int row=ui->UserTableView->currentIndex().row();
    QString userName = model->record(row).value(0).toString();
    if(userName=="") {
        QMessageBox::warning(this,"提示","请点击选择用户账号！");
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认冻结","是否确认冻结用户 "+userName+"?");
    if(reply==QMessageBox::Yes){
        QSqlQuery query=dbp.DBGetData();
        QString sqlFreeze = QString("update user set freezed = true where userAccount = '%1';").arg(userName);
        if(!query.exec(sqlFreeze))
        {
            QMessageBox::warning(this, "警告","冻结失败");
        }
        else
        {
            QMessageBox::information(this, "提示", "冻结成功");
        }
        userTableView();
    }
}


void Teacher::on_Unfreeze_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    int row = ui->UserTableView->currentIndex().row();
    QString userName = model->record(row).value(0).toString();
    if(userName=="") {
        QMessageBox::warning(this,"提示","请点击选择用户账号！");
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认解冻","是否确认解冻用户 "+userName+"?");
    if(reply==QMessageBox::Yes){
        QSqlQuery query = dbp.DBGetData();
        QString sqlUnFreeze = QString("update user set freezed = false where userAccount = '%1';").arg(userName);
        if(!query.exec(sqlUnFreeze))
        {
            QMessageBox::warning(this, "警告","解冻失败");
        }
        else
        {
            QMessageBox::information(this, "提示", "解冻成功");
        }
        userTableView();
    }
}

void Teacher::updateDate()
{
    currentDate = QDate::currentDate();
    QSqlQuery query = dbp.DBGetData();
    QSqlQuery query1 = dbp.DBGetData();
    QSqlQuery query2 = dbp.DBGetData();
    QSqlQuery query3 = dbp.DBGetData();
    QString sqlSelect = QString("select * from user");
    QString sqlSelect1 = QString("select * from borrowlist");
    if(!query.exec(sqlSelect))
    {
       QMessageBox::warning(this, "警告","select error");
       return;
    }
    while(query.next())
    {
       QString Account = query.value("userAccount").toString();
       int bookUnreturned = 0;
       if(!query1.exec(sqlSelect1))
       {
            QMessageBox::warning(this, "警告","select error");
            return;
       }
       while(query1.next())
       {
           QString ISBN = query1.value("ISBN").toString();
           QDate returnDate = query1.value("returnDate").toDate();
           if(Account == query1.value("borrowerAccount").toString() && currentDate.daysTo(returnDate) < 0)
           {
               bookUnreturned++;
               QString sqlUpdate = QString("update borrowlist "
                                           "set unreturned = %1 "
                                           "where ISBN = '%2' and borrowerAccount = '%3'").arg(true).arg(ISBN).arg(Account);
               if(!query2.exec(sqlUpdate))
               {
                   QMessageBox::warning(this, "警告","update error");
               }
           }
       }
       QString sqlUpdate = QString("update user "
                                   "set bookUnreturned = %1 "
                                   "where userAccount = '%2' ").arg(bookUnreturned).arg(Account);
       if(!query3.exec(sqlUpdate))
       {
           QMessageBox::warning(this, "警告","update error");
           return;
       }
    }
}

void Teacher::on_BookSaveButton_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    QMessageBox::StandardButton reply= QMessageBox::question(this,"确认保存","是否确认保存？",QMessageBox::Yes | QMessageBox::Cancel);
    if(reply==QMessageBox::Yes){
        model1->submitAll();
    }
}



void Teacher::on_BookWriteAll_clicked()
{
    //转入批量写入页面
    ui->stackedWidget->setCurrentWidget(ui->ALLWritePage);
    ui->Book->setDown(Book);
    ui->User->setDown(User);
}



void Teacher::on_ALLBookSure_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    //将批量输入框的内容逐行读入并写入数据库中
    QMessageBox::StandardButton reply=QMessageBox::question(this,"提示","你确定要进行批量插入操作吗？",QMessageBox::Yes | QMessageBox::Cancel);
    if(reply== QMessageBox::Cancel) return;//如果用户点击取消，那么终止操作

    QTextDocument* doc = ui->BookWriteText->document();//读出输入框中的所有内容
    int cnt=doc->blockCount();//读出有多少行信息

    int row=0;
    QMessageBox::warning(this,"提示",QString("检测到一共有%1行图书信息").arg(cnt));
    for(int i=0;i<cnt;i++){
        QSqlQuery query = dbp.DBGetData();
        row=i;
        QTextBlock textline = doc->findBlockByNumber(i);
        QString str = textline.text().trimmed();
        QStringList info = str.split('\t');//分割字符串
        QStringList::Iterator currentinfo=info.begin();
        bool isFound = false;
        if(info.length()<7){
            QMessageBox::warning(this,"提示",QString("第 %1 行输入的信息不全,插入失败,请修改后再进行！").arg(i+1));
            break;
        }

        else{

        QString bookName = (*currentinfo).trimmed();
        currentinfo++;
        QString author  =(*currentinfo).trimmed();
        currentinfo++;
        QString press =(*currentinfo).trimmed();
        currentinfo++;
        QString category =(*currentinfo).trimmed();
        currentinfo++;
        QString ISBN = (*currentinfo).trimmed();
        currentinfo++;
        QString bookput = (*currentinfo).trimmed();
        currentinfo++;
        int number = (*currentinfo).trimmed().toInt();
        int rnumber=-1;
//        if(info.length()>6){
//            currentinfo++;//如果输入了剩余库存数量remain，就覆盖原来的remain(如果有的话);如果没有remain输入，就采用insert原来的办法
//            rnumber=(*currentinfo).trimmed().toInt();
//        }
        QString sqlSelect = QString("select * from bookdata;");

        if(!query.exec(sqlSelect))
        {
            qDebug() << "select error";
        }
        while(query.next())
        {
            QString BookName = query.value("bookName").toString();
            QString Author  = query.value("author").toString();
            QString Press = query.value("press").toString();
            QString isbn = query.value("ISBN").toString();
            QString Category = query.value("category").toString();
            QString Bookput =query.value("bookput").toString();
            int Number = query.value("number").toInt();
            int Remain = query.value("remain").toInt();
            if(BookName == bookName && Author == author &&
                    Press == press && ISBN == isbn && Bookput == bookput &&
                    Category == category)
            {
                isFound = true;
                if(rnumber==-1)rnumber=Remain+number;
                QString sqlUpdate = QString("update [bookdata]"
                                            "set [number] = %1, [remain] = %2 "
                                            "where [ISBN] = '%3';").arg(Number + number).arg(rnumber).arg(isbn);
                if(!query.exec(sqlUpdate))
                {
                    QMessageBox::warning(this, "警告",QString("第%1行 ").arg(i+1)+BookName+" "+ISBN+" 数据更新失败！");
                    break;
                }
                else
                {
                    //QMessageBox::information(this, "提示", "数据更新成功！");
                }
                break;
            }
        }
        if(!isFound){
            if(rnumber==-1)rnumber=number;
            QString insert = QString("INSERT INTO bookdata VALUES('%1', '%2', '%3', '%4', '%5', '%6', %7 , %8)"
                                     ).arg(bookName).arg(author).arg(press).arg(category).arg(ISBN).arg(bookput).arg(number).arg(rnumber);

                if(query.exec(insert)==false)
                {
                    QMessageBox::warning(this, "警告", QString("第%1行 ").arg(row+1)+bookName+" "+ISBN+" 数据插入失败！");
                    break;
                }
                else
                {
                    //QMessageBox::information(this, "提示", "数据插入成功！");
                }
        }
    }
    }

    if(row+1==cnt)QMessageBox::warning(this, "提示", "批量数据插入操作已结束！");
    else QMessageBox::warning(this, "提示", QString("批量数据插入终止！目前已进行到第%1行，请注意不要将前面已插入的图书信息重复插入，否择库存将重复增加！").arg(row+1));
}


void Teacher::on_return_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->BookPage);
    ui->Book->setDown(Book);
    ui->User->setDown(User);
}

void Teacher::on_userWrite_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->userAllWrite);
    ui->Book->setDown(Book);
    ui->User->setDown(User);
}


void Teacher::on_sureToWrite_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    //将批量输入框的内容逐行读入并写入数据库中
    QMessageBox::StandardButton reply=QMessageBox::question(this,"提示","你确定要进行批量新学号录入吗？",QMessageBox::Yes | QMessageBox::Cancel);
    if(reply== QMessageBox::Cancel) return;//如果用户点击取消，那么终止操作

    QTextDocument* doc = ui->userAllNewAccount->document();//读出输入框中的所有内容
    int cnt=doc->blockCount();//读出有多少行信息
    int row=-1;
    QMessageBox::warning(this,"提示",QString("检测到一共有%1个学生账号").arg(cnt));
    for(int i=0;i<cnt;i++){
        QSqlQuery query = dbp.DBGetData();
        QTextBlock textline = doc->findBlockByNumber(i);
        QString newAccount = textline.text().trimmed();
        bool isFound = false;
        QString sqlSelect = QString("select * from user;");
        bool ok = true;
        newAccount.toInt(&ok);//判断是否是数字
        if(newAccount.length()!=8 || ok == false){
            QMessageBox::warning(this, "警告",QString("第%1行 ").arg(i+1)+" 学号:"+newAccount+"输入有误！");
            break;
        }
        else
        {

            if(!query.exec(sqlSelect))
            {
                qDebug() << "select error";
            }

            while(query.next())
            {
                QString useraccount=query.value("userAccount").toString();
                if(newAccount==useraccount)
                {   row=i;
                    isFound=true;
                    QMessageBox::warning(this, "警告",QString("第%1行 ").arg(i+1)+" 学号:"+useraccount+"已经存在！");
                    break;
                }
            }

            if(!isFound){
                QString insert = QString("INSERT INTO user(userAccount,password,activated) VALUES('%1', '%2', %3)"
                                         ).arg(newAccount).arg(newAccount.mid(2,6)).arg(false);
                    if(query.exec(insert)==false)
                    {
                        QMessageBox::warning(this, "警告", QString("第%1行 ").arg(row+1)+"学号:"+newAccount+"录入失败！");
                        break;
                    }
                    else
                    {
                        //QMessageBox::information(this, "提示", "数据插入成功！");
                        row=i;
                    }
            }
         }
    }
    if(row+1==cnt)QMessageBox::warning(this, "提示", "批量新学号录入操作已结束!\n新录入账号默认密码为账号后6位，请提醒用户进行激活。");
    else QMessageBox::warning(this, "提示", QString("批量数据插入终止！目前已进行到第%1行。").arg(row+1));
}


void Teacher::on_returnToUser_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->UserPage);
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    userTableView();
}


void Teacher::on_UserInfosearch_clicked()
{
        ui->Book->setDown(Book);
        ui->User->setDown(User);

        QString userName = ui->UserName->text();
        QSqlQuery query = dbp.DBGetData();

        if(userName==""){//如果为空，那么显示全部的信息
            QString sqlSelect = QString("select userAccount, userName, amountOfBook, bookUnreturned, freezed, activated from user order by bookUnreturned DESC");
            userTableView();
        }
        else{//如果不为空，则进行搜索显示
            QString sqlSelect = QString("select userAccount, userName, password, phone, email,amountOfBook, bookUnreturned, freezed, activated from user"
                                        " where userAccount = '%1' or userName = '%2'"
                                        " order by bookUnreturned DESC").arg(userName).arg(userName);
            if(!query.exec(sqlSelect)){
                QMessageBox::warning(this,"警告","select error");
                return;
            }
                model->setQuery(sqlSelect);
        ui->UserTableView->setModel(model);
        //设置标题栏样式
        QString head[]={"用户账号","用户名称","密码","手机","邮箱","借阅书籍总数","逾期未还书籍数量","是否冻结","是否激活"};
        for(int i=0;i<9;i++){
           model->setHeaderData(i,Qt::Horizontal,head[i]);
        }
        ui->UserTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                              "color: brown;font: 10pt;"
                                                              "background-color:rgba(255,230,230,210);"
                                                              "border: 1px solid white;"
                                                             "selection-background-color:rgba(255,200,200,210);"
                                                             "}"
                                                             );
        ui->UserTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
         ui->UserTableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
        ui->UserTableView->horizontalHeader()->setSectionResizeMode(8,QHeaderView::ResizeToContents);
        ui->UserTableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
        ui->UserName->clear();
        }
}

void Teacher::on_dealbook_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
    QString userName = ui->UserName->text();
    if(userName==""){
        QMessageBox::warning(this,"提示","请在输入框输入账号！");
    }
    else{

        QSqlQuery query = dbp.DBGetData();
        QSqlQuery query1 = dbp.DBGetData();
        QString sqlselect = QString("select count(*) from borrowlist where borrowerAccount = '%1';").arg(userName);
        if(!query.exec(sqlselect)) {
           QMessageBox::warning(this,"提示","select count error");
        }
        else{
            query.next();
            if(query.value(0).toInt()==0){
//                QMessageBox::warning(this,"提示","输入账号不存在借阅信息！");
//                return;
            }

            //显示还书界面
               ui->dealBookWidget->clear();
               QString sqlselect2 = QString("select * from borrowlist where borrowerAccount = '%1';").arg(userName);

               ui->stackedWidget->setCurrentWidget(ui->dealBookPage);
               ui->Book->setDown(Book);
               ui->User->setDown(User);
               if(!query.exec(sqlselect2)) {
                  QMessageBox::warning(this,"提示","select error");
                  return;
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
                    {//读取图书数据
                         QMessageBox::warning(this,"警告","selectremain error");
                    }
                    while(query1.next())
                    {
                        QString isbn = query1.value("ISBN").toString();
                        if(isbn == ISBN)
                        {
                            remain = query1.value("remain").toInt(); //读取剩余量
                            break;
                        }
                    }
                    if(borrowerAccount == userName)
                    {
                        ReturnItem* returnlistitem = new ReturnItem;

                        returnlistitem->setValues(bookName,userName,ISBN,remain,true);

                        returnlistitem->setReturnBookinfo(bookName,borrowDate,returnDate);

                        QListWidgetItem* item = new QListWidgetItem;

                        item->setSizeHint(QSize(856,58));

                        ui->dealBookWidget->addItem(item);

                        ui->dealBookWidget->setItemWidget(item,returnlistitem);

                        connect(returnlistitem,&ReturnItem::Clear,this,&Teacher::on_dealbook_clicked);

            }
        }
    }
    }
}


void Teacher::on_return_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->UserPage);
    ui->Book->setDown(Book);
    ui->User->setDown(User);
}


void Teacher::on_Exit_clicked()
{
    ui->Book->setDown(Book);
    ui->User->setDown(User);
}

void Teacher::buttonUpdate()
{
    if(User)
    {
        if(!(ui->User->isDown()))
        {
            ui->User->setDown(User);
        }
    }
    else if(Book)
    {
        if(!(ui->Book->isDown()))
        {
            ui->Book->setDown(Book);
        }
    }
}

void Teacher::on_deleteAccount_clicked()
{
    QString userName = ui->UserName->text();
    QSqlQuery query = dbp.DBGetData();
    QString sqlSelect1 = QString("select * from user");
    bool exist = false;
    query.exec(sqlSelect1);
    QMessageBox::StandardButton answer= QMessageBox::question(this,"确认删除账号","确定要删除账号"+userName+"吗？");
    if(answer==QMessageBox::Yes){
        if(userName == "")
        {
            QMessageBox::warning(this, "警告", "账号输入不能为空!");
        }
        else
        {
            while(query.next())
            {
                QString user = query.value("userAccount").toString();
                if(user == userName)
                {
                    exist = true;
                    QString sqlDelete = QString("delete from user where userAccount = '%1'").arg(userName);
                    if(query.exec(sqlDelete))
                    {
                        QMessageBox::information(this, "提示", "删除成功!");
                    };
                    break;
                }
            }
            if(exist == false)
            {
                QMessageBox::warning(this, "警告", "该账号不存在!");
            }
        }
        ui->UserName->clear();
        userTableView();
    }
}

