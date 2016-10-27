#ifndef L19MAXWELL_H
#define L19MAXWELL_H

#include <stdbool.h>

    struct l19interval
    {
        double x1;
        double x2;
    };


     int calcOrientSurface(double *vertises, int startPtId, int stopPtId, double GapXValue, double *res, int *fpt, int *lpt, double *fY, double *lY);
     int MaxwellRule(double *vertises, int startPtId, int stopPtId, double x1, double x2, double delta, double *GapXValue, int *fpt, int *lpt, double *fY, double *lY);
     int separateReverseInterval(double *vertises, int startPtId, int stopPtId, struct l19interval* interval, int* startPtIdNew, int* stopPtIdNew);
     //int sign(float x);
     //int sign(int x);
     int sign(double x);
     double OAreaTr(double x1, double y1, double x2, double y2, double x3, double y3);
     bool PointIsBetween(double x1, double x2, double x);
     double frexp10(double arg, int * exp);
     void test(double* s, int* n);


#endif // L19MAXWELL_H
