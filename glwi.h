#ifndef GLWI_H
#define GLWI_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMatrix>
#include <QVector>
#include "contourcollection.h"

struct axisLine
{
    int win_val;
    double real_val;
};


class GLWi : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWi(QWidget *parent = 0);
    qint32 lstate;
    int selected_pid;
    void setLayerId(int id);
    int getLayerId();
    void addNewLayer();
    int getLayerCount();
    void deleteLayerById(int id);
    void saveToFile(QTextStream& out);
    void loadFromFile(QTextStream& in);

signals:

public slots:
    void CenterByCur();
    void CenterByAll();
    void calc();
    void SwitchGrid();
    void save_png();

protected:
    QVector <axisLine> xLines;
    QVector <axisLine> yLines;
    int lnpresX;
    int lnpresY;
    double cent_x, cent_y;
    double mash;
    ContourCollection *Cnt;
    int selected_lay;
    bool showgrid;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void initShaders();
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event);
    void MakeMtx(int w, int h);
    void CalcGrid();
    void ResolveByMaxwellRule(Contour *Cont);

    void paintGLoffscreen(int layid, QString dir);

    void calcCenter(double minx, double miny, double maxx, double maxy);


    QOpenGLShaderProgram program;
    QMatrix4x4 projection;
    QMatrix4x4 look;
    QMatrix4x4 vMx;
    QMatrix r2Mx;

};

#endif // GLWI_H
