#ifndef CONTOUR_H
#define CONTOUR_H

//#include <QObject>
//#include <QVector>
//#include <QTextStream>

#include "vector.h"


    struct Contour
    {
        cvector vVertices;
        double tm_stamp;
    };

    void CreateContour(struct Contour* cnt);
    void addXY(struct Contour* cnt,double x, double y);
    int getPointCount(struct Contour* cnt);
    double* getdata(struct Contour* cnt);
    //float* fgetdata();
    void removeLast(struct Contour* cnt);
    int findAtXY(struct Contour* cnt,double x, double y, double okr);
    void setById(struct Contour* cnt,int id,double x, double y);
    int getMinMax(struct Contour* cnt,double* minx, double* miny, double* maxx, double* maxy);
    //void saveToFile(QTextStream& out);
    //void loadFromFile(QTextStream& in);
    void removeAt(struct Contour* cnt,int id);
    double getTime(struct Contour* cnt);
    void setTime(struct Contour* cnt,double t);
    void printOut(struct Contour* cnt);

    //QVector<double> vVertices;


#endif // CONTOUR_H
