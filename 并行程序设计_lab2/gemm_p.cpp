#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<iostream>

using namespace std;

int M,N,K;

double **A;
double **B;
double **C;

int thread_count;

void init_Mat(int M,int N,int K){
	srand(243);
    A = new double* [M];
    B = new double* [N];
    C = new double* [M];
    for (int m=0;m<M;m++){
        A[m]=new double[N];
		for(int n=0;n<N;n++){
			A[m][n]=(double)(rand()%1000/10.0);
		}
	}
	for (int n=0;n<N;n++){
        B[n]=new double [K];
		for(int k=0;k<K;k++){
			B[n][k]=(double)(rand()%1000/10.0);
		}
	}
	for (int m=0;m<M;m++){
        C[m]=new double[K];
		for(int k=0;k<K;k++){
			C[m][k]=0;
		}
	}
} 

// print matrix
void print_mat(int row,int col,double ** matrix){
	for(int i=0;i<row;i++){
				for(int j=0;j<col;j++){
					printf("%.2f \t",matrix[i][j]);
				}
				cout<<endl;
			}
}

void *gemm(void *rank) 
{
    // 获取线程编号
    int p_rank = (long)rank;
    // 指定每个线程要计算的行数
    int p_first_row, p_end_row; 
    int quotient = M / thread_count;
    int remainder = M % thread_count;
    int p_cols=0;
    if (p_rank < remainder)
    {
        p_cols = quotient + 1;
        p_first_row = p_rank * p_cols;
    }
    else
    {
        p_cols = quotient;
        p_first_row = p_rank * p_cols + remainder;
    }
    p_end_row = p_first_row + p_cols;

    // 执行矩阵乘法
    for (int m = p_first_row; m < p_end_row; m++)
    {
        for (int k= 0; k < K; k++)
        {
            C[m][k] = 0;
            for (int n = 0; n < N; n++)
            {
                C[m][k] += A[m][n] * B[n][k];
            }
        }
    }
    return NULL;
}

int main(int argc, char * argv[] ){
    // 矩阵 A、B 和 C 的维度以及线程数量
    M=atoi(argv[1]);
    N=atoi(argv[2]);
    K=atoi(argv[3]);
    thread_count = atoi(argv[4]);

    // 初始化
    init_Mat(M,N,K);

    cout<<"matrix A:"<<endl;
    print_mat(M,N,A);
    cout<<"matrix B:"<<endl;
    print_mat(N,K,B);

    pthread_t *thread_handles;
    // 创建一个线程数组，大小为线程数量
    thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t)); 

    clock_t start_time=clock();
    for (int t = 0; t < thread_count; t++)          
    {
        // 创建一个线程，并指定该线程要执行的函数为 gemm
        // 将线程编号 t 传递给 gemm 函数
        pthread_create(&thread_handles[t], NULL, gemm, (void *)t);
    }
    // 等待所有线程执行完成
    for (int t = 0; t < thread_count; t++) 
    {
        pthread_join(thread_handles[t], NULL);
    }
    
    clock_t end_time=clock();
    double using_time=(double)(end_time-start_time)/CLOCKS_PER_SEC;

    cout<<"result:"<<endl;
    print_mat(M,K,C);
    cout<<"uisng time:"<<using_time<<" s"<<endl;

    free(thread_handles);

    return 0;
}



