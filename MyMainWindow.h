#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H



//包含头文件
#include <QWidget>
//类前向声明
class QTableView;
class QSqlQueryModel;
class QLabel;
class QLineEdit;
class QPushButton;


#include<QHBoxLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QLabel>
#include<QSplitter>
#include<QTableView>
#include<QMessageBox>
#include<QHBoxLayout>



//**********************************************************************
// 类名:  MyMainWindow
// 目的:  自定义窗口类
//*********************************************************************
class MyMainWindow : public QWidget
{
    //宏声明
    Q_OBJECT

  //成员函数
  public:
    //构造函数
    MyMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    //析构函数
    ~MyMainWindow();

    //私有槽函数

    bool createDatabase();
private slots:
      void OnPrevButtonClick();//前一页按钮按下
      void OnNextButtonClick();//后一页按钮按下
      void OnSwitchPageButtonClick();//转到页按钮按下

    //私有成员函数
    private:
      void CreateWindow1111();//创建窗口
      void SetTableViewFrom_queryModel();//设置表格
      int  GetTotalRecordCount();//得到记录数
      int  GetPageCount();//得到页数
      void RecordQuery(int limitIndex); //记录查询
      void UpdateStatus();  //刷新状态
      void SetTotalPageLabel(); //设置总数页文本

    //成员变量
    private:
      QSqlQueryModel    *queryModel; //查询模型
      QTableView    *tableView;//数据表
      QLabel    *totalPageLabel;//总数页文本
      QLabel    *currentPageLabel;   //当前页文本
      QLineEdit *switchPageLineEdit; //转到页输入框
      QPushButton   *prevButton;      //前一页按钮
      QPushButton   *nextButton;      //下一页按钮
      QPushButton   *switchPageButton;    //转到页按钮

      int       currentPage;      //当前页
      int       totalPage;    //总页数

      int       totalRecrodCount;     //总记录数
      enum      {RecordCounts_onePage = 5};//每页显示记录数
    };


#endif // MYMAINWINDOW_H
