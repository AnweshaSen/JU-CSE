#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    //boolean b = mouseMoveEvent(QMouseEvent *e);
    void showMousePosition(QPoint& pos);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_show_axes_clicked();

    void on_Draw_clicked();

    void on_set_point1_clicked();

    void on_set_point2_clicked();

    void draw_grid(QPixmap *pix,QPainter *paint);

    void setPix(int x,int y,QPixmap *pix,QPainter *paint);

private:
    void linearEqnLine(QPixmap *pix, QPainter *paint);
    void dda_line(QPixmap *pix,QPainter *paint);
    void bresen_line(QPixmap *pix,QPainter *paint);

    void param_circle(int r,QPixmap *pix, QPainter *paint);
    void midp_circle(int r,QPixmap *pix, QPainter *paint);
    void bresen_circle(int r,QPixmap *pix, QPainter *paint);

    void param_ellipse(int rx, int ry, QPixmap *pix, QPainter *paint);
    void midp_ellipse(int rx, int ry, QPixmap *pix, QPainter *paint);

    void point(int,int,int);

    QQueue<QPoint> pointQ;
    Ui::MainWindow *ui;
    QPoint p1,p2;
    int gridSize;
};

#endif // MAINWINDOW_H
