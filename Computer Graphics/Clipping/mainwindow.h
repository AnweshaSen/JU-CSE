#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint& pos);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Draw_clicked();
    void on_clip_clicked();
    void on_poly_clip_clicked();
    void on_complete_clicked();
    void cohenSutherlandClip(double x1, double y1,
                             double x2, double y2);
    void drw_line(int x0,int y0,int xn,int yn);
    void drw_red_line(int x0,int y0,int xn,int yn);
    void suthHodgClip(int poly_points[][2], int poly_size,
                      int clipper_points[][2], int clipper_size);

    void on_clear_clicked();

private:
    Ui::MainWindow *ui;
    QPoint p1,p2;
    void point(int,int,int);
};

#endif // MAINWINDOW_H
