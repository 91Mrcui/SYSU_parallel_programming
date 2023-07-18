#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include <omp.h>
#include <random>



int Thread_NUM =4;

using namespace std;

int M,N,K;

double **A;
double **B;
double **C;

// init matrix
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

//use openmp
void parallel_gemm(){
    //默认调度
    //#pragma omp parallel for num_threads(Thread_NUM)
    //静态调度
    #pragma omp parallel for num_threads(Thread_NUM)\
    schedule(static, 1)
    //动态调度
    //#pragma omp parallel for num_threads(Thread_NUM)\
    //schedule(dynamic, 1)

    for(int m=0;m<M;m++){
        for(int n=0;n<N;n++){
            for(int k=0;k<K;k++){
                C[m][k]+=A[m][n]*B[n][k];
            }
        }
    }
}

//serial gemm
void serial_gemm(){
    for(int m=0;m<M;m++){
        for(int n=0;n<N;n++){
            for(int k=0;k<K;k++){
                C[m][k]+=A[m][n]*B[n][k];
            }
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



int main(int argc, char * argv[] ){
    // 矩阵 A、B 和 C 的维度以及线程数量
    M=atoi(argv[1]);
    N=atoi(argv[2]);
    K=atoi(argv[3]);
    Thread_NUM=atoi(argv[4]);

    clock_t start_time1,end_time1,start_time2,end_time2;

    init_Mat(M,N,K);
    start_time1=clock();
    serial_gemm();
    end_time1=clock();
    double using_time1=(double)(end_time1-start_time1)/CLOCKS_PER_SEC;

    init_Mat(M,N,K);
    start_time2=clock();
    parallel_gemm();
    end_time2=clock();
    double using_time2=(double)(end_time2-start_time2)/CLOCKS_PER_SEC;
    
    cout<<"result:"<<endl;
    print_mat(M,K,C);
    cout<<"normal gemm uisng time:"<<using_time1<<" s"<<endl;
    cout<<"openmp gemm uisng time:"<<using_time2<<" s"<<endl;
    return 0;
 }