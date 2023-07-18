#include <iostream>
#include <cuda.h>
#include <vector>
#include "read_data.h"

__global__ void matrixVectorMul(float* A, float* b, float* c, int rows, int cols) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < rows) {
        float sum = 0.0f;
        for (int j = 0; j < cols; j++) {
            sum += A[tid * cols + j] * b[j];
        }
        c[tid] = sum;
    }
}

void matrixVectorMultiplication(std::vector<float>& A, std::vector<float>& b, std::vector<float>& c, int rows, int cols) {
    // Device memory allocation
    float *d_A, *d_b, *d_c;
    cudaMalloc((void**)&d_A, rows * cols * sizeof(float));
    cudaMalloc((void**)&d_b, cols * sizeof(float));
    cudaMalloc((void**)&d_c, rows * sizeof(float));

    // Copy data from host to device
    cudaMemcpy(d_A, A.data(), rows * cols * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b.data(), cols * sizeof(float), cudaMemcpyHostToDevice);

    // Launch kernel
    int blockSize = 256;
    int gridSize = (rows + blockSize - 1) / blockSize;
    matrixVectorMul<<<gridSize, blockSize>>>(d_A, d_b, d_c, rows, cols);

    // Copy result from device to host
    cudaMemcpy(c.data(), d_c, rows * sizeof(float), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_b);
    cudaFree(d_c);
}

int main() {
    //修改这里的路径，读入二进制文件和输出二进制文件
    string read_dir="data/test5.in";
    string save_dir="output/res5.out";
    std::vector<float> A ;
    std::vector<std::vector<float> >array_2d; 
    std::vector<float> b ;

    read(A,array_2d,b,read_dir);
    //generate_data(A,array_2d,b,4096);

    int rows = array_2d.size();
    int cols = array_2d[0].size();

    std::vector<float> c(rows);

    clock_t start, finish;
    start = clock();
    matrixVectorMultiplication(A, b, c, rows, cols);
    finish=clock();

    // Print result
    std::cout << "Result: ";
    for (int i = 0; i < rows; i++) {
        printf("%.5f ",c[i]);
    }
    std::cout << std::endl;
    // Print using time
    std::cout <<"using time: "<<1000*double(finish - start) / CLOCKS_PER_SEC<<" ms"<<std::endl;

    write(c,save_dir);
    return 0;
}
