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
vector<QPoint> points;
vector<QPoint> poly;
vector<QPoint> crss_pts;
int x_min,y_min,x_max,y_max;

void draw_pxl(int x0,int y0,int grd);
void draw_blu_pxl(int x0,int y0,int grd);

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
                img.setPixel(a,b,qRgb(255,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

int chk_point(int x,int y)
{
    for(int a0=0;a0<poly.size();a0++)
    {
        if(poly[a0].x()==x && poly[a0].y()==y)
        {
            if((poly[(a0+1)%poly.size()].y()-y)*(poly[(a0+poly.size()-1)%poly.size()].y()-y)<0)
                return 1;
            else
                return 0;
        }
    }
    for(int a0=0;a0<crss_pts.size();a0++)
        if(x==crss_pts[a0].x() && y==crss_pts[a0].y())
            return 1;
    return 0;
}


void draw_pxl_clr(int x0,int y0,int grd,int r,int g,int b)
{
    for(int a0=x0*grd;a0<=x0*grd+grd;a0++)
        for(int b0=y0*grd;b0<=y0*grd+grd;b0++)
            if(a0<450 && b0<450)
                img.setPixel(a0,b0,qRgb(r,g,b));
}

void scanline_fill(int grd,int r,int g,int b)
{
    for(int y=0;y<450/grd-1;y++)
    {
        bool pen=false;
        int crss_pti=0;
        for(int a0=0;a0<poly.size();a0++)
        {
            if((poly[(a0+1)%poly.size()].y()-y)*(poly[a0].y()-y)<0)
            {
                QPoint p;
                p.setX(int((y*1.0*(poly[(a0+1)%poly.size()].x()-poly[a0].x())+(poly[a0].x()*1.0*poly[(a0+1)%poly.size()].y()-poly[a0].y()*poly[(a0+1)%poly.size()].x()))/(poly[(a0+1)%poly.size()].y()-poly[a0].y())+0.5));
                p.setY(y);
                crss_pts.push_back(p);
            }
        }
        for(int x=0;x<450/grd-1;x++)
        {
            int point_type=chk_point(x,y);
            if(point_type==1)
                    pen=!pen;
            if(pen)
                draw_pxl_clr(x,y,grd,r,g,b);
        }
    }
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

void MainWindow::drw_line(int x0,int y0,int xn,int yn){
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    if(abs(xn-x0)>=abs(yn-y0))
    {
        for( int ix=((x0<xn)? x0:xn); ix<=((x0<xn)? xn:x0); ix++ )
        {
            float fy = y0 + float(yn-y0)/float(xn-x0)*(ix-x0);
            int iy = (int)(fy+0.5);
            draw_blu_pxl(ix,iy,grd);
            ui->frame->setPixmap(QPixmap::fromImage(img));
        }
    }
    else
    {
        for( int iy=((y0<yn)? y0:yn); iy<=((y0<yn)? yn:y0); iy++ )
        {
            float fx = x0 + float(xn-x0)/float(yn-y0)*(iy-y0);
            int ix = (int)(fx+0.5);
            draw_blu_pxl(ix,iy,grd);
            ui->frame->setPixmap(QPixmap::fromImage(img));
        }
    }
}

void MainWindow::drw_red_line(int x0,int y0,int xn,int yn){
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
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

void MainWindow::Mouse_Pressed()
{
    QString s=ui->lineEdit->text();
    int grd=s.toLong();
    if(grd==0)  grd=1;
    int xyaxis=225/grd;
    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x/grd-xyaxis)+", Y : "+QString::number(ui->frame->y/grd-xyaxis));
    point(ui->frame->x,ui->frame->y,grd);

    if(ui->draw_line->isChecked()){
        QPoint p;
        p.setX(ui->frame->x/grd);
        p.setY(ui->frame->y/grd);
        points.push_back(p);
        if(points.size()>0 && points.size()%2==0){

            int x0 = points[points.size()-2].x(),y0 = points[points.size()-2].y();
            int xn = points[points.size()-1].x(),yn = points[points.size()-1].y();
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
    }

    if(ui->draw_rect->isChecked()){
        QPoint p;
        p.setX(ui->frame->x/grd);
        p.setY(ui->frame->y/grd);
        points.push_back(p);
        if(points.size()>0 && points.size()%2==0){
            qDebug() << "hi\n";
            int x0 = points[points.size()-2].x(),y0 = points[points.size()-2].y();
            int xn = points[points.size()-1].x(),yn = points[points.size()-1].y();
            drw_line(x0,y0,x0,yn);
            drw_line(x0,y0,xn,y0);
            drw_line(x0,yn,xn,yn);
            drw_line(xn,y0,xn,yn);
        }
    }

    if(ui->draw_poly->isChecked()){
        QPoint p;
        p.setX(ui->frame->x/grd);
        p.setY(ui->frame->y/grd);
        poly.push_back(p);
        if(poly.size()>1){
            drw_line(poly[poly.size()-2].x(),poly[poly.size()-2].y(),poly[poly.size()-1].x(),poly[poly.size()-1].y());
        }
    }
}

void MainWindow::on_complete_clicked(){
    drw_line(poly[poly.size()-1].x(),poly[poly.size()-1].y(),poly[0].x(),poly[0].y());
    //scanline_fill(5,0,0,255);
    poly.clear();
}

void MainWindow::on_Draw_clicked()
{
    QString s=ui->lineEdit->text();
    if (s.length() == 0) {
        s = QString("10");
    }
    int grd=s.toLong() > 5 ? s.toLong() : 5 ;
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

void draw_pxl(int x0,int y0,int grd)
{
    for(int a0=x0*grd;a0<=x0*grd+grd;a0++)
        for(int b0=y0*grd;b0<=y0*grd+grd;b0++)
            if(a0<450 && b0<450)
                img.setPixel(a0,b0,qRgb(255,0,0));
}

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000


int computeCode(double x, double y)
{
    // initialized as being inside
    int code = INSIDE;

    if (x < x_min)       // to the left of rectangle
        code |= LEFT;
    else if (x > x_max)  // to the right of rectangle
        code |= RIGHT;
    if (y < y_min)       // below the rectangle
        code |= BOTTOM;
    else if (y > y_max)  // above the rectangle
        code |= TOP;

    return code;
}

// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
void MainWindow::cohenSutherlandClip(double x1, double y1,double x2, double y2)
{
    // Compute region codes for P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Initialize line as outside the rectangular window
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            break;
        }
        else
        {
            int code_out;
            double x, y;
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            if (code_out & TOP)
            {
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM)
            {
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT)
            {
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT)
            {
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }
            if (code_out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept)
    {
        drw_line(x1,y1,x2,y2);
        qDebug() << x1 << ", "
                 << y1 << " to "<< x2 << ", " << y2 << endl;
    }
}

void draw_blu_pxl(int x0,int y0,int grd)
{
    for(int a0=x0*grd;a0<=x0*grd+grd;a0++)
        for(int b0=y0*grd;b0<=y0*grd+grd;b0++)
            if(a0<450 && b0<450)
                img.setPixel(a0,b0,qRgb(0,230,255));
}

void MainWindow::on_clip_clicked(){
    x_min = points[points.size()-2].x();
    y_min = points[points.size()-2].y();
    x_max = points[points.size()-1].x();
    y_max = points[points.size()-1].y();
    for(int i=0;i<points.size()-2;i+=2){
        cohenSutherlandClip(points[i].x(),points[i].y(),points[i+1].x(),points[i+1].y());
    }

}

int x_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1*y2 - y1*x2) * (x3-x4) -
              (x1-x2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}

// Returns y-value of point of intersectipn of
// two lines
int y_intersect(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4)
{
    int num = (x1*y2 - y1*x2) * (y3-y4) -
              (y1-y2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}

// This functions clips all the edges w.r.t one clip
// edge of clipping area
void poly_clip(int poly_points[][2], int &poly_size,
          int x1, int y1, int x2, int y2)
{
    int new_points[100][2], new_poly_size = 0;

    // (ix,iy),(kx,ky) are the co-ordinate values of
    // the points
    for (int i = 0; i < poly_size; i++)
    {
        // i and k form a line in polygon
        int k = (i+1) % poly_size;
        int ix = poly_points[i][0], iy = poly_points[i][1];
        int kx = poly_points[k][0], ky = poly_points[k][1];

        // Calculating position of first point
        // w.r.t. clipper line
        int i_pos = (x2-x1) * (iy-y1) - (y2-y1) * (ix-x1);

        // Calculating position of second point
        // w.r.t. clipper line
        int k_pos = (x2-x1) * (ky-y1) - (y2-y1) * (kx-x1);

        // Case 1 : When both points are inside
        if (i_pos < 0  && k_pos < 0)
        {
            //Only second point is added
            new_points[new_poly_size][0] = kx;
            new_points[new_poly_size][1] = ky;
            new_poly_size++;
        }

        // Case 2: When only first point is outside
        else if (i_pos >= 0  && k_pos < 0)
        {
            // Point of intersection with edge
            // and the second point is added
            new_points[new_poly_size][0] = x_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            new_points[new_poly_size][1] = y_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            new_poly_size++;

            new_points[new_poly_size][0] = kx;
            new_points[new_poly_size][1] = ky;
            new_poly_size++;
        }

        // Case 3: When only second point is outside
        else if (i_pos < 0  && k_pos >= 0)
        {
            //Only point of intersection with edge is added
            new_points[new_poly_size][0] = x_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            new_points[new_poly_size][1] = y_intersect(x1,
                              y1, x2, y2, ix, iy, kx, ky);
            new_poly_size++;
        }

        // Case 4: When both points are outside
        else
        {
            //No points are added
        }
    }

    // Copying new points into original array
    // and changing the no. of vertices
    poly_size = new_poly_size;
    for (int i = 0; i < poly_size; i++)
    {
        poly_points[i][0] = new_points[i][0];
        poly_points[i][1] = new_points[i][1];
    }
}

// Implements Sutherland–Hodgman algorithm
void MainWindow::suthHodgClip(int poly_points[][2], int poly_size,
                  int clipper_points[][2], int clipper_size)
{
    //i and k are two consecutive indexes
    for (int i=0; i<clipper_size; i++)
    {
        int k = (i+1) % clipper_size;

        // We pass the current array of vertices, it's size
        // and the end points of the selected clipper line
        poly_clip(poly_points, poly_size, clipper_points[i][0],
             clipper_points[i][1], clipper_points[k][0],
             clipper_points[k][1]);
    }

    // Printing vertices of clipped polygon
    poly.clear();
    for (int i=0; i < poly_size; i++){
        QPoint p;
        p.setX(poly_points[i][0]);
        p.setY(poly_points[i][1]);
        poly.push_back(p);
        qDebug() << '(' << poly_points[i][0] <<
                ", " << poly_points[i][1] << ") ";
        if(i>=1)drw_red_line(poly_points[i][0],poly_points[i][1],poly_points[i-1][0],poly_points[i-1][1]);
    }
    drw_red_line(poly_points[0][0],poly_points[0][1],poly_points[poly_size-1][0],poly_points[poly_size-1][1]);
}

void MainWindow::on_poly_clip_clicked(){
    x_min = points[points.size()-2].x();
    y_min = points[points.size()-2].y();
    x_max = points[points.size()-1].x();
    y_max = points[points.size()-1].y();
    int poly_size = poly.size();
    int poly_points[100][2];

    for(int i=0;i<poly_size;i++){
        poly_points[i][0] = poly[i].x();
        poly_points[i][1] = poly[i].y();
    }

    // Defining clipper polygon vertices in clockwise order

    int clipper_size = 4;
    int clipper_points[][2] = {{x_min,y_min}, {x_min,y_max},
                              {x_max,y_max}, {x_max,y_min} };


    //Calling the clipping function
    suthHodgClip(poly_points, poly_size, clipper_points,
                 clipper_size);
    scanline_fill(5,255,0,0);
}

void MainWindow::on_clear_clicked()
{
    on_Draw_clicked();
}
