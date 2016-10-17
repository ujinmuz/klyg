#ifndef L19MAXWELL_H
#define L19MAXWELL_H

#include <deque>

using namespace std;


class L19Maxwell
{
public:
    struct l19interval
    {
        double x1;
        double x2;
    };

public:
    L19Maxwell();
    static int calcOrientSurface(double *vertises, int startPtId, int stopPtId, double GapXValue, double &res, int &fpt, int &lpt, double &fY, double &lY);
    static int MaxwellRule(double *vertises, int startPtId, int stopPtId, double x1, double x2, double delta, double &GapXValue, int &fpt, int &lpt, double &fY, double &lY);
    static int separateReverseInterval(double *vertises, int startPtId, int stopPtId, l19interval& interval, int& startPtIdNew, int& stopPtIdNew);
    static int sign(float x);
    static int sign(int x);
    static int sign(double x);
    static double OAreaTr(double x1, double y1, double x2, double y2, double x3, double y3);
    static int debug(double *vertises, int startPtId, int stopPtId);
    static bool PointIsBetween(double x1, double x2, double x);
    static double frexp10(double arg, int * exp);
};

#endif // L19MAXWELL_H
