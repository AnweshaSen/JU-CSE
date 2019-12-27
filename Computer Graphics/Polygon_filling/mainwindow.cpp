#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include <QLineEdit>
#include <QElapsedTimer>
#include <QTimer>
#include <cstdlib>
#include <ctime>
using namespace std;
QImage img=QImage(450,450,QImage::Format_RGB888);
int xx0=0,yy0=0,xxn=0,yyn=0,flg=0;
int flag=0,pts[100][2],pi=0,crss_pts[100][2],crss_pti=0,xinside,yinside;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::point(int x,int y,int grd)
{
    int xx=x/grd,yy=y/grd;
    for(int a=xx*grd;a<=xx*grd+grd;a++){
        for(int b=yy*grd;b<=yy*grd+grd;b++){
                img.setPixel(a,b,qRgb(0,0,255));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::showMousePosition(QPoint &pos)
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    if(grd==0)  grd=1;
    int xyaxis=225/grd;
    ui->mouse_movement->setText(" X : "+QString::number(pos.x()/grd-xyaxis)+", Y : "+QString::number(pos.y()/grd-xyaxis));
}

void change_color(int x0,int y0,int grd)
{
    for(int a0=x0*grd;a0<=x0*grd+grd;a0++)
        for(int b0=y0*grd;b0<=y0*grd+grd;b0++)
            img.setPixel(a0,b0,qRgb(255,0,0));
}

void MainWindow::Mouse_Pressed()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    if(grd==0)  grd=1;
    int xyaxis=225/grd;
    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x/grd-xyaxis)+", Y : "+QString::number(ui->frame->y/grd-xyaxis));
    point(ui->frame->x,ui->frame->y,grd);
    pts[pi][0]=ui->frame->x/grd,pts[pi][1]=ui->frame->y/grd;
    pi++;
    if(flg==0)  xx0=ui->frame->x/grd,yy0=ui->frame->y/grd,flg=1;
    else    xxn=ui->frame->x/grd,yyn=ui->frame->y/grd,flg=0;
    if(flag==0) flag=1;
    else if(flag==2)
    {
        xinside=ui->frame->x/grd,yinside=ui->frame->y/grd;
        change_color(xinside,yinside,grd);
        flag=0;
    }
    else
        SimplePixelCalc(grd,xx0,xxn,yy0,yyn);
}
int chk_point(int x,int y)
{
    for(int a0=0;a0<pi;a0++)
    {
        if(pts[a0][0]==x && pts[a0][1]==y)
        {
            if((pts[(a0+1)%pi][1]-y)*(pts[(a0+pi-1)%pi][1]-y)<0)
                return 1;
            else
                return 0;
        }
    }
    for(int a0=0;a0<crss_pti;a0++)
        if(x==crss_pts[a0][0] && y==crss_pts[a0][1])
            return 1;
    return 0;
}

void MainWindow::on_Draw_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    img.fill(Qt::black);
    for(int a0=0;a0<450;a0+=grd)
        for(int b0=0;b0<450;b0++)
            img.setPixel(a0,b0,qRgb(255,255,255));
    for(int a0=0;a0<450;a0++)
        for(int b0=0;b0<450;b0+=grd)
            img.setPixel(a0,b0,qRgb(255,255,255));
    int xyaxis=225/grd;
    for(int a0=xyaxis*grd;a0<=xyaxis*grd+grd;a0++)
        for(int b0=0;b0<450;b0++)
            img.setPixel(a0,b0,qRgb(255,0,0));
    for(int a0=0;a0<450;a0++)
        for(int b0=xyaxis*grd;b0<=xyaxis*grd+grd;b0++)
            img.setPixel(a0,b0,qRgb(255,0,0));
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void draw_pxl(int x0,int y0,int grd, int c = 0)
{
    for(int a0=x0*grd;a0<=x0*grd+grd;a0++)
        for(int b0=y0*grd;b0<=y0*grd+grd;b0++)
            if(a0<450 && b0<450)
                img.setPixel(a0,b0,qRgb(0,c,255));
}
void floodfill(int grd)
{
    for(int y=0;y<450/grd-1;y++)
    {
        bool pen=false;
        crss_pti=0;
        for(int a0=0;a0<pi;a0++)
        {
            if((pts[(a0+1)%pi][1]-y)*(pts[a0][1]-y)<0)
            {
                crss_pts[crss_pti][0]=int((y*1.0*(pts[(a0+1)%pi][0]-pts[a0][0])+(pts[a0][0]*1.0*pts[(a0+1)%pi][1]-pts[a0][1]*pts[(a0+1)%pi][0]))/(pts[(a0+1)%pi][1]-pts[a0][1])+0.5);
                crss_pts[crss_pti][1]=y;
                crss_pti++;
            }
        }
        for(int x=0;x<450/grd-1;x++)
        {
            int point_type=chk_point(x,y);
            if(point_type==1)
                    pen=!pen;
            if(pen)
                draw_pxl(x,y,grd);
        }
    }
}
void MainWindow::SimplePixelCalc(int grd, int x0, int xn, int y0, int yn )
{
    if(x0==xn && y0==yn)
        {
            draw_pxl(x0,y0,grd);
            return;
        }
        if(abs(xn-x0)>=abs(yn-y0))
        {
            for( int ix=((x0<xn)? x0:xn); ix<=((x0<xn)? xn:x0); ix++ )
            {
                float fy = y0 + float(yn-y0)/float(xn-x0)*(ix-x0);
                int iy = (int)(fy+0.5);
                draw_pxl(ix,iy,grd);
                ui->frame->setPixmap(QPixmap::fromImage(img));
            }
        }
        else
        {
            for( int iy=((y0<yn)? y0:yn); iy<=((y0<yn)? yn:y0); iy++ )
            {
                float fx = x0 + float(xn-x0)/float(yn-y0)*(iy-y0);
                int ix = (int)(fx+0.5);
                draw_pxl(ix,iy,grd);
                ui->frame->setPixmap(QPixmap::fromImage(img));
            }
        }
}
void flood_fill(int x,int y,int grd)
{
    if(x<0 || x>450/grd-1 || y<0 || y>450/grd-1 || qBlue(img.pixel(x*grd+2,y*grd+2))==255)
        return;
    if((qBlue(img.pixel(x*grd+1,y*grd+1))==255) && (qGreen(img.pixel(x*grd+1,y*grd+1))==255) && (qRed(img.pixel(x*grd+1,y*grd+1))==255) )
    {
        draw_pxl(x,y,grd,122);
        flood_fill(x+grd,y,grd);flood_fill(x,y+grd,grd);flood_fill(x-grd,y,grd);flood_fill(x,y-grd,grd);
    }
}
void boundary_fill(int x,int y,int grd,int c = 255)
{
    if(x<0 || x>450/grd-1 || y<0 || y>450/grd-1 || qBlue(img.pixel(x*grd+1,y*grd+1))==255)
        return;

    draw_pxl(x,y,grd,c);
    boundary_fill(x+1,y,grd);boundary_fill(x,y+1,grd);boundary_fill(x-1,y,grd);boundary_fill(x,y-1,grd);
}

void MainWindow::on_pushButton_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    QElapsedTimer timer1;
    timer1.restart();
    boundary_fill(xinside,yinside,grd);
    int msec = timer1.nsecsElapsed();
    ui->lineEdit_2->setText(QString::number(msec));
    flag=0,pi=0;
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_3_clicked()
{
    on_Draw_clicked();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    SimplePixelCalc(grd,pts[pi-1][0],pts[0][0],pts[pi-1][1],pts[0][1]);
    flag=2;
}


void scanline_fill(int grd)
{
    for(int y=0;y<450/grd-1;y++)
    {
        bool pen=false;
        crss_pti=0;
        for(int a0=0;a0<pi;a0++)
        {
            if((pts[(a0+1)%pi][1]-y)*(pts[a0][1]-y)<0)
            {
                crss_pts[crss_pti][0]=int((y*1.0*(pts[(a0+1)%pi][0]-pts[a0][0])+(pts[a0][0]*1.0*pts[(a0+1)%pi][1]-pts[a0][1]*pts[(a0+1)%pi][0]))/(pts[(a0+1)%pi][1]-pts[a0][1])+0.5);
                crss_pts[crss_pti][1]=y;
                crss_pti++;
            }
        }
        for(int x=0;x<450/grd-1;x++)
        {
            int point_type=chk_point(x,y);
            if(point_type==1)
                    pen=!pen;
            if(pen)
                draw_pxl(x,y,grd);
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    QElapsedTimer timer1;
    timer1.restart();
    scanline_fill(grd);
    int msec = timer1.nsecsElapsed();
    ui->lineEdit_2->setText(QString::number(msec));
    flag=0,pi=0;
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_5_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    QElapsedTimer timer1;
    timer1.restart();
    boundary_fill(xinside,yinside,grd,122);
    int msec = timer1.nsecsElapsed();
    ui->lineEdit_2->setText(QString::number(msec));
    flag=0,pi=0;
    ui->frame->setPixmap(QPixmap::fromImage(img));
}
