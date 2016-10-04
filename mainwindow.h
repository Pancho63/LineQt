#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include "oscpkt.hh"
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QKeyEvent>

using namespace oscpkt;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QUdpSocket  *udpSocket;
    QGraphicsRectItem *rect1;
    QGraphicsEllipseItem *ellipse1;
    QGraphicsRectItem *rect2;
    QGraphicsEllipseItem *ellipse2;
    QGraphicsScene *m_graphicsScene;

private slots:
    void processPendingDatagrams();
    void pan(int, int);
    void tilt(int, int);
    void largeur(int, int);
    void hauteur(int, int);
    void rotate(int, int);
    void red(int, int);
    void green(int, int);
    void blue(int, int);
    void thickness(int, int);
    void ligneUpdate();
    void fullscreen();

};

#endif // MAINWINDOW_H
