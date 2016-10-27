#ifndef cdeque_H
#define cdeque_H

#include "dynarray.h"

typedef struct {
    dynarray *arrays;
    unsigned int arraysize;
    unsigned int front;
    unsigned int back;
    unsigned int firstempty;
    unsigned int lastempty;
    unsigned int count;
} cdeque;

typedef void (*cdeque_forfn)(void*);

cdeque * cdeque_create(void);
void cdeque_delete(cdeque * queue);
void cdeque_push_front(cdeque * queue, void * data);
void cdeque_push_back(cdeque * queue, void * data);
void * cdeque_pop_front(cdeque * queue);
void * cdeque_pop_back(cdeque * queue);
void * cdeque_get_at(const cdeque *queue, unsigned int index);
void * cdeque_set_at(cdeque *queue, unsigned int index, void * data);
void * cdeque_peek_front(const cdeque * queue);
void * cdeque_peek_back(const cdeque * queue);
void cdeque_for_each(const cdeque * queue, cdeque_forfn fun);

#endif /* cdeque_H */