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
#include <cmath>

QImage img=QImage(450,450,QImage::Format_RGB888);
int cp[100][2],pi=0;

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
void MainWindow::Mouse_Pressed()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    if(grd==0)  grd=1;
    int xyaxis=225/grd;
    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x/grd-xyaxis)+", Y : "+QString::number(ui->frame->y/grd-xyaxis));
    point(ui->frame->x,ui->frame->y,grd);
    cp[pi][0]=ui->frame->x/grd,cp[pi][1]=ui->frame->y/grd;
    pi++;
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

void draw_pxl(int x0,int y0,int grd)
{
    for(int a0=x0*grd;a0<=x0*grd+grd;a0++)
        for(int b0=y0*grd;b0<=y0*grd+grd;b0++)
            img.setPixel(a0,b0,qRgb(0,0,255));
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

void drawbez(int grd,int itr)
{
    double u=0,du=1.0/itr;
    int ip1x=cp[0][0],ip1y=cp[0][1],ip2x,ip2y;
    int cnk[pi],fac[pi];
    cnk[0]=1,fac[0]=1;
    for(int a0=1;a0<pi;a0++)fac[a0]=fac[a0-1]*a0;
    for(int a0=1;a0<pi;a0++)cnk[a0]=fac[pi-1]/(fac[a0]*fac[pi-1-a0]);
    for(int a0=0;a0<itr;a0++)
    {
        u+=du;
        ip2x=0,ip2y=0;
        for(int k=0;k<pi;k++)ip2x+=cp[k][0]*cnk[k]*pow(u,k)*pow(1-u,pi-1-k),ip2y+=cp[k][1]*cnk[k]*pow(u,k)*pow(1-u,pi-1-k);
        SimplePixelCalc(grd,ip1x,ip2x,ip1y,ip2y);
        ip1x=ip2x,ip1y=ip2y;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    s=ui->lineEdit_2->text();
    int itr=s.toLong();
    drawbez(grd,itr);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_2_clicked()
{
    pi=0;
    on_Draw_clicked();
}
