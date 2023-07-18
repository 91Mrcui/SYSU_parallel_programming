#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H
#include <pthread.h>

// for 循环参数的结构体
struct for_index{
    void *args;
    int start; // 起始下标
    int end; //终止下标
    int increment; // 步长
};
void parallel_for(int start, int end, int increment, void *(*functor)(void *), void *arg, int num_threads);

#endif
