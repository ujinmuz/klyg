#include "contour.h"


Contour::Contour(QObject *parent)
{
    tm_stamp = 0;
}

void Contour::addXY(double x,double y)
{
    vVertices.append(x);
    vVertices.append(y);
}


int Contour::getPointCount()
{
    return vVertices.count()/2;
}

double* Contour::getdata()
{
    return vVertices.data();
}

void Contour::removeLast()
{
    if (!vVertices.isEmpty())
        vVertices.removeLast();
    if (!vVertices.isEmpty())
        vVertices.removeLast();
}


int Contour::findAtXY(double x, double y, double okr)
{
    int c=getPointCount();
    double dist;
    double distmin;
    int id=-1;
    double d2=okr*okr;
    for(int i=0;i<c;i++)
    {
        dist=(x-vVertices[2*i])*(x-vVertices[2*i])+(y-vVertices[2*i+1])*(y-vVertices[2*i+1]);
        if ((dist<d2)&&((id==-1)||(dist<distmin)))
        {
            id=i;
            distmin=dist;
        }
    }
    return id;
}


void Contour::setById(int id,double x, double y)
{
    int pcnt=getPointCount();
    if ((id>=0)&&(id<pcnt))
    {
        vVertices[2*id]=x;
        vVertices[2*id+1]=y;
    }
}


int Contour::getMinMax(double &minx, double &miny, double &maxx, double &maxy)
{
    int c=getPointCount();
    if (c==0)
    {
        return 0; // Возвращаяет ошибку
    }
    else
    {
        minx=maxx=vVertices[0];
        miny=maxy=vVertices[1];
    }
    for(int i=1;i<c;i++)
    {
        if (minx>vVertices[2*i])
            minx=vVertices[2*i];
        if (maxx<vVertices[2*i])
            maxx=vVertices[2*i];
        if (miny>vVertices[2*i+1])
            miny=vVertices[2*i+1];
        if (maxy<vVertices[2*i+1])
            maxy=vVertices[2*i+1];
    }
    return 1;
}


void Contour::saveToFile(QTextStream& out)
{
    int c=getPointCount();
    for (int i=0;i<c;i++)
    {
        out << vVertices[2*i]<<":"<< vVertices[2*i+1] <<"\n";
    }
    out << "#"<<getTime()<<"\n";
}

void Contour::loadFromFile(QTextStream& in)
{
    QString line = in.readLine();
}


void Contour::removeAt(int id)
{
    int c=getPointCount();
    if ((id>-1)&&(id<c))
    {
        vVertices.removeAt(2*id);
        vVertices.removeAt(2*id);
    }
}



float* Contour::fgetdata()
{
    float* otmp;
    double* itmp;
    int c = vVertices.count();
    otmp = new float[c];
    itmp = vVertices.data();
    std::copy(itmp,itmp+c,otmp);
    return otmp;
}



double Contour::getTime()
{
    return tm_stamp;
}

void Contour::setTime(double t)
{
    tm_stamp = t;
}
