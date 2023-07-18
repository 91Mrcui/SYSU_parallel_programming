#include <iostream>
#include <pthread.h>
#include <random>

#define THREAD_COUNT 8  // 线程数量
#define ITERATIONS 1000000  // 迭代次数

double sum = 0;  
pthread_mutex_t lock;  //互斥锁

void *calculate_area(void *thread_id_ptr) {
    int thread_id = *(int *) thread_id_ptr;  // 获取线程ID
    std::random_device rd; 
    std::mt19937 gen(rd());  
    std::uniform_real_distribution<> dis(0, 1);  
    double local_sum = 0;  

    for (int i = 0; i < ITERATIONS; i++) { 
        double x = dis(gen);  
        double y = dis(gen);  
         //  (x, y) 在 y=x^2 曲线下方，需要计入面积
        if (y <= x * x) { 
            local_sum++;
        }
    }

    //将当前线程的计算结果加到 sum 变量中
    pthread_mutex_lock(&lock); 
    sum += local_sum / ITERATIONS;  
    pthread_mutex_unlock(&lock); 

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];  // 创建线程数组
    int thread_ids[THREAD_COUNT];  // 创建线程 ID 数组
    pthread_mutex_init(&lock, NULL);  // 初始化互斥锁

      // 创建线程并执行计算
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_ids[i] = i;  
        pthread_create(&threads[i], NULL, calculate_area, &thread_ids[i]);  
    }

    // 等待所有线程完成计算
    for (int i = 0; i < THREAD_COUNT; i++) {  
        pthread_join(threads[i], NULL);  
    }

    pthread_mutex_destroy(&lock);  
    std::cout << "function: y=x^2\nEstimated area: " << sum / THREAD_COUNT << std::endl;  // 输出计算结果
    return 0;
}
