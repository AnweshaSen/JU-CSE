//#include <bits/stdc++.h>
#include<QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include <QMouseEvent>
#include <QQueue>
#include <QElapsedTimer>
#include <QTimer>
#include <cstdlib>
#include <ctime>

#define PI 3.1415

int max(int a,int b){
    return a>b?a:b;
}

void swap(int *a,int *b){
    int t = *a;
    *a = *b;
    *b = t;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->x_axis->hide();
    ui->y_axis->hide();
    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw_grid(QPixmap *pix,QPainter *paint)
{
    gridSize = max(5,ui->horizontalSlider->value());
    //qDebug()<<ui->frame->width();
    //qDebug()<<ui->frame->height();
    int orix = ((ui->frame->width()/2)/gridSize)*gridSize;
    int oriy = ((ui->frame->height()/2)/gridSize)*gridSize;
    ui->x_axis->move(0,oriy + gridSize/2);
    ui->y_axis->move(orix + gridSize/2,0);
    paint->begin(pix);
    paint->setPen(Qt::blue);
    for(int i=0;i<ui->frame->width();i++){
        if(i%gridSize == 0)paint->drawLine(QPoint(i,0),QPoint(i,ui->frame->height()));
    }
    for(int i=0;i<ui->frame->height();i++){
        if(i%gridSize == 0)paint->drawLine(QPoint(0,i),QPoint(ui->frame->width(),i));
    }
    for(int i=0;i<ui->frame->width();i++)paint->fillRect(orix,i,gridSize,gridSize,Qt::yellow);
    for(int i=0;i<ui->frame->height();i++)paint->fillRect(i,oriy,gridSize,gridSize,Qt::yellow);
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::point(int x,int y,int r)
{
    int a,b;
    QImage img=QImage(450,450,QImage::Format_RGB888);

    for(a=-r;a<=r;a++){
        for(b=-r;b<=r;b++){
            img.setPixel(x+a,y+b,qRgb(0,255,255    ));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::showMousePosition(QPoint &pos)
{
    ui->mouse_movement->setText(" X : "+QString::number(pos.x()/gridSize - ui->frame->width()/(2*gridSize))+", Y : "+QString::number(ui->frame->height()/(2*gridSize) - pos.y()/gridSize));
}
void MainWindow::Mouse_Pressed()
{
    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x/gridSize - ui->frame->width()/(2*gridSize))+", Y : "+QString::number(ui->frame->height()/(2*gridSize) - ui->frame->y/gridSize));
    QPixmap *pix=new QPixmap(450,450);
    QPainter *paint=new QPainter();
    draw_grid(pix,paint);
    paint->begin(pix);
    paint->setPen(Qt::white);
    int a0 = (ui->frame->x/gridSize)*gridSize;
    int b0 = (ui->frame->y/gridSize)*gridSize;
    paint->fillRect(a0,b0,gridSize,gridSize,Qt::white);
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::on_show_axes_clicked()
{
    if(ui->show_axes->isChecked())
    {
        ui->x_axis->show();
        ui->y_axis->show();
    }
    else{
        ui->x_axis->hide();
        ui->y_axis->hide();
    }
}
void MainWindow::on_set_point1_clicked()
{
    if(ui->linear_line->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
    if(ui->DDALine->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
    if(ui->bresenhem_line->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
}

void MainWindow::on_set_point2_clicked()
{
    if(ui->linear_line->isChecked()){
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
    }
    if(ui->DDALine->isChecked()){
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
    }
    if(ui->bresenhem_line->isChecked()){
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
    }
}

void MainWindow::setPix(int x,int y,QPixmap *pix,QPainter *paint){
    Q_UNUSED(pix);
    int a0 = (x/gridSize)*gridSize;
    int b0 = (y/gridSize)*gridSize;
    paint->fillRect(a0,b0,gridSize,gridSize,Qt::white);
}

void MainWindow::linearEqnLine(QPixmap *pix, QPainter *paint) {
    float m = (float)(p2.y() - p1.y()) / (float)(p2.x() - p1.x());
    int x1, y1, x0, y0;
    x0 = p1.x();
    y0 = p1.y();
    for (x1 = x0; x1 <= p2.x(); ++x1) {
        y1 = (x1 - x0) * m + y0;
        setPix(x1, y1, pix, paint);
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::dda_line(QPixmap *pix,QPainter *paint){
    int dx = p2.x()-p1.x();
    int dy = p2.y()-p1.y(),steps;
    float xinc,yinc,x = float(p1.x()),y = float(p1.y());
    if(abs(dx)>abs(dy))steps = abs(dx);
    else steps = abs(dy);
    xinc = float(dx)/float(steps);
    yinc = float(dy)/float(steps);
    setPix(x,y,pix,paint);
    for(int k=0;k<steps/gridSize;k++){
        x += xinc*gridSize;
        y += yinc*gridSize;

        setPix(x,y,pix,paint);
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::bresen_line(QPixmap *pix, QPainter *paint)
{
    int x = p1.x(),y = p1.y();
    int dx = abs(p2.x()-p1.x());
    int dy = abs(p2.y()-p1.y());
    int xinc = gridSize*(p2.x()-p1.x())/dx;
    int yinc = gridSize*(p2.y()-p1.y())/dy;
    bool swap_flag = false;
    if(dy > dx){
        swap(&dy,&dx);
        swap_flag = true;
    }
    int p = 2*dy-dx;
    setPix(x,y,pix,paint);
    for(int i=0;i<dx/gridSize;i++){
        if(p >= 0){
            x += xinc;
            y += yinc;
            p += 2*(dy-dx);
        }
        else if(swap_flag){
            y += yinc;
            p += 2*dy;
        }
        else{
            x += xinc;
            p += 2*dy;
        }
        setPix(x,y,pix,paint);
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::param_circle(int r,QPixmap *pix, QPainter *paint){
    int x,y;
    r *= gridSize;
    for(int i=0;i<360;i++){
        x = r*sin(float(i*PI)/180);
        y = r*cos(float(i*PI)/180);
        setPix(x+p1.x(),y+p1.y(),pix,paint);
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::midp_circle(int r,QPixmap *pix, QPainter *paint){
    int x=0,y=r*gridSize,p=1-r,xprev,yprev;
    while(x<=y){
        setPix(x+p1.x(),y+p1.y(),pix,paint);
        setPix(-x+p1.x(),y+p1.y(),pix,paint);
        setPix(x+p1.x(),-y+p1.y(),pix,paint);
        setPix(-x+p1.x(),-y+p1.y(),pix,paint);

        setPix(y+p1.x(),x+p1.y(),pix,paint);
        setPix(y+p1.x(),-x+p1.y(),pix,paint);
        setPix(-y+p1.x(),x+p1.y(),pix,paint);
        setPix(-y+p1.x(),-x+p1.y(),pix,paint);

        xprev = x;
        yprev = y;
        x += 1;
        if(p >= 0) y-= 1;
        p += 2*(xprev+1)+(y*y - yprev*yprev)-(y-yprev)+1;
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::bresen_circle(int r,QPixmap *pix, QPainter *paint){
    int x=0,y=r*gridSize,p=3-2*r;
    while(x<=y){
        setPix(x+p1.x(),y+p1.y(),pix,paint);
        setPix(-x+p1.x(),y+p1.y(),pix,paint);
        setPix(x+p1.x(),-y+p1.y(),pix,paint);
        setPix(-x+p1.x(),-y+p1.y(),pix,paint);

        setPix(y+p1.x(),x+p1.y(),pix,paint);
        setPix(y+p1.x(),-x+p1.y(),pix,paint);
        setPix(-y+p1.x(),x+p1.y(),pix,paint);
        setPix(-y+p1.x(),-x+p1.y(),pix,paint);

        x += gridSize;
        if(p >= 0){
            y -= gridSize;
            p += 4*(x-y)+10;
        }
        else p += 4*x+6;
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::param_ellipse(int rx, int ry, QPixmap *pix, QPainter *paint) {
    int theta = 0;
    rx *= gridSize;
    ry *= gridSize;
    for (; theta < 360; ++theta) {
        int x1 = rx * cos(float(theta * PI) / 180);
        int y1 = ry * sin(float(theta * PI) / 180);
        x1 += p1.x();
        y1 += p1.y();
        setPix(x1, y1, pix, paint);
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::midp_ellipse(int rx, int ry, QPixmap *pix, QPainter *paint) {
    rx *= gridSize;
    ry *= gridSize;
    int xc = p1.x();
    int yc = p1.y();
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    // Initial decision parameter of region 1
    d1 = (ry * ry) - (rx * rx * ry) +
            (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // For region 1
    while (dx < dy)
    {
        setPix(x+xc, y+yc, pix, paint);
        setPix(-x+xc, y+yc, pix, paint);
        setPix(x+xc, -y+yc, pix, paint);
        setPix(-x+xc, -y+yc, pix, paint);

        // Checking and updating value of
        // decision parameter based on algorithm
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    // Decision parameter of region 2
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
            ((rx * rx) * ((y - 1) * (y - 1))) -
            (rx * rx * ry * ry);

    // Plotting points of region 2
    while (y >= 0)
    {
        setPix(x+xc, y+yc, pix, paint);
        setPix(-x+xc, y+yc, pix, paint);
        setPix(x+xc, -y+yc, pix, paint);
        setPix(-x+xc, -y+yc, pix, paint);

        // Checking and updating parameter
        // value based on algorithm
        if (d2 > 0)
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
    ui->frame->setPixmap(*pix);
    paint->end();
}

void MainWindow::on_Draw_clicked()
{
    QElapsedTimer timer;

    int r0=ui->circle_radius->value();
    QPixmap *pix=new QPixmap(450,450);
    QPainter *paint=new QPainter();
    draw_grid(pix,paint);
    if(ui->bresenhem_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        paint->begin(pix);
        timer.restart();
        bresen_circle(r0,pix,paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
        //time(&end);
        //ui->execution_time->setText(QString::number(time.elapsed()+"ms",2000));
    }
    if(ui->param_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        paint->begin(pix);
        paint->setPen(Qt::white);
        timer.restart();
        param_circle(r0,pix,paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
    }
    if(ui->mid_point_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        paint->begin(pix);
        paint->setPen(Qt::white);
        timer.restart();
        midp_circle(r0,pix,paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
    }
    if(ui->bresenhem_line->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        paint->begin(pix);
        paint->setPen(Qt::white);
        timer.restart();
        bresen_line(pix,paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
    }

    if (ui->linear_line->isChecked()) {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        paint->begin(pix);
        paint->setPen(Qt::white);
        timer.restart();
        linearEqnLine(pix,paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
    }

    if (ui->DDALine->isChecked()) {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        paint->begin(pix);
        paint->setPen(Qt::white);
        timer.restart();
        dda_line(pix,paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
    }

    if (ui->Param_Ellipse->isChecked()) {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        int rx = ui->ellipse_rx->value();
        int ry = ui->ellipse_ry->value();
        paint->begin(pix);
        paint->setPen(Qt::white);
        timer.restart();
        param_ellipse(rx, ry, pix, paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
    }

    if (ui->mid_point->isChecked()) {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        int rx = ui->ellipse_rx->value();
        int ry = ui->ellipse_ry->value();
        paint->begin(pix);
        paint->setPen(Qt::white);
        timer.restart();
        midp_ellipse(rx, ry, pix, paint);
        int msec = timer.nsecsElapsed();
        ui->execution_time->setText(QString::number(msec));
    }

}


