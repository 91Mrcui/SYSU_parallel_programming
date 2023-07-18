//p2p

#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

// print matrix
void print_mat(int row,int col,double * matrix){
	for(int i=0;i<row;i++){
				for(int j=0;j<col;j++){
					printf("%.2f \t",matrix[i*row+j]);
				}
				cout<<endl;
			}
}

//init matrix
void init_Mat(int row, int col,double* mat){
	for (int m=0;m<row;m++){
		for (int n=0;n<col;n++){
			mat[m*col+n]=(double)(rand()%1000/10.0);
		}
	}
}

int main(int argc, char * argv[] ){
    int M=atoi(argv[1]);
	int N=atoi(argv[2]);
	int K=atoi(argv[3]);
    double *b= new double [ N* K ];
	double *result = new double [ M * K ];
	double *a=NULL,*c=NULL;
	int pid, process_num, line;

    MPI_Init(NULL,NULL);//Initialize
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);//process id
    MPI_Comm_size(MPI_COMM_WORLD,&process_num);//num of process

	line = M/process_num;//divide data
	srand(233);

	//main process
	if(pid==0){
			a=new double[M*N];
			c=new double[M*K];
			init_Mat(M,N,a);
			init_Mat(N,K,b);
			cout<<"Matrix A:"<<endl;
			print_mat(M,N,a);
			cout<<"Matrix B:"<<endl;
			print_mat(N,K,b);

			double start_time;
			double end_time;
			start_time=MPI_Wtime();
			//send matrix N to sub processes
			for (int i=1;i<process_num;i++){
				MPI_Send(b,N*K,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			}
			// send each row of A to sub processes
			for (int i=1;i<process_num;i++){
				MPI_Send(a+(i-1)*line*N,N*line,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
			}
			// receive result
			for (int i=1;i<process_num;i++){
				MPI_Recv(result,line*K,MPI_DOUBLE,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				for(int l=0;l<line;l++){
					for(int k=0;k<K;k++){
						c[((i-1)*line+l)*K+k]=result[l*K+k];
					}
				}
			}

			for (int i=(process_num-1)*line;i<M;i++){
            	for (int j=0;j<K;j++){
                	double tmp=0;
                	for (int k=0;k<N;k++)
                    	tmp += a[i*N+k]*b[k*K+j];
                		c[i*K+j] = tmp;
            	}
        	}
			end_time=MPI_Wtime();
			double using_time=end_time-start_time;
			cout<<"Matrix C:"<<endl;
			print_mat(M,K,c);
			cout<<"using time:"<<using_time<<endl;
	}

	//sub process
	else{
		double* temp = new double [ N * line ];
		MPI_Recv(b,N*K,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        MPI_Recv(temp,N*line,MPI_DOUBLE,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		for(int i=0;i<line;i++){
			for(int j=0;j<N;j++){
				double tmp=0;
				for(int k=0;k<N;k++)
					tmp += temp[i*N+k]*b[k*K+j];
				result[i*K+j] = tmp;
			}
		}
		MPI_Send(result, line*K, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}