#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QDebug>

const int MOUSE_MARGIN = 4;                             //缩放时鼠标变形时边界宽度
const int SHADOW_WIDTH = 3;                             //阴影宽度
const int SHADOW_BLUR_RADIUS = 8;                       //阴影圆角半径



MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);     //无边框、任务栏单击可隐藏和显示窗口
    ui->menubar->setVisible(false);
    ui->statusbar->setVisible(false);
//    ui->pushButtonMinimum->setVisible(true);
//    ui->pushButtonMaximum->setVisible(true);
    ui->pushButtonRestore->setVisible(false);
//    ui->pushButtonClose->setVisible(true);
    ui->title_bar->setMouseTracking(true);
    
    setAttribute(Qt::WA_TranslucentBackground, true);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);    //实例阴影shadow
    shadow->setOffset(0, 0);                                                    //设置阴影距离
    shadow->setColor(QColor("#444444"));                                        //设置阴影颜色
    shadow->setBlurRadius(SHADOW_BLUR_RADIUS);                                  //设置阴影圆角
    setGraphicsEffect(shadow);                                                  //给QWidget设置阴影

    //setWindowFlags(Qt::FramelessWindowHint);                                  //设置窗体无边框
    this->menuBar()->setVisible(false);                                         //设置菜单栏不可见
    this->statusBar()->setVisible(false);                                       //设置状态栏不可见
    
    ui->pushButton_stop_daemon->setEnabled(false);
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

MyMainWindow::MouseArea MyMainWindow::calc_mouse_area(QPoint p)
{
    if(p.x()<MOUSE_MARGIN && p.y() < MOUSE_MARGIN) {                                                                    //左上
        return MouseArea::LeftUp;
    }
    else if(p.x()>MOUSE_MARGIN && p.y()< MOUSE_MARGIN && p.x()<this->width()-MOUSE_MARGIN) {                            //上
        return MouseArea::Up;
    }
    else if(p.x() > this->width() - MOUSE_MARGIN && p.y() < MOUSE_MARGIN) {                                             //右上
        return MouseArea::RightUp;
    }
    else if(p.x() > this->width() - MOUSE_MARGIN && p.y() > MOUSE_MARGIN && p.y()<this->height() - MOUSE_MARGIN) {      //右
        return MouseArea::Right;
    }
    else if(p.x() > this->width() - MOUSE_MARGIN && p.y() > this->height() - MOUSE_MARGIN) {                            //右下
        return MouseArea::RightDown;
    }
    else if(p.x() > MOUSE_MARGIN && p.x() < this->width() - MOUSE_MARGIN && p.y() > this->height() - MOUSE_MARGIN) {    //下
        return MouseArea::Down;
    }
    else if(p.x() < MOUSE_MARGIN && p.y() > this->height() - MOUSE_MARGIN) {                                            //左下
        return MouseArea::LeftDown;
    }
    else if(p.x() < MOUSE_MARGIN && p.y() < this->height() - MOUSE_MARGIN && p.y() > MOUSE_MARGIN) {                    //左
        return MouseArea::Left;
    }
    else {                                                                                                              //中间
        return MouseArea::Middle;
    }
}

void MyMainWindow::set_cursor_type(MyMainWindow::MouseArea type)
{
    switch(type) {
        case MouseArea::LeftUp:
        case MouseArea::RightDown:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case MouseArea::Up:
        case MouseArea::Down:
            setCursor(Qt::SizeVerCursor);
            break;
        case MouseArea::RightUp:
        case MouseArea::LeftDown:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case MouseArea::Right:
        case MouseArea::Left:
            setCursor(Qt::SizeHorCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
    }
}

void MyMainWindow::change_window_size(MyMainWindow::MouseArea type, QRect &wid, QPoint tmp_point, int minWidth, int minHeight)
{
    switch(type) {
        case MouseArea::LeftUp:                                                         //左上角
        {
            QPoint pos = wid.topLeft();
            if(wid.width() > minWidth || tmp_point.x() < 0) {
                pos.rx() = pos.rx() + tmp_point.x();
            }
            if(wid.height() > minHeight || tmp_point.y() < 0) {
                pos.ry() = pos.ry() + tmp_point.y();
            }
            wid.setTopLeft(pos);
            break;
        }
        case MouseArea::RightUp:                                                        //右上角
        {
            QPoint pos = wid.topRight();
            if(wid.width() > minWidth || tmp_point.x() > 0) {
                pos.rx() = pos.rx() + tmp_point.x();
            }
            if(wid.height() > minHeight || tmp_point.y() < 0) {
                pos.ry() = pos.ry() + tmp_point.y();
            }
            wid.setTopRight(pos);
            break;
        }
        case MouseArea::LeftDown:                                                       //左下角
        {
            QPoint pos = wid.bottomLeft();
            if(wid.width() > minWidth || tmp_point.x() < 0) {
                pos.rx() = pos.rx() + tmp_point.x();
            }
            if(wid.height() > minHeight || tmp_point.y() > 0) {
                pos.ry() = pos.ry() + tmp_point.y();
            }
            wid.setBottomLeft(pos);
            break;
        }
        case MouseArea::RightDown:                                                      //右下角
        {
            QPoint pos = wid.bottomRight();
            if(wid.width() > minWidth || tmp_point.x() > 0) {
                pos.rx() = pos.rx() + tmp_point.x();
            }
            if(wid.height() > minHeight || tmp_point.y() > 0) {
                pos.ry() = pos.ry() + tmp_point.y();
            }
            wid.setBottomRight(pos);
            break;
        }
        case MouseArea::Up:                                                             //上
        {
            int topY = wid.top();
            if(wid.height() > minHeight || tmp_point.y() < 0) {
                topY = topY + tmp_point.y();
            }
            wid.setTop(topY);
            break;
        }
        case MouseArea::Left:                                                           //左
        {
            int leftX = wid.left();
            if(wid.width() > minWidth || tmp_point.x() < 0) {
                leftX = leftX + tmp_point.x();
            }
            wid.setLeft(leftX);
            break;
        }
        case MouseArea::Right:                                                          //右
        {
            int rightX = wid.right();
            if(wid.width() > minWidth || tmp_point.x() > 0) {
                rightX = rightX + tmp_point.x();
            }
            wid.setRight(rightX);
            break;
        }
        case MouseArea::Down:                                                           //下
        {
            int botY = wid.bottom();
            if(wid.height() > minHeight || tmp_point.y() > 0) {
                botY = botY + tmp_point.y();
            }
            wid.setBottom(botY);
            break;
        }
    default:
        break;
    }
}

void MyMainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        last = event->globalPos();
        event->ignore();
    }
}

void MyMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //最大化状态，鼠标是否在标题栏,拖拽还原
    if(this->isMaximized()) {
        if(ui->title_bar->geometry().contains(this->mapFromGlobal(QCursor::pos())) && event->buttons() == Qt::LeftButton) {
            QPoint ptemp = event->globalPos();
            if(ptemp != last) {
                showNormal();
            }
        }
    }
    MouseArea cur_mouse_area = calc_mouse_area(event->pos());
    if(!event->buttons()) {
        set_cursor_type(cur_mouse_area);
        mp_curPos = cur_mouse_area;
    }
    
    if(event->buttons() == Qt::LeftButton) {                              //坐标左键按下并移动
        QPoint ptemp = event->globalPos();
        ptemp = ptemp - last;                                             //鼠标移动的偏移量
        if(mp_curPos == MouseArea::Middle && ui->title_bar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
            move(ptemp + pos());
        }
        else {
            QRect wid = geometry();
            int minWidth = this->minimumWidth();
            int minHeight = this->minimumHeight();
            change_window_size(mp_curPos, wid, ptemp, minWidth, minHeight);
            setGeometry(wid);                                           //更新大小
        }
        last = event->globalPos();
    }
    event->ignore();
}

void MyMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        move(x() + dx, y() + dy);
    }
}

void MyMainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    QRect rt = this->geometry();
    //int n = event->screenPos().y();
    if (event->screenPos().y() > rt.top() + 100)
        return;

    if (this->isMaximized()) {
        this->showNormal();
    }
    else {
        this->showMaximized();
    }
}

void MyMainWindow::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.fillRect(this->rect().adjusted(SHADOW_WIDTH, SHADOW_WIDTH, -SHADOW_WIDTH, -SHADOW_WIDTH), QColor(255, 255, 255));
}

void MyMainWindow::on_pushButton_option_clicked()
{
    QMenu menu(NULL);
    
    QAction *shearM = menu.addAction(tr("Cut"), this, SLOT(cut()));
    shearM->setEnabled(true);
    
    QAction *copyM = menu.addAction(tr("Copy"), this, SLOT(copy()));
    copyM->setEnabled(false);
    
    menu.addAction(QIcon("qrc:/img/dui_hao.png"), tr("Paste"), this, SLOT(paste()));
    
    QAction *deleteM = menu.addAction(tr("Delete"), this, SLOT(clear()));
    deleteM->setEnabled(false);
    
    menu.addSeparator();
    
    QAction *selectM = menu.addAction(tr("SelectAll"), this, SLOT(selectAll()));
    selectM->setEnabled(true);
    
    int top = 0;
    int botton = 0;
    int left = 0;
    int right = 0;
    ui->gridLayout->getContentsMargins(&left, &top, &right, &botton);
    //menu.exec(ui->pushButton_option->mapToParent(ui->pushButton_option->geometry().bottomLeft() + QPoint(left, top)));
    menu.exec(mapToParent(ui->pushButton_option->geometry().bottomLeft() + QPoint(left + SHADOW_BLUR_RADIUS, top + SHADOW_BLUR_RADIUS)));
}

void MyMainWindow::on_pushButtonMinimum_clicked()
{
    showMinimized();
}

void MyMainWindow::on_pushButtonMaximum_clicked()
{
    ui->pushButtonMaximum->setVisible(false);
    ui->pushButtonRestore->setVisible(true);
    showMaximized();
}

void MyMainWindow::on_pushButtonRestore_clicked()
{
    ui->pushButtonMaximum->setVisible(true);
    ui->pushButtonRestore->setVisible(false);
    showNormal();
}

void MyMainWindow::on_pushButtonClose_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(800);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
}

void MyMainWindow::on_pushButton_start_daemon_clicked()
{
    ui->textBrowser_log->append("start");
    
    myDaemonThread = new MyDaemonThread(this, ui->lineEdit_exe_location->text().toStdString());
    if (myDaemonThread) {
        connect(myDaemonThread, SIGNAL(signalLog(const QString&)), this, SLOT(slotLog(const QString&)));
        myDaemonThread->start();
        ui->pushButton_stop_daemon->setEnabled(true);
        ui->pushButton_start_daemon->setEnabled(false);
    }
}

void MyMainWindow::on_pushButton_stop_daemon_clicked()
{
    ui->textBrowser_log->append("stop");
    
    if (myDaemonThread) {
        myDaemonThread->terminate();
        delete myDaemonThread;
        myDaemonThread = NULL;
        ui->pushButton_stop_daemon->setEnabled(false);
        ui->pushButton_start_daemon->setEnabled(true);
    }
}

void MyMainWindow::slotLog(const QString& msg)
{
    ui->textBrowser_log->append(msg);
}
