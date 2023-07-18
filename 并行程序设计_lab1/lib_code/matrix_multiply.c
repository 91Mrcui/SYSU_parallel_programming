#include "matrix_multiply.h"
#include<stdio.h>
#include<stdlib.h>
void matrix_multiply(double**A,double**B,double**C,int M,int N,int K){
    for(int m=0;m<M;m++){
		for(int k=0;k<K;k++){
			for(int n=0;n<N;n++){
				C[m][k]+=A[m][n]*B[n][k];
			}
		}
	}
}