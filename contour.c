#include "contour.h"
#include <stdio.h>


void CreateContour(struct Contour* cnt)
{
    cnt->tm_stamp = 0;
    cvector_init(&(cnt->vVertices));
}

void addXY(struct Contour* cnt, double x,double y)
{
    cvector_add(&(cnt->vVertices), x);
    cvector_add(&(cnt->vVertices), y);
    //vVertices.append(x);
    //vVertices.append(y);
}


int getPointCount(struct Contour* cnt)
{
    //return vVertices.count()/2;
    return cvector_count(&(cnt->vVertices))/2;
}

double* getdata(struct Contour* cnt)
{
    return cnt->vVertices.data;
}

void removeLast(struct Contour* cnt)
{
    if (getPointCount(cnt)>0)
    {
        cvector_delete(&(cnt->vVertices),cvector_count(&(cnt->vVertices)));
        cvector_delete(&(cnt->vVertices),cvector_count(&(cnt->vVertices)));
    }
}


int findAtXY(struct Contour* cnt, double x, double y, double okr)
{
    int c=getPointCount(cnt);
    double dist;
    double distmin;
    int id=-1;
    double d2=okr*okr;
    for(int i=0;i<c;i++)
    {
        double x1 = cvector_get(&(cnt->vVertices),2*i);
        double y1 = cvector_get(&(cnt->vVertices),2*i+1);
        dist=(x-x1)*(x-x1)+(y-y1)*(y-y1);
        if ((dist<d2)&&((id==-1)||(dist<distmin)))
        {
            id=i;
            distmin=dist;
        }
    }
    return id;
}


void setById(struct Contour* cnt, int id,double x, double y)
{
    int pcnt=getPointCount(cnt);
    if ((id>=0)&&(id<pcnt))
    {
        cvector_set(&(cnt->vVertices),2*id,x);
        cvector_set(&(cnt->vVertices),2*id+1,y);
        //vVertices[2*id]=x;
        //vVertices[2*id+1]=y;
    }
}


int getMinMax(struct Contour* cnt, double *minx, double *miny, double *maxx, double *maxy)
{
    int c=getPointCount(cnt);
    if (c==0)
    {
        return 0; // Возвращаяет ошибку
    }
    else
    {
        *minx=*maxx=cvector_get(&(cnt->vVertices),0);
        *miny=*maxy=cvector_get(&(cnt->vVertices),1);
    }
    for(int i=1;i<c;i++)
    {
        double x1 = cvector_get(&(cnt->vVertices),2*i);
        double y1 = cvector_get(&(cnt->vVertices),2*i+1);
        
        if (*minx>x1)
            *minx=x1;
        if (*maxx<x1)
            *maxx=x1;
        if (*miny>y1)
            *miny=y1;
        if (*maxy<y1)
            *maxy=y1;
    }
    return 1;
}

/*
void Contour::saveToFile(QTextStream& out)
{
    int c=getPointCount();
    for (int i=0;i<c;i++)
    {
        out << vVertices[2*i]<<":"<< vVertices[2*i+1] <<"\n";
    }
    out << "#"<<getTime()<<"\n";
}
*/

void printOut(struct Contour* cnt)
{
    int c=getPointCount(cnt);
    for (int i=0;i<c;i++)
    {
        double x1 = cvector_get(&(cnt->vVertices),2*i);
        double y1 = cvector_get(&(cnt->vVertices),2*i+1);

        printf ("%g:%g \n",x1,y1);
    }
    //out << "#"<<getTime()<<"\n";
}

/*
void Contour::loadFromFile(QTextStream& in)
{
    QString line = in.readLine();
}
*/

void removeAt(struct Contour* cnt, int id)
{
    int c=getPointCount(cnt);
    if ((id>-1)&&(id<c))
    {
        cvector_delete(&(cnt->vVertices),2*id);
        cvector_delete(&(cnt->vVertices),2*id);
        //vVertices.removeAt(2*id);
        //vVertices.removeAt(2*id);
    }
}


/*
float* Contour::fgetdata()
{
    float* otmp;
    double* itmp;
    int c = vVertices.size();
    otmp = new float[c];
    itmp = vVertices.data();
    std::copy(itmp,itmp+c,otmp);
    return otmp;
}

*/

double getTime(struct Contour* cnt)
{
    return cnt->tm_stamp;
}

void setTime(struct Contour* cnt, double t)
{
    cnt->tm_stamp = t;
}
