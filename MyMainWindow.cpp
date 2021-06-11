

#include "MyMainWindow.h"
#include "ui_MyMainWindow.h"


//包含头文件
#include <QtGui>
#include <QRegExp>
#include <QSqlQueryModel>


//**********************************************************************
// 函数: MyMainWindow
// 功能: 构造函数
//*********************************************************************
MyMainWindow::MyMainWindow(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
    //设置窗口属性
    setMinimumSize(600,300);
    setWindowTitle("分页查询");


    createDatabase();

    //创建窗口
    CreateWindow1111();
    //设置表格
    SetTableViewFrom_queryModel();
    //信号槽连接

    QObject::connect(prevButton,SIGNAL(clicked()),this,SLOT(OnPrevButtonClick()));
    QObject::connect(nextButton,SIGNAL(clicked()),this,SLOT(OnNextButtonClick()));
    QObject::connect(switchPageButton,SIGNAL(clicked()),this,SLOT(OnSwitchPageButtonClick()));
}


//**********************************************************************
// 函数: ~MyMainWindow
// 功能: 析构函数
//*********************************************************************
MyMainWindow::~MyMainWindow()
{
}

//**********************************************************************
// 函数: CreateWindow
// 功能: 创建窗口
//*********************************************************************
void MyMainWindow::CreateWindow1111()
{
    //操作布局
    QHBoxLayout *operatorLayout = new QHBoxLayout;
    prevButton = new QPushButton("前一页");
    nextButton = new QPushButton("下一页");
    switchPageButton = new QPushButton("Go");
    switchPageLineEdit = new QLineEdit;
    switchPageLineEdit->setFixedWidth(40);
    QLabel *switchPage = new QLabel("转到第");
    QLabel *page = new QLabel("页");
    operatorLayout->addWidget(prevButton);
    operatorLayout->addWidget(nextButton);
    operatorLayout->addWidget(switchPage);
    operatorLayout->addWidget(switchPageLineEdit);
    operatorLayout->addWidget(page);
    operatorLayout->addWidget(switchPageButton);
    operatorLayout->addWidget(new QSplitter());
    //状态布局
    QHBoxLayout *statusLayout = new QHBoxLayout;
    totalPageLabel = new QLabel;
    totalPageLabel->setFixedWidth(70);
    currentPageLabel = new QLabel;
    currentPageLabel->setFixedWidth(70);
    statusLayout->addWidget(totalPageLabel);
    statusLayout->addWidget(currentPageLabel);
    statusLayout->addWidget(new QSplitter());


    //设置表格属性
    tableView = new QTableView;

    //创建界面
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(operatorLayout);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(statusLayout);
}

//**********************************************************************
// 函数: SetTableView
// 功能: 设置表格
//*********************************************************************
void MyMainWindow::SetTableViewFrom_queryModel()
{
    //声明查询模型
    queryModel = new QSqlQueryModel(this);
    //设置当前页
    currentPage = 1;
    //得到总记录数
    totalRecrodCount = GetTotalRecordCount();
    //得到总页数
    totalPage = GetPageCount();
    //刷新状态
    UpdateStatus();
    //设置总页数文本
    SetTotalPageLabel();
    //记录查询
    RecordQuery(0);
    //设置模型
    tableView->setModel(queryModel);
    //设置表格表头
    queryModel->setHeaderData(0,Qt::Horizontal,"编号");
    queryModel->setHeaderData(1,Qt::Horizontal,"姓名");
    queryModel->setHeaderData(2,Qt::Horizontal,"性别");
    queryModel->setHeaderData(3,Qt::Horizontal,"年龄");
    queryModel->setHeaderData(4,Qt::Horizontal,"院系");
}

//**********************************************************************
// 函数: GetTotalRecordCount
// 功能: 得到记录数
//*********************************************************************
int MyMainWindow::GetTotalRecordCount()
{
    queryModel->setQuery("select * from student");
    return queryModel->rowCount();
}

//**********************************************************************
// 函数: GetPageCount
// 功能: 得到页数---就是当前有多少页
//*********************************************************************
int MyMainWindow::GetPageCount()
{
    return (totalRecrodCount % RecordCounts_onePage == 0) ? (totalRecrodCount / RecordCounts_onePage) : (totalRecrodCount / RecordCounts_onePage + 1);
}

//**********************************************************************
// 函数: RecordQuery
// 功能: 记录查询
//*********************************************************************
void MyMainWindow::RecordQuery(int limitIndex)
{
    QString szQuery = QString("select * from student limit %1,%2").arg(QString::number(limitIndex)).arg(QString::number(RecordCounts_onePage));
    queryModel->setQuery(szQuery);
}

//**********************************************************************
// 函数: UpdateStatus
// 功能: 刷新状态
//*********************************************************************
void MyMainWindow::UpdateStatus()
{
    //设置当前页文本
    QString szCurrentText = QString("当前第%1页").arg(QString::number(currentPage));
    currentPageLabel->setText(szCurrentText);
    //设置按钮是否可用
    if(currentPage == 1)
    {
        prevButton->setEnabled(false);
        nextButton->setEnabled(true);
    }
    else if(currentPage == totalPage)
    {
        prevButton->setEnabled(true);
        nextButton->setEnabled(false);
    }
    else
    {
        prevButton->setEnabled(true);
        nextButton->setEnabled(true);
    }
}

//**********************************************************************
// 函数: SetTotalPageLabel
// 功能: 设置总数页文本
//*********************************************************************
void MyMainWindow::SetTotalPageLabel()
{
    QString szPageCountText = QString("总共%1页").arg(QString::number(totalPage));
    totalPageLabel->setText(szPageCountText);
}

//**********************************************************************
// 函数: OnPrevButtonClick
// 功能: 前一页按钮按下
//*********************************************************************
void MyMainWindow::OnPrevButtonClick()
{
    int limitIndex = (currentPage - 2) * RecordCounts_onePage;
    RecordQuery(limitIndex);
    currentPage -= 1;
    UpdateStatus();
}

//**********************************************************************
// 函数: OnNextButtonClick
// 功能: 后一页按钮按下
//*********************************************************************
void MyMainWindow::OnNextButtonClick()
{
    int limitIndex = currentPage * RecordCounts_onePage;
    RecordQuery(limitIndex);
    currentPage += 1;
    UpdateStatus();
}

//**********************************************************************
// 函数: OnSwitchPageButtonClick
// 功能: 转到页按钮按下
//*********************************************************************
void MyMainWindow::OnSwitchPageButtonClick()
{
    //得到输入字符串
    QString szText = switchPageLineEdit->text();
    //数字正则表达式
    QRegExp regExp("-?[0-9]*");
    //判断是否为数字
    if(!regExp.exactMatch(szText))
    {
        QMessageBox::information(this, tr("提示"), tr("请输入数字"));
        return;
    }
    //是否为空
    if(szText.isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("请输入跳转页面"));
        return;
    }
    //得到页数
    int pageIndex = szText.toInt();
    //判断是否有指定页
    if(pageIndex > totalPage || pageIndex < 1)
    {
        QMessageBox::information(this, tr("提示"), tr("没有指定的页面，请重新输入"));
        return;
    }
    //得到查询起始行号
    int limitIndex = (pageIndex-1) * RecordCounts_onePage;
    //记录查询
    RecordQuery(limitIndex);
    //设置当前页
    currentPage = pageIndex;
    //刷新状态
    UpdateStatus();
}




//包含头文件
#include <QSqlDatabase>
#include <QSqlQuery>

//**********************************************************************
// 函数: createDatabase
// 功能: 创建数据库
//*********************************************************************
bool MyMainWindow::createDatabase()
{
    //添加数据
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名称
    database.setDatabaseName("database.db");
    //判断是否打开
    if(!database.open())
        //返回
        return false;
    //声明数据库查询对象
    QSqlQuery query;
    //创建表
    query.exec("create table student(id int primary key, name vchar, sex vchar, age int, deparment vchar)");
    //添加记录
    query.exec("insert into student values(1,'张三1','男',20,'计算机')");
    query.exec("insert into student values(2,'李四1','男',19,'经管')");
    query.exec("insert into student values(3,'王五1','男',22,'机械')");
    query.exec("insert into student values(4,'赵六1','男',21,'建筑工程')");
    query.exec("insert into student values(5,'小明1','男',20,'英语')");
    query.exec("insert into student values(6,'小李1','女',19,'计算机')");
    query.exec("insert into student values(7,'小张1','男',20,'机械')");
    query.exec("insert into student values(8,'小刚1','男',19,'经管')");
    query.exec("insert into student values(9,'张三2','男',21,'计算机')");
    query.exec("insert into student values(10,'张三3','女',20,'建筑工程')");
    query.exec("insert into student values(11,'王五2','男',19,'经管')");
    query.exec("insert into student values(12,'张三4','男',20,'计算机')");
    query.exec("insert into student values(13,'小李2','男',20,'机械')");
    query.exec("insert into student values(14,'李四2','女',19,'经管')");
    query.exec("insert into student values(15,'赵六3','男',21,'英语')");
    query.exec("insert into student values(16,'李四2','男',19,'建筑工程')");
    query.exec("insert into student values(17,'小张2','女',22,'经管')");
    query.exec("insert into student values(18,'李四3','男',21,'英语')");
    query.exec("insert into student values(19,'小李3','女',19,'建筑工程')");
    query.exec("insert into student values(20,'王五3','女',20,'机械')");
    query.exec("insert into student values(21,'张三4','男',22,'计算机')");
    query.exec("insert into student values(22,'小李2','男',20,'建筑工程')");
    query.exec("insert into student values(23,'张三5','男',19,'经管')");
    query.exec("insert into student values(24,'小张3','女',20,'计算机')");
    query.exec("insert into student values(25,'李四4','男',22,'英语')");
    query.exec("insert into student values(26,'赵六2','男',20,'机械')");
    query.exec("insert into student values(27,'小李3','女',19,'英语')");
    query.exec("insert into student values(28,'王五4','男',21,'经管')");
    //返回
    return true;
}


