#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

void cvector_init(cvector *v)
{
    v->data = NULL;
    v->size = 0;
    v->count = 0;
}

int cvector_count(cvector *v)
{
    return v->count;
}

void cvector_add(cvector *v, double e)
{
    if (v->size == 0) {
        v->size = 10;
        v->data = malloc(sizeof(double) * v->size);
        memset(v->data, '\0', sizeof(double) * v->size);
    }

    if (v->size == v->count) {
        v->size *= 2;
        v->data = realloc(v->data, sizeof(double) * v->size);
    }

    v->data[v->count] = e;
    v->count++;
}

void cvector_set(cvector *v, int index, double e)
{
    if (index >= v->count) {
        return;
    }

    v->data[index] = e;
}

double cvector_get(cvector *v, int index)
{
    if (index >= v->count) {
        return 0.0;
    }

    return v->data[index];
}

void cvector_delete(cvector *v, int index)
{
    if (index >= v->count) {
        return;
    }

    for (int i = index+1, j = index; i < v->count; i++) {
        v->data[j] = v->data[i];
        j++;
    }

    v->count--;
}

void cvector_free(cvector *v)
{
    free(v->data);
}
/*
int main(void)
{
    vector v;
    vector_init(&v);

    vector_add(&v, "emil");
    vector_add(&v, "hannes");
    vector_add(&v, "lydia");
    vector_add(&v, "olle");
    vector_add(&v, "erik");

    int i;
    printf("first round:\n");
    for (i = 0; i < vector_count(&v); i++) {
        printf("%s\n", (char*)vector_get(&v, i));
    }

    vector_delete(&v, 1);
    vector_delete(&v, 3);

    printf("second round:\n");
    for (i = 0; i < vector_count(&v); i++) {
        printf("%s\n", (char*)vector_get(&v, i));
    }

    vector_free(&v);

    return 0;
}
 */