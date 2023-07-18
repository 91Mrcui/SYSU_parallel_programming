#include <pthread.h>

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"parallel_for.h"

using namespace std;

int M,N,K;
int Thread_NUM =4;
double *A;
double *B;
double *C;

//print matrix
void print_mat(int row,int col,double * matrix){
	for(int i=0;i<row;i++){
				for(int j=0;j<col;j++){
					printf("%.2f \t",matrix[i*row+j]);
				}
				cout<<endl;
			}
}

//init matrix
void init_Mat(int M,int N,int K){
	srand(243);
    A = new double [M*N];
    B = new double [N*K];
    C = new double [M*K];
    for (int m=0;m<M;m++){
		for(int n=0;n<N;n++){
			A[m*M+n]=(double)(rand()%1000/10.0);
		}
	}
	for (int n=0;n<N;n++){
		for(int k=0;k<K;k++){
			B[n*N+k]=(double)(rand()%1000/10.0);
		}
	}
	for (int m=0;m<M;m++){
		for(int k=0;k<K;k++){
			C[m*M+k]=0;
		}
	}
} 

//函数参数的结构体
struct args{
    double*A;
    double *B;
    double *C;
    int m;
    int n;
    int k;
};


void *gemm_fun(void *args){
    struct for_index *idx = (struct for_index *)args;
    struct args *matrix = (struct args *)(idx->args);

    int K=matrix->k;
    int N=matrix->n;

    for (int m = idx->start; m < idx->end; m = m + idx->increment){
        for (int k = 0; k < K; k++){
            matrix->C[m * K + k] =0;
            for (int n = 0; n < N; n++){
                matrix->C[m * K + k]  += matrix->A[m * N + n] * matrix->B[n* K + k];
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int M=atoi(argv[1]);
	int N=atoi(argv[2]);
	int K=atoi(argv[3]);
    Thread_NUM=atoi(argv[4]);

    init_Mat(M,N,K);

    struct args *arg = new args();
    arg->A=A;
    arg->B=B;
    arg->C=C;
    arg->n=N;
    arg->m=M;
    arg->k=K;

    clock_t start_time=clock();
    parallel_for(0, M, 1, gemm_fun, arg, Thread_NUM);
    clock_t end_time=clock();
    double using_time=(double)(end_time-start_time)/CLOCKS_PER_SEC;

    cout<<"result:"<<endl;
    print_mat(M,K,C);
    cout<<"uisng time:"<<using_time<<" s"<<endl;
    return 0;
}