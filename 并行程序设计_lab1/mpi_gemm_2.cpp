//collective communication

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
    double* a = new double [M*N];
	double* b = new double [N*K];
	double* c = new double [M*K];
    int pid, process_num, line;

	MPI_Init(NULL,NULL);//Initialize
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);//process id
    MPI_Comm_size(MPI_COMM_WORLD,&process_num);//num of process
	
	line = M/process_num;//divide data
	srand(233);
	double * local_matrix = new double [line*N];
	double * result = new double [M*K];

    //main process
	if(pid==0){
        init_Mat(M,N,a);
        init_Mat(N,K,b);
        cout<<"Matrix A:"<<endl;
		print_mat(M,N,a);
		cout<<"Matrix B:"<<endl;
		print_mat(N,K,b);


        double start_time;
		double end_time;
		start_time=MPI_Wtime();
        //send divition of a to sub process
        MPI_Scatter(a, line*N, MPI_DOUBLE, local_matrix, line*N, MPI_DOUBLE, 0, MPI_COMM_WORLD );
        //broadcast b to every process
		MPI_Bcast(b, N*K, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        //calculate local results
        for(int i= 0; i< M;i++){
			for(int j=0;j<N;j++){
				double tmp = 0;
				for(int k=0;k<N;k++)
					tmp += a[i*N+k] * b[k*K+ j];
				result[i*K+ j ] = tmp;
			}
		}
		//wait all
		MPI_Barrier() ;
        //Collect data into a process
        MPI_Gather( result, line*K, MPI_DOUBLE, c, line*K, MPI_DOUBLE, 0, MPI_COMM_WORLD );
        //calculate the remain
		for(int i = (process_num-1)*line;i<M;i++){
			for(int j=0;j<N;j++){
				double tmp = 0;
				for(int k=0;k<N;k++)
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

    else{
        double * temp= new double [ N * line ];
		MPI_Scatter(a, line*N, MPI_DOUBLE, temp, line*N, MPI_DOUBLE, 0, MPI_COMM_WORLD );
		MPI_Bcast( b, N* K, MPI_DOUBLE, 0, MPI_COMM_WORLD );
		for(int i=0;i<line;i++){
			for(int j=0;j<N;j++){
				double tmp=0;
				for(int k=0;k<N;k++)
					tmp += temp[i*N+k]*b[k*K+j];
				result[i*K+j] = tmp;
			}
		}
		MPI_Gather(result, line*K, MPI_DOUBLE, c, line*K, MPI_DOUBLE, 0, MPI_COMM_WORLD );
    }
    MPI_Finalize();
    return 0;    
}