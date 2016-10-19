#ifndef CONTOUR_H
#define CONTOUR_H

#include <QObject>
#include <QVector>
#include <QTextStream>

class Contour
{
public:
    explicit Contour(QObject *parent = 0);
    void addXY(double x, double y);
    int getPointCount();
    double* getdata();
    float* fgetdata();
    void removeLast();
    int findAtXY(double x, double y, double okr);
    void setById(int id,double x, double y);
    int getMinMax(double &minx, double &miny, double &maxx, double &maxy);
    void saveToFile(QTextStream& out);
    void loadFromFile(QTextStream& in);
    void removeAt(int id);
    double getTime();
    void setTime(double t);
signals:

public slots:

protected:
    QVector<double> vVertices;
    double tm_stamp;
};

#endif // CONTOUR_H
