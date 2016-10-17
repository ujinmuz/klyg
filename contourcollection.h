#ifndef CONTOURCOLLECTION_H
#define CONTOURCOLLECTION_H

#include "contour.h"
#include <QList>



class ContourCollection
{
public:
    ContourCollection();
    void append(Contour* C);
    Contour* getById(int id);
    void addNew();
    int getCount();
    void deleteById(int id);
    int getMinMax(double &minx, double &miny, double &maxx, double &maxy);
    int getMinMaxByID(int id, double &minx, double &miny, double &maxx, double &maxy);
    void saveToFile(QTextStream& out);
    void loadFromFile(QTextStream& in);
    void Clear();
private:
    QList <Contour*> Contours;

};

#endif // CONTOURCOLLECTION_H
