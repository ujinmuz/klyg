#include "contourcollection.h"
#include <QDebug>
#include <QRegExp>

ContourCollection::ContourCollection()
{

}



void ContourCollection::append(Contour* C)
{
    Contours.append(C);
}

Contour* ContourCollection::getById(int id)
{
    return Contours[id];
}

void ContourCollection::addNew()
{
    Contour *C = new Contour();
    append(C);
}

int ContourCollection::getCount()
{
    return Contours.count();
}


void ContourCollection::deleteById(int id)
{
    int cnt=Contours.count();
    if ((id>=0)&&(id<cnt))
    {
        delete Contours[id];
        Contours.removeAt(id);
    }
}


int ContourCollection::getMinMax(double &minx, double &miny, double &maxx, double &maxy)
{
    int c=Contours.count();
    if (c==0)
    {
        return 0;
    }
    else
    {
        Contours[0]->getMinMax(minx,miny,maxx,maxy);
    }

    for(int i=1;i<c;i++)
    {
        double x1,x2,y1,y2;
        Contours[i]->getMinMax(x1,y1,x2,y2);
        if (x1<minx)
            minx=x1;
        if (y1<miny)
            miny=y1;
        if (x2>maxx)
            maxx=x2;
        if (y2<maxy)
            maxy=y2;
    }
    return 1;
}

int ContourCollection::getMinMaxByID(int id, double &minx, double &miny, double &maxx, double &maxy)
{
    int c=Contours.count();
    if ((id<0)||(id>c))
    {
        return 0;
    }
    return Contours[id]->getMinMax(minx,miny,maxx,maxy);
}


void ContourCollection::saveToFile(QTextStream &out)
{
    out << "#klyg_v1\n";
    int c=Contours.count();
    for (int i=0; i<c;i++)
    {
        Contours[i]->saveToFile(out);
    }
}


void ContourCollection::loadFromFile(QTextStream& in)
{
    QString line = in.readLine();

    if (line.trimmed()!="#klyg_v1")
    {
        qDebug()<<"Попа!";
        return;
    }
    Clear();
    bool neednew = true;
    int indx=-1;
    Contour *Cn;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QRegExp rx("([-+]?\\d*\\.?\\d*)\\s*:\\s*([-+]?\\d*\\.?\\d*)");
        int pos=0;
        pos = rx.indexIn(line, pos);
        if (pos>-1)
        {
            if (neednew)
            {
                addNew();
                indx++;
                Cn=getById(indx);
                neednew=false;
            }
            Cn->addXY(rx.cap(1).toDouble(),rx.cap(2).toDouble());


            //qDebug()<<ptcnt1 << " - "<<ptcnt2;
        }
        else if (line.trimmed()=="#")
        {
            neednew=true;
        }
    }
}

void ContourCollection::Clear()
{
    int c=Contours.count();
    for(int i=0;i<c;i++)
    {
        delete Contours[i];
    }
    Contours.clear();
}
