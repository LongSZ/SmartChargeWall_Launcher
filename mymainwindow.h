#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

//Qt
#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QClipboard>
#include <QFileDialog>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include <QThread>

//C++
#include <iostream>
#include <thread>
#include <map>
#include <windows.h>
#include <tchar.h>
#include <libloaderapi.h>
#include <string>
using namespace std;

#include "mydaemonthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyMainWindow; }
QT_END_NAMESPACE

class MyMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MyMainWindow(QWidget *parent = nullptr);
    ~MyMainWindow();
    
    //标识鼠标所在的9个区域
    enum MouseArea{
        LeftUp = 0,                                     //左上
        Up,                                             //上
        RightUp,                                        //右上
        Right,                                          //右
        RightDown,                                      //右下
        Down,                                           //下
        LeftDown,                                       //左下
        Left,                                           //左
        Middle                                          //中
    };
    
    MouseArea calc_mouse_area(QPoint p);
    void set_cursor_type(MouseArea type);
    void change_window_size(MouseArea type, QRect &wid, QPoint tmp_point, int minWidth, int minHeight);
    
protected:
    //在头文件中重写鼠标事件, 由于无边框后无法进行标题栏的相关操作
    void mousePressEvent(QMouseEvent *event);           //鼠标按下
    void mouseMoveEvent(QMouseEvent *event);            //鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);         //鼠标释放
    void mouseDoubleClickEvent(QMouseEvent *event);     //双击事件
    void paintEvent(QPaintEvent *event);
    
public slots:
    void slotLog(const QString& msg);

private slots:
    void on_pushButton_option_clicked();
    void on_pushButtonMinimum_clicked();
    void on_pushButtonMaximum_clicked();
    void on_pushButtonRestore_clicked();
    void on_pushButtonClose_clicked();

    void on_pushButton_start_daemon_clicked();
    
    void on_pushButton_stop_daemon_clicked();
    
private:
    Ui::MyMainWindow *ui;
    
    MouseArea mp_curPos;                                //记录鼠标当前所在区域的变量
    QPoint last;                                        //窗口拖动用变量, 记录上次位置
    
    MyDaemonThread *myDaemonThread;
};
#endif // MYMAINWINDOW_H
