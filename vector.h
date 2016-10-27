#ifndef CVECTOR_H__
#define CVECTOR_H__

typedef struct cvector_ {
    double* data;
    int size;
    int count;
} cvector;

void cvector_init(cvector*);
int cvector_count(cvector*);
void cvector_add(cvector*, double);
void cvector_set(cvector*, int, double);
double cvector_get(cvector*, int);
void cvector_delete(cvector*, int);
void cvector_free(cvector*);

#endif
