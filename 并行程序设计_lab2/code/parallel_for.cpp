#include"parallel_for.h"
#include<stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include<iostream>

//并行循环函数parallel_for
void parallel_for(int start, int end, int increment, void *(*functor)(void *), void *arg, int num_threads){
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    for_index *index_arr = (for_index *)malloc(num_threads * sizeof(for_index));

    //每个线程要处理的数据块大小
    int block = (end - start) / num_threads;
    //为每个线程分配参数
    for (int i = 0; i < num_threads; i++){
        index_arr[i].args = arg;
        index_arr[i].start = start + i * block;
        index_arr[i].end = index_arr[i].start + block;
        //处理最后一个线程的数据块
        if (i == (num_threads - 1))
            index_arr[i].end = end;
        index_arr[i].increment = increment;
        pthread_create(&threads[i], NULL, functor, (void *)(index_arr + i));
    }
    //等待所有线程执行完成
    for (int thread = 0; thread < num_threads; thread++)
        pthread_join(threads[thread], NULL);
    free(threads);
    free(index_arr);
}