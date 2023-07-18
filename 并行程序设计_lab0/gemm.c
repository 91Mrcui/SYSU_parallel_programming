#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define NUM 100

int M,N,K;

double A[2048][2048];
double B[2048][2048];
double C[2048][2048];



void init_Mat(int M,int N,int K){

	srand(233);

    for (int m=0;m<M;m++){
		for(int n=0;n<N;n++){
			A[m][n]=(double)(rand()%1000/10.0);
			printf("%f",A[m][n]);
		}
	}

	for (int n=0;n<N;n++){
		for(int k=0;k<K;k++){
			B[n][k]=(double)(rand()%1000/10.0);
		}
	}

	for (int m=0;m<M;m++){
		for(int k=0;k<K;k++){
			C[m][k]=0;
		}
	}
} 


int main(){
    clock_t start_time,end_time;
    printf("input three integer(512 ~2048):\n");
    scanf("%d %d %d",&M,&N,&K);

	init_Mat(M,N,K);

    start_time=clock();
    for(int m=0;m<M;m++){
		for(int k=0;k<K;k++){
			for(int n=0;n<N;n++){
				C[m][k]+=A[m][n]*B[n][k];
			}
		}
	}
    end_time=clock();

	printf("\n");
	printf("matrix_1:\n");
	for (int i = 0; i<M; i++){
		for(int j=0;j<N;j++){
			printf("%0.2f ",A[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	printf("matrix_2:\n");
	for (int i = 0; i<N; i++){
		for(int j=0;j<K;j++){
			printf("%0.2f ",B[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	printf("result:\n");
	for (int i = 0; i<M; i++){
		for(int j=0;j<K;j++){
			printf("%0.2f ",C[i][j]);
		}
		printf("\n");
	}
	
	printf("\n");
	printf("using time: %f  ms\n",(double)(end_time-start_time)*1000.0/CLOCKS_PER_SEC);

    return 0;
}