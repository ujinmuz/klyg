#include "l19maxwell.h"
#include <QDebug>

L19Maxwell::L19Maxwell()
{

}


int L19Maxwell::calcOrientSurface(double *vertises,int startPtId, int stopPtId, double GapXValue, double &res, int &fpt, int &lpt, double &fY, double &lY)
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
    res = 0;
    res += (vertises[2*fp]-GapXValue)*(vertises[2*fp+1]+fy);
    for (int i=fp;i<lp;i++)
    {
        res += (vertises[2*i+2]-vertises[2*i])*(vertises[2*i+3]+vertises[2*i+1]);
    }
    res += (GapXValue-vertises[2*lp])*(ly+vertises[2*lp+1]);
    res = res/2;

    fpt=fp;
    lpt=lp;
    fY=fy;
    lY=ly;

    return 1;
}


int L19Maxwell::MaxwellRule(double *vertises, int startPtId, int stopPtId, double x1, double x2, double delta, double &GapXValue, int &fpt, int &lpt, double &fY, double &lY)
{
    int lp1,fp1,lp2,fp2,lpc,fpc;
    double fy1,ly1,fy2,ly2,fyc,lyc;
    double xx1,xx2;
    xx1=x1;
    xx2=x2;
    double res1,res2,resc;

    int e1,e2,ec;

    e1=L19Maxwell::calcOrientSurface(vertises,startPtId,stopPtId,xx1,res1,fp1,lp1,fy1,ly1);
    if (!e1)
    {
        return 0;
    }
    e2=L19Maxwell::calcOrientSurface(vertises,startPtId,stopPtId,xx2,res2,fp2,lp2,fy2,ly2);
    if (!e2)
    {
        return 0;
    }
    if (fabs(res1)<delta)
    {
        GapXValue=xx1;
        fpt=fp1;
        lpt=lp1;
        fY=fy1;
        lY=ly1;
        return 1;
    }
    else if ((fabs(res2)<delta))
    {
        GapXValue=xx2;
        fpt=fp2;
        lpt=lp2;
        fY=fy2;
        lY=ly2;
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
            ec=L19Maxwell::calcOrientSurface(vertises,startPtId,stopPtId,xc,resc,fpc,lpc,fyc,lyc);
            if (!ec)
            {
                return 0;
            }
            else if (fabs(resc)<delta)
            {
                GapXValue=xc;
                fpt=fpc;
                lpt=lpc;
                fY=fyc;
                lY=lyc;
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


int L19Maxwell::separateReverseInterval(double *vertises, int startPtId, int stopPtId, l19interval& interval, int& startPtIdNew, int& stopPtIdNew)
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
    deque<int> revpts;
    revpts.push_back(startPtId);


    for (int i=startPtId+1;i<stopPtId;i++)
    {
        nsignx=sign(vertises[2*i+2]-vertises[2*i]);
        if (nsignx==-crntsignx)
        {                        
            revpts.push_back(i);            
            if (revpts.size()==4)
            {
                if (PointIsBetween(vertises[2*revpts[1]],vertises[2*revpts[2]],vertises[2*revpts[0]])||PointIsBetween(vertises[2*revpts[1]],vertises[2*revpts[2]],vertises[2*revpts[3]]))
                {
                    revpts.pop_front();
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
               revpts.pop_front();

            }            
            crntsigny=nsigny;
        }
    }
    if (revpts.size()==3)
    {
        revpts.push_back(stopPtId);     
    }

    if (revpts.size()==4)
    {
        startPtIdNew=revpts[0];
        stopPtIdNew=revpts[3];
        interval.x2=vertises[2*revpts[2]];
        interval.x1=vertises[2*revpts[1]];
        return 1;
    }
    return 0; ////!!!!! change
}


int L19Maxwell::sign(float x)
{
    return (x > 0) - (x < 0);
}

int L19Maxwell::sign(double x)
{
    return (x > 0) - (x < 0);
}


int L19Maxwell::sign(int x)
{
    return (x > 0) - (x < 0);
}


int L19Maxwell::debug(double *vertises, int startPtId, int stopPtId)
{
    vector<l19interval> v;
    //separateReverseInterval(vertises,startPtId,stopPtId,v);
    return 0;
}


double L19Maxwell::OAreaTr(double x1,double y1,double x2,double y2,double x3,double y3)
{
  double s;
  s=(x1-x2)*(y1+y2);
  s+=(x2-x3)*(y2+y3);
  s+=(x3-x1)*(y3+y1);
  return s/2;
}


bool L19Maxwell::PointIsBetween(double x1, double x2, double x)
{
    return ((x1<=x)&&(x<=x2))||((x2<=x)&&(x<=x1));
}



double L19Maxwell::frexp10(double arg, int * exp)
{
   *exp = (arg == 0) ? 0 : ceil( std::log10(std::fabs(arg) ) );
   return arg * pow(10 , -(*exp));
}


