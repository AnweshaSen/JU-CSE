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
#include <cmath>
#define pi 3.14159265359

QImage img=QImage(450,450,QImage::Format_RGB888);
int xx0=0,yy0=0,xxn=0,yyn=0,flg=0,selected_x=-1,selected_y=-1;
int pts[100][2],pind=0;

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

void draw_pxl(int x0,int y0,int grd)
{
    for(int a0=x0*grd;a0<=x0*grd+grd;a0++)
        for(int b0=y0*grd;b0<=y0*grd+grd;b0++)
            img.setPixel(a0,b0,qRgb(128,220,255));
}

void SimplePixelCalc(int grd, int x0, int xn, int y0, int yn )
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
        }
    }
    else
    {
        for( int iy=((y0<yn)? y0:yn); iy<=((y0<yn)? yn:y0); iy++ )
        {
            float fx = x0 + float(xn-x0)/float(yn-y0)*(iy-y0);
            int ix = (int)(fx+0.5);
            draw_pxl(ix,iy,grd);
        }
    }
}

void MainWindow::Mouse_Pressed()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    if(grd==0)  grd=1;
    int xyaxis=225/grd;
    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x/grd-xyaxis)+", Y : "+QString::number(ui->frame->y/grd-xyaxis));
    point(ui->frame->x,ui->frame->y,grd);
    if(flg==0)
    {
        pts[pind][0]=ui->frame->x/grd,pts[pind][1]=ui->frame->y/grd;
        pind++;
        if(pind>1)
            SimplePixelCalc(grd,pts[pind-2][0],pts[pind-1][0],pts[pind-2][1],pts[pind-1][1]);
    }
    else if(flg==1)
        selected_x=ui->frame->x/grd,selected_y=ui->frame->y/grd,xx0=ui->frame->x/grd,yy0=ui->frame->y/grd,flg=2;
    else
    {
        selected_x=ui->frame->x/grd,selected_y=ui->frame->y/grd,xxn=ui->frame->x/grd,yyn=ui->frame->y/grd,flg=1;
        SimplePixelCalc(grd,xx0,xxn,yy0,yyn);
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_Draw_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    img.fill(Qt::black);
    for(int a0=0;a0<450;a0+=grd)
        for(int b0=0;b0<450;b0++)
            img.setPixel(a0,b0,qRgb(255,230,128));
    for(int a0=0;a0<450;a0++)
        for(int b0=0;b0<450;b0+=grd)
            img.setPixel(a0,b0,qRgb(255,230,128));
    int xyaxis=225/grd;
    for(int a0=xyaxis*grd;a0<=xyaxis*grd+grd;a0++)
        for(int b0=0;b0<450;b0++)
            img.setPixel(a0,b0,qRgb(255,230,128));
    for(int a0=0;a0<450;a0++)
        for(int b0=xyaxis*grd;b0<=xyaxis*grd+grd;b0++)
            img.setPixel(a0,b0,qRgb(255,230,128));
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void up(int grd)
{
    for(int a0=0;a0<pind;a0++)
            pts[a0][1]--;
    for(int a0=0;a0<pind-1;a0++)
            SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
}

void MainWindow::on_pushButton_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    on_Draw_clicked();
    up(grd);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void down(int grd)
{
    for(int a0=0;a0<pind;a0++)
            pts[a0][1]++;
    for(int a0=0;a0<pind-1;a0++)
            SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    on_Draw_clicked();
    down(grd);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void left(int grd)
{
    for(int a0=0;a0<pind;a0++)
            pts[a0][0]--;
    for(int a0=0;a0<pind-1;a0++)
            SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    on_Draw_clicked();
    left(grd);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void right(int grd)
{
    for(int a0=0;a0<pind;a0++)
            pts[a0][0]++;
    for(int a0=0;a0<pind-1;a0++)
            SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
}

void MainWindow::on_pushButton_4_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    on_Draw_clicked();
    right(grd);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void scale(int grd,double xscale,double yscale)
{
    int axis_x=225/grd,axis_y=225/grd;
    if(selected_x!=-1)
        axis_x=selected_x,axis_y=selected_y;
    for(int a0=0;a0<pind;a0++)
            pts[a0][0]=int((pts[a0][0]-axis_x)*xscale+axis_x+0.5),pts[a0][1]=int((pts[a0][1]-axis_y)*yscale+axis_y+0.5);
    for(int a0=0;a0<pind-1;a0++)
            SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
}

void MainWindow::on_pushButton_5_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    s=ui->lineEdit_2->text();
    double xscale=s.toDouble();
    s=ui->lineEdit_3->text();
    double yscale=s.toDouble();
    on_Draw_clicked();
    if(selected_x!=-1)
        draw_pxl(selected_x,selected_y,grd);
    scale(grd,xscale,yscale);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void rotate(int grd,double t)
{
    int axis_x=225/grd,axis_y=225/grd;
    if(selected_x!=-1)
        axis_x=selected_x,axis_y=selected_y;
    int newx,newy;
    double rotx,roty;
    for(int a0=0;a0<pind;a0++)
    {
        newx=pts[a0][0]-axis_x;
        newy=axis_y-pts[a0][1];
        rotx=newx*1.0*cos(t)-newy*1.0*sin(t);
        roty=newx*1.0*sin(t)+newy*1.0*cos(t);
        pts[a0][0]=round(rotx+axis_x*1.0),pts[a0][1]=round(axis_y-roty*1.0);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    s=ui->lineEdit_4->text();
    double rotate_angle=s.toDouble();
    rotate_angle=rotate_angle*pi/180.0;
    on_Draw_clicked();
    if(selected_x!=-1)
        draw_pxl(selected_x,selected_y,grd);
    rotate(grd,rotate_angle);
    for(int a0=0;a0<pind-1;a0++)
            SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_7_clicked()
{
    flg=1;
}

void MainWindow::on_pushButton_8_clicked()
{
    on_Draw_clicked();
    pind=0;
    flg=0;
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void reflect(int grd)
{
    if(yy0==yyn)
    {
        for(int a0=0;a0<pind;a0++)
            pts[a0][1]=pts[a0][1]+2*(yy0-pts[a0][1]);
        for(int a0=0;a0<pind-1;a0++)
                SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
        return;
    }
    double m=(yyn-yy0*1.0)/(xxn-xx0*1.0);
    double translate_x=xx0*1.0-yy0*1.0/m;
    for(int a0=0;a0<pind;a0++)
        pts[a0][0]=round(pts[a0][0]*1.0-translate_x);
    double t=atan(-m);
    int newx,newy;
    double rotx,roty;
    for(int a0=0;a0<pind;a0++)
    {
        newx=pts[a0][0];
        newy=pts[a0][1];
        rotx=newx*1.0*cos(t)-newy*1.0*sin(t);
        roty=newx*1.0*sin(t)+newy*1.0*cos(t);
        pts[a0][0]=round(rotx),pts[a0][1]=round(roty);
    }
    for(int a0=0;a0<pind;a0++)
        pts[a0][1]=-pts[a0][1];
    t=atan(m);
    for(int a0=0;a0<pind;a0++)
    {
        newx=pts[a0][0];
        newy=pts[a0][1];
        rotx=newx*1.0*cos(t)-newy*1.0*sin(t);
        roty=newx*1.0*sin(t)+newy*1.0*cos(t);
        pts[a0][0]=round(rotx),pts[a0][1]=round(roty);
    }
    for(int a0=0;a0<pind;a0++)
        pts[a0][0]=round(pts[a0][0]*1.0+translate_x);
    for(int a0=0;a0<pind-1;a0++)
            SimplePixelCalc(grd,pts[a0][0],pts[a0+1][0],pts[a0][1],pts[a0+1][1]);
}

void MainWindow::on_pushButton_9_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    reflect(grd);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}


