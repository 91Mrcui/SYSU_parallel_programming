#include <iostream>
#include <cmath>
#include <cuda_runtime.h>
#include <iomanip>
#define BLOCK_SIZE 16
#include "deal_binary.h"
using std::string;

// CUDA核函数，计算以每个元素为中心的窗口中的熵
// 计算每个元素的熵的核函数（使用共享内存优化）
__global__ void calculateEntropy_share(int* input, float* output, int width, int height) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    int col = blockIdx.y * blockDim.y + threadIdx.y;
    // 定义共享内存
    __shared__ int shared_input[8 + 4][8 + 4];
    // 计算线程在共享内存中的索引
    int shared_row = threadIdx.x + 2;
    int shared_col = threadIdx.y + 2;
    // 将数据从全局内存复制到共享内存
    if (row < height && col < width) {
        int global_index = col * height + row;
        shared_input[shared_row][shared_col] = input[global_index];
    }
    // 线程同步，确保数据复制完成
    __syncthreads();
    if (row < height && col < width) {
        float entropy = 0;
        // 记录每个数字出现过的次数
        int record[16];
        // 窗口内元素总数
        int count = 0, x, y;
        for (int i = 0; i < 16; i++) {
            record[i] = 0;
        }
        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                x = shared_col + i;
                y = shared_row + j;
                printf("%d %d\n",x,y);
                int value = shared_input[y][x];
                record[value]++;
                count++;
            }
        }
        // 计算熵值
        for (int i = 0; i < 16; i++) {
            //entropy -= (float)record[i] * (log_table[record[i]]-log_table[count]) / count;
            float prob = (float)(record[i]) / count;
            //printf("%f\n",prob);
            if (prob > 0.0f)
                entropy -= prob * log2f(prob);
        }
        output[col * height + row] = entropy;
    }
}

// CUDA核函数，计算以每个元素为中心的窗口中的熵
__global__ void calculateEntropy(int* input, float* output, int row,int col)
{
    int c = blockIdx.y * blockDim.y + threadIdx.y;
    int r = blockIdx.x * blockDim.x + threadIdx.x;

    if (r < row && c < col)
    {
        int windowSize = 5;
        int windowStartRow = r - 2;
        int windowStartCol = c - 2;
        int windowEndRow = windowStartRow + 4;
        int windowEndCol = windowStartCol + 4;

       // 边界处理
        if (windowStartRow < 0)
            windowStartRow = 0;
        if (windowStartCol < 0)
            windowStartCol = 0;
        if (windowEndRow >= row)
            windowEndRow = row - 1;
        if (windowEndCol >= col)
            windowEndCol = col - 1;

        float entropy = 0.0f;
        int windowElements = (windowEndRow - windowStartRow + 1) * (windowEndCol - windowStartCol + 1);

        // 计算窗口内元素的频率
        int frequency[16] = { 0 };
        for (int i = windowStartRow; i <= windowEndRow; i++)
        {
            for (int j = windowStartCol; j <= windowEndCol; j++)
            {
                int value = input[i * col + j];
                frequency[value]++;
            }
        }

        // 计算熵
        for (int k = 0; k < 16; k++)
        {
            float prob = static_cast<float>(frequency[k]) / windowElements;
            if (prob > 0.0f)
                entropy -= prob * log2f(prob);
        }

        output[r * col + c] = entropy;
    }
}

int main()
{
    int sizes[] = { 5, 16, 128, 1024, 2048, 3000 ,4000};
    srand(static_cast<unsigned int>(1234));
    std::vector<std::vector<int> > array;
    std::vector<float> res;
    read(array,res,"test/test.in","test/test.out");
    int row=array.size();
    int col=array[0].size();


    int* hostInput = new int[row*col];
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            hostInput[i*col+j]=array[i][j];
        }
    }

    int* deviceInput;
    cudaMalloc((void**)&deviceInput, row * col * sizeof(int));
    cudaMemcpy(deviceInput, hostInput, row * col * sizeof(int), cudaMemcpyHostToDevice);

    float* hostOutput = new float[row * col];
    float* deviceOutput;
    cudaMalloc((void**)&deviceOutput, row * col * sizeof(float));
    // 定义CUDA的网格和块大小
    // 设置grid、block
    dim3 block_size(8, 8);
    dim3 grid_size((col + block_size.x - 1) / block_size.x, (row + block_size.y - 1) / block_size.y);
    dim3 gridSize((row + BLOCK_SIZE - 1) / BLOCK_SIZE, (col + BLOCK_SIZE - 1) / BLOCK_SIZE);
    dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE);
    calculateEntropy<<<gridSize, blockSize>>>(deviceInput, deviceOutput, row,col);
    //calculateEntropy<< <grid_size, block_size >> >(deviceInput, deviceOutput, row,col);
    cudaMemcpy(hostOutput, deviceOutput, row* col * sizeof(float), cudaMemcpyDeviceToHost);

    std::vector<std::vector<float> > entropyArray;
    entropyArray.resize(row);
    for(int i=0;i<row;i++){
        entropyArray[i].resize(col);
    }
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            array[i][j]=hostInput[i*col+j];
            entropyArray[i][j]=hostOutput[i*col+j];
        }
    }

    write(array,entropyArray,"output/share_mem.in","output/share_mem.out");
    
    // 释放内存
    delete[] hostInput;
    delete[] hostOutput;
    cudaFree(deviceInput);
    cudaFree(deviceOutput);


    return 0;
}
