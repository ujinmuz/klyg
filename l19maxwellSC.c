#include "l19maxwellSC.h"
#include "contour.h"
#include <math.h>
#include "cdeque.h"
#include "vector.h"


void ResolveByMaxwellRule(struct Contour *Cont) {
    struct l19interval v;
    bool solved;
    double res;
    int lp, fp;
    double fy, ly;
    int start = 0;
    int stop = getPointCount(Cont) - 1;
    int strt;
    int stp;
    double minx, miny, maxx, maxy;
    getMinMax(Cont,&minx, &miny, &maxx, &maxy);
    solved = true;
    while (separateReverseInterval(getdata(Cont), start, stop, &v, &strt, &stp) && solved) {
        solved = false;
        double x1 = v.x2;
        double x2 = v.x1;
        double eps = fabs((maxx - minx) * (maxy - miny) / 100000000);
        if (MaxwellRule(getdata(Cont), strt, stp, x1, x2, eps, &res, &fp, &lp, &fy, &ly)) {
            solved = true;
            setById(Cont,fp, res, fy);
            setById(Cont,lp, res, ly);
            for (int i = fp + 1; i < lp; i++) {
                removeAt(Cont,fp + 1);
            }
            stop = getPointCount(Cont) - 1;
        }
    }
}
int calcOrientSurface(double *vertises,int startPtId, int stopPtId, double GapXValue, double *res, int *fpt, int *lpt, double *fY, double *lY)
{
    if ((stopPtId-startPtId)<2)
    {
        return 0; // недостаточно точек
    }
    int fp=-1;
    int lp=-1;
    double fy,ly;
    for(int i=startPtId;i<stopPtId;i++)
    {
        double x1=vertises[2*i];
        double x2=vertises[2*i+2];
        if(((x1>=GapXValue)&&(x2<GapXValue))||((x1<=GapXValue)&&(x2>GapXValue)))
        {
            fp = i+1;
            fy=vertises[2*i+1]+(GapXValue-x1)*(vertises[2*i+3]-vertises[2*i+1])/(x2-x1);
            break;
        }
    }
    for(int i=fp;i<stopPtId;i++)
    {
        double x1=vertises[2*i];
        double x2=vertises[2*i+2];
        if(((x1>=GapXValue)&&(x2<GapXValue))||((x1<=GapXValue)&&(x2>GapXValue)))
        {
            lp = i;
            ly=vertises[2*i+1]+(GapXValue-x1)*(vertises[2*i+3]-vertises[2*i+1])/(x2-x1);
        }
    }

    double x1t=vertises[2*stopPtId];

    if (x1t==GapXValue)
    {
        lp = stopPtId;
        ly=vertises[2*stopPtId+1];
    }

    if (lp==-1)
    {
        return 0; // недостаточно пересечений
    }
    *res = 0;
    *res += (vertises[2*fp]-GapXValue)*(vertises[2*fp+1]+fy);
    for (int i=fp;i<lp;i++)
    {
        *res += (vertises[2*i+2]-vertises[2*i])*(vertises[2*i+3]+vertises[2*i+1]);
    }
    *res += (GapXValue-vertises[2*lp])*(ly+vertises[2*lp+1]);
    *res = *res/2;

    *fpt=fp;
    *lpt=lp;
    *fY=fy;
    *lY=ly;

    return 1;
}




int MaxwellRule(double *vertises, int startPtId, int stopPtId, double x1, double x2, double delta, double *GapXValue, int *fpt, int *lpt, double *fY, double *lY)
{
    int lp1,fp1,lp2,fp2,lpc,fpc;
    double fy1,ly1,fy2,ly2,fyc,lyc;
    double xx1,xx2;
    xx1=x1;
    xx2=x2;
    double res1,res2,resc;

    int e1,e2,ec;

    e1=calcOrientSurface(vertises,startPtId,stopPtId,xx1,&res1,&fp1,&lp1,&fy1,&ly1);
    if (!e1)
    {
        return 0;
    }
    e2=calcOrientSurface(vertises,startPtId,stopPtId,xx2,&res2,&fp2,&lp2,&fy2,&ly2);
    if (!e2)
    {
        return 0;
    }
    if (fabs(res1)<delta)
    {
        *GapXValue=xx1;
        *fpt=fp1;
        *lpt=lp1;
        *fY=fy1;
        *lY=ly1;
        return 1;
    }
    else if ((fabs(res2)<delta))
    {
        *GapXValue=xx2;
        *fpt=fp2;
        *lpt=lp2;
        *fY=fy2;
        *lY=ly2;
        return 1;
    }
    else if ((res1*res2)>0)
    {
       return 0;
    }
    else
    {
        bool clc=true;
        while (clc)
        {
            double xc=(xx1+xx2)/2;
            ec=calcOrientSurface(vertises,startPtId,stopPtId,xc,&resc,&fpc,&lpc,&fyc,&lyc);
            if (!ec)
            {
                return 0;
            }
            else if (fabs(resc)<delta)
            {
                *GapXValue=xc;
                *fpt=fpc;
                *lpt=lpc;
                *fY=fyc;
                *lY=lyc;
                return 1;
            }
            else if ((res1*resc)<0)
            {
                xx2=xc;
                res2=resc;
            }
            else if ((res2*resc)<0)
            {
                xx1=xc;
                res1=resc;
            }
            else
            {
                return 0;
            }
        }
    }


}


int separateReverseInterval(double *vertises, int startPtId, int stopPtId, struct l19interval* interval, int* startPtIdNew, int* stopPtIdNew)
{
    if ((stopPtId-startPtId)<2)
    {
        return 0;
    }

    int stsignx=sign(vertises[2*startPtId+2]-vertises[2*startPtId]);
    int crntsignx=stsignx;
    int crntsigny=0;
    int nsignx;
    int nsigny;

    ////

    cdeque * revpts = cdeque_create();

    //deque<int> revpts;
    cdeque_push_back(revpts, startPtId);
    //revpts.push_back(startPtId);


    for (int i=startPtId+1;i<stopPtId;i++)
    {
        nsignx=sign(vertises[2*i+2]-vertises[2*i]);
        if (nsignx==-crntsignx)
        {
            cdeque_push_back(revpts,i);
            //revpts.push_back(i);
            if (revpts->count==4)
            {
                int i0,i1,i2,i3;
                i0 = cdeque_get_at(revpts,0);
                i1 = cdeque_get_at(revpts,1);
                i2 = cdeque_get_at(revpts,2);
                i3 = cdeque_get_at(revpts,3);
                if (PointIsBetween(vertises[2*i1],vertises[2*i2],vertises[2*i0])||PointIsBetween(vertises[2*i1],vertises[2*i2],vertises[2*i3]))
                {
                    cdeque_pop_front(revpts);
                }
                else
                {
                    break;
                }
            }
            crntsignx=nsignx;
            nsigny=sign(OAreaTr(vertises[2*i-2],vertises[2*i-1],vertises[2*i],vertises[2*i+1],vertises[2*i+2],vertises[2*i+3]));
            if (nsigny==crntsigny)
            {
                cdeque_pop_front(revpts);

            }            
            crntsigny=nsigny;
        }
    }
    if (revpts->count==3)
    {
        cdeque_push_back(revpts, stopPtId);
        //revpts.push_back(stopPtId);
    }

    if (revpts->count==4)
    {
        /*
        startPtIdNew=revpts[0];
        stopPtIdNew=revpts[3];
        interval.x2=vertises[2*revpts[2]];
        interval.x1=vertises[2*revpts[1]];
        */

        *startPtIdNew=cdeque_get_at(revpts,0);
        *stopPtIdNew=cdeque_get_at(revpts,3);
        interval->x2=vertises[2*(int)cdeque_get_at(revpts,2)];
        interval->x1=vertises[2*(int)cdeque_get_at(revpts,1)];

        return 1;
    }
    return 0; ////!!!!! change
}

/*
int sign(float x)
{
    return (x > 0) - (x < 0);
}
*/
int sign(double x)
{
    return (x > 0) - (x < 0);
}

/*
int sign(int x)
{
    return (x > 0) - (x < 0);
}
*/



double OAreaTr(double x1,double y1,double x2,double y2,double x3,double y3)
{
  double s;
  s=(x1-x2)*(y1+y2);
  s+=(x2-x3)*(y2+y3);
  s+=(x3-x1)*(y3+y1);
  return s/2;
}


bool PointIsBetween(double x1, double x2, double x)
{
    return ((x1<=x)&&(x<=x2))||((x2<=x)&&(x<=x1));
}


double frexp10(double arg, int * exp)
{
   *exp = (arg == 0) ? 0 : ceil( log10(fabs(arg) ) );
   return arg * pow(10 , -(*exp));
}


void test(double s[], int* n)
{
    struct Contour cnt;
    CreateContour(&cnt);

    addXY(&cnt,-0.161833f, 0.326151f);
    addXY(&cnt,0.00426493f, 0.348886f);
    addXY(&cnt,0.0641305f, 0.376182f);
    addXY(&cnt,0.115517f, 0.42324f);
    addXY(&cnt,0.119887, 0.435352f);
    addXY(&cnt,0.12121f, 0.448361f);
    addXY(&cnt,0.119318 , 0.462236);
    addXY(&cnt,0.114307 , 0.476931);
    addXY(&cnt,0.106539 , 0.492377);
    addXY(&cnt,0.0966066 , 0.508492);
    addXY(&cnt,0.0852513 , 0.52517);
    addXY(&cnt,0.0732591 , 0.54229);
    addXY(&cnt,0.061364 , 0.559711);
    addXY(&cnt,0.0501748 , 0.577277);
    addXY(&cnt,0.0401377 , 0.594817);
    addXY(&cnt,0.0315314 , 0.612147);
    addXY(&cnt,0.0244869 , 0.629077);
    addXY(&cnt,0.0190202 , 0.645408);
    addXY(&cnt,0.0150675 , 0.660941);
    addXY(&cnt,0.0125178 , 0.675479);
    addXY(&cnt,0.0112385 , 0.688829);
    addXY(&cnt,0.0110939 , 0.700812);
    addXY(&cnt,0.0119578 , 0.711259);
    addXY(&cnt,0.0137206 , 0.720024);
    addXY(&cnt,0.0162926 , 0.726978);
    addXY(&cnt,0.0196055 , 0.73202);
    addXY(&cnt,0.0236118 , 0.735076);
    addXY(&cnt,0.0282837 , 0.7361);
    addXY(&cnt,0.0336118 , 0.735076);
    addXY(&cnt,0.0396055 , 0.73202);
    addXY(&cnt,0.0462926 , 0.726978);
    addXY(&cnt,0.0537206 , 0.720024);
    addXY(&cnt,0.0619578 , 0.711259);
    addXY(&cnt,0.0710939 , 0.700812);
    addXY(&cnt,0.0812385 , 0.688829);
    addXY(&cnt,0.0925178 , 0.675479);
    addXY(&cnt,0.105068 , 0.660941);
    addXY(&cnt,0.11902 , 0.645408);
    addXY(&cnt,0.134487 , 0.629077);
    addXY(&cnt,0.151531 , 0.612147);
    addXY(&cnt,0.170138 , 0.594817);
    addXY(&cnt,0.190175 , 0.577277);
    addXY(&cnt,0.211364 , 0.559711);
    addXY(&cnt,0.233259 , 0.54229);
    addXY(&cnt,0.255251 , 0.52517);
    addXY(&cnt,0.276607 , 0.508492);
    addXY(&cnt,0.296539 , 0.492377);
    addXY(&cnt,0.314307 , 0.476931);
    addXY(&cnt,0.329318 , 0.462236);
    addXY(&cnt,0.34121 , 0.448361);
    addXY(&cnt,0.349887 , 0.435352);
    addXY(&cnt,0.355517 , 0.42324);
    addXY(&cnt,0.358474 , 0.412041);
    addXY(&cnt,0.359264 , 0.401753);
    addXY(&cnt,0.358449 , 0.392365);
    addXY(&cnt,0.356576 , 0.383852);
    addXY(&cnt,0.35413 , 0.376182);
    addXY(&cnt,0.351514 , 0.369314);
    addXY(&cnt,0.349036 , 0.363201);
    addXY(&cnt,0.346916 , 0.357795);
    addXY(&cnt,0.345298 , 0.353041);
    addXY(&cnt,0.344265 , 0.348886);
    addXY(&cnt,0.343852 , 0.345276);
    addXY(&cnt,0.344061 , 0.342157);
    addXY(&cnt,0.344868 , 0.339479);
    addXY(&cnt,0.346236 , 0.337191);
    addXY(&cnt,0.348118 , 0.335249);

    ResolveByMaxwellRule(&cnt);

    *n=cnt.vVertices.count;
    s=cnt.vVertices.data;
}


int main(int argc, char *argv[])
{
    struct Contour cnt;
    CreateContour(&cnt);

    addXY(&cnt,-0.161833f, 0.326151f);
    addXY(&cnt,0.00426493f, 0.348886f);
    addXY(&cnt,0.0641305f, 0.376182f);
    addXY(&cnt,0.115517f, 0.42324f);
    addXY(&cnt,0.119887, 0.435352f);
    addXY(&cnt,0.12121f, 0.448361f);
    addXY(&cnt,0.119318 , 0.462236);
    addXY(&cnt,0.114307 , 0.476931);
    addXY(&cnt,0.106539 , 0.492377);
    addXY(&cnt,0.0966066 , 0.508492);
    addXY(&cnt,0.0852513 , 0.52517);
    addXY(&cnt,0.0732591 , 0.54229);
    addXY(&cnt,0.061364 , 0.559711);
    addXY(&cnt,0.0501748 , 0.577277);
    addXY(&cnt,0.0401377 , 0.594817);
    addXY(&cnt,0.0315314 , 0.612147);
    addXY(&cnt,0.0244869 , 0.629077);
    addXY(&cnt,0.0190202 , 0.645408);
    addXY(&cnt,0.0150675 , 0.660941);
    addXY(&cnt,0.0125178 , 0.675479);
    addXY(&cnt,0.0112385 , 0.688829);
    addXY(&cnt,0.0110939 , 0.700812);
    addXY(&cnt,0.0119578 , 0.711259);
    addXY(&cnt,0.0137206 , 0.720024);
    addXY(&cnt,0.0162926 , 0.726978);
    addXY(&cnt,0.0196055 , 0.73202);
    addXY(&cnt,0.0236118 , 0.735076);
    addXY(&cnt,0.0282837 , 0.7361);
    addXY(&cnt,0.0336118 , 0.735076);
    addXY(&cnt,0.0396055 , 0.73202);
    addXY(&cnt,0.0462926 , 0.726978);
    addXY(&cnt,0.0537206 , 0.720024);
    addXY(&cnt,0.0619578 , 0.711259);
    addXY(&cnt,0.0710939 , 0.700812);
    addXY(&cnt,0.0812385 , 0.688829);
    addXY(&cnt,0.0925178 , 0.675479);
    addXY(&cnt,0.105068 , 0.660941);
    addXY(&cnt,0.11902 , 0.645408);
    addXY(&cnt,0.134487 , 0.629077);
    addXY(&cnt,0.151531 , 0.612147);
    addXY(&cnt,0.170138 , 0.594817);
    addXY(&cnt,0.190175 , 0.577277);
    addXY(&cnt,0.211364 , 0.559711);
    addXY(&cnt,0.233259 , 0.54229);
    addXY(&cnt,0.255251 , 0.52517);
    addXY(&cnt,0.276607 , 0.508492);
    addXY(&cnt,0.296539 , 0.492377);
    addXY(&cnt,0.314307 , 0.476931);
    addXY(&cnt,0.329318 , 0.462236);
    addXY(&cnt,0.34121 , 0.448361);
    addXY(&cnt,0.349887 , 0.435352);
    addXY(&cnt,0.355517 , 0.42324);
    addXY(&cnt,0.358474 , 0.412041);
    addXY(&cnt,0.359264 , 0.401753);
    addXY(&cnt,0.358449 , 0.392365);
    addXY(&cnt,0.356576 , 0.383852);
    addXY(&cnt,0.35413 , 0.376182);
    addXY(&cnt,0.351514 , 0.369314);
    addXY(&cnt,0.349036 , 0.363201);
    addXY(&cnt,0.346916 , 0.357795);
    addXY(&cnt,0.345298 , 0.353041);
    addXY(&cnt,0.344265 , 0.348886);
    addXY(&cnt,0.343852 , 0.345276);
    addXY(&cnt,0.344061 , 0.342157);
    addXY(&cnt,0.344868 , 0.339479);
    addXY(&cnt,0.346236 , 0.337191);
    addXY(&cnt,0.348118 , 0.335249);


    ResolveByMaxwellRule(&cnt);

    printOut(&cnt);
/*
    cvector v;
    cvector_init(&v);

    cvector_add(&v, 0.1);
    cvector_add(&v, 0.2);
    cvector_add(&v, 0.3);
    cvector_add(&v, 0.4);
    cvector_add(&v, 0.5);

    int i;
    std::cout << "first round:\n";
    for (i = 0; i < cvector_count(&v); i++) {
        std::cout << cvector_get(&v, i) << "\n";
        //printf("%s\n", (char*)cvector_get(&v, i));
    }

    cvector_delete(&v, 1);
    cvector_delete(&v, 3);

    std::cout << "second round:\n";
    for (i = 0; i < cvector_count(&v); i++) {
        std::cout << cvector_get(&v, i) << "\n";
        //printf("%s\n", (char*)cvector_get(&v, i));
    }

    cvector_free(&v);
*/

    return 0;
}