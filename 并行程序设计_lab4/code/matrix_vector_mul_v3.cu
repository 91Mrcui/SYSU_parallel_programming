//v3版本，用了纹理内存进行优化，将内核函数中的内存访问从全局内存改为从纹理内存中读取数据

#include <iostream>
#include <cuda.h>
#include <vector>
#include "read_data.h"

texture<float, 1, cudaReadModeElementType> texA;
texture<float, 1, cudaReadModeElementType> texB;

__global__ void matrixVectorMul(float* c, int rows, int cols) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < rows) {
        float sum = 0.0f;
        for (int j = 0; j < cols; j++) {
            sum += tex1Dfetch(texA, tid * cols + j) * tex1Dfetch(texB, j);
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

    // Bind texture memory
    cudaBindTexture(NULL, texA, d_A, rows * cols * sizeof(float));
    cudaBindTexture(NULL, texB, d_b, cols * sizeof(float));

    // Launch kernel
    int blockSize = 256;
    int gridSize = (rows + blockSize - 1) / blockSize;
    matrixVectorMul<<<gridSize, blockSize>>>(d_c, rows, cols);

    // Copy result from device to host
    cudaMemcpy(c.data(), d_c, rows * sizeof(float), cudaMemcpyDeviceToHost);

    // Unbind texture memory
    cudaUnbindTexture(texA);
    cudaUnbindTexture(texB);

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_b);
    cudaFree(d_c);
}

int main() {
    string read_dir="data/test1.in";
    string save_dir="output/res1.out";
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
        //std::cout << c[i] << " ";
        printf("%.5f ",c[i]);
    }
    std::cout << std::endl;
    // Print using time
    std::cout <<"using time: "<<1000*double(finish - start) / CLOCKS_PER_SEC<<" ms"<<std::endl;
    write(c,save_dir);
    return 0;
}
