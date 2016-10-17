#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSpinBox>
#include <QLabel>
#include "glwi.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QAction *m_2d;
    QAction *m_3d;
    QAction *m_adl;
    QAction *m_rml;
    QAction *m_plus;
    QAction *m_minus;
    qint32 dstate;
    GLWi *openGL2D;
    QSpinBox *spinBox;
    QLabel *lb1;
    QAction *m_lbBox;
    QAction *m_sBox;
    QAction *m_center;
    QAction *m_centerN;
    QStackedWidget *stackedWidget;
    QAction *m_sep2;
    QAction *m_calc;
    QAction *m_grid;
    void after2DClick();
    void after3DClick();
private slots:
    void open();
    void save();
    void d2Click();
    void d3Click();
    void adlClick();
    void rmlClick();
    void plusClick();
    void minusClick();
    void chLayer(int i);


};

#endif // MAINWINDOW_H
