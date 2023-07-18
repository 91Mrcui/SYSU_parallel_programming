//v4版本，用了cublas进行优化
#include <iostream>
#include <cuda.h>
#include <vector>
#include <cublas_v2.h>
#include "read_data.h"

void matrixVectorMultiplication(std::vector<float>& A, std::vector<float>& b, std::vector<float>& c, int rows, int cols) {
    // Device memory allocation
    float *d_A, *d_b, *d_c;
    cudaMalloc((void**)&d_A, rows * cols * sizeof(float));
    cudaMalloc((void**)&d_b, cols * sizeof(float));
    cudaMalloc((void**)&d_c, rows * sizeof(float));

    // Copy data from host to device
    cudaMemcpy(d_A, A.data(), rows * cols * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b.data(), cols * sizeof(float), cudaMemcpyHostToDevice);

    // cuBLAS initialization
    cublasHandle_t handle;
    cublasCreate(&handle);

    // Matrix-vector multiplication using cuBLAS
    float alpha = 1.0f;
    float beta = 0.0f;
    cublasSgemv(handle, CUBLAS_OP_T, cols, rows, &alpha, d_A, cols, d_b, 1, &beta, d_c, 1);

    // Copy result from device to host
    cudaMemcpy(c.data(), d_c, rows * sizeof(float), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_b);
    cudaFree(d_c);

    // Destroy cuBLAS handle
    cublasDestroy(handle);
}

int main() {
    string read_dir="data/test1.in";
    string save_dir="output/res1.out";
    std::vector<float> A;
    std::vector<std::vector<float>> array_2d;
    std::vector<float> b;

    read(A, array_2d, b, read_dir);

    int rows = array_2d.size();
    int cols = array_2d[0].size();

    std::vector<float> c(rows);

    clock_t start, finish;
    start = clock();
    matrixVectorMultiplication(A, b, c, rows, cols);
    finish = clock();

    // Print result
    std::cout << "Result: ";
    for (int i = 0; i < rows; i++) {
        printf("%.5f ", c[i]);
    }
    std::cout << std::endl;

    // Print execution time
    std::cout << "Using time: " << 1000 * double(finish - start) / CLOCKS_PER_SEC << " ms" << std::endl;
    write(c,save_dir);
    return 0;
}
