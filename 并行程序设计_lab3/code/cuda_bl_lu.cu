#include <iostream>
#include <cmath>
#include <cuda_runtime.h>
#include <iomanip>
#define BLOCK_SIZE 16
#include "deal_binary.h"
using std::string;

// CUDA核函数，计算以每个元素为中心的窗口中的熵
__global__ void calculateEntropy(int* input, float* output, int row,int col)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    //printf("hhh\n");
    double logTable[26];
    for (int k = 1; k <= 25; k++)
    {
        logTable[k] = log2f(k);
        //printf("%f\n",logTable[k-1]);
    }
    //printf("kkk\n");
    if (index < row * col)
    {
        int r = index / col;
        int c = index % col;

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
            //printf("%d \n",frequency[k]);
            if (prob > 0.0f){
                if(frequency[k] >= 1 && frequency[k] <= 25)
                entropy -= prob * (logTable[frequency[k]]-logTable[windowElements]);
                else 
                entropy -= prob * log2f(prob);
            }
        }

        output[index] = entropy;
    }
}

int main()
{
    int sizes[] = { 5, 16, 128, 1024, 2048, 3000, 4000 };
    srand(static_cast<unsigned int>(1234));
    /*
    for (int i = 0; i < 7; i++)
    {
        int size = sizes[i];
        
        // 随机生成二维数组
        int* hostInput = new int[size * size];
        for (int j = 0; j < size * size; j++)
            hostInput[j] = rand() % 16;

        int* deviceInput;
        cudaMalloc((void**)&deviceInput, size * size * sizeof(int));
        cudaMemcpy(deviceInput, hostInput, size * size * sizeof(int), cudaMemcpyHostToDevice);

        float* hostOutput = new float[size * size];
        float* deviceOutput;
        cudaMalloc((void**)&deviceOutput, size * size * sizeof(float));

        // 定义CUDA的网格和块大小
        int gridSize = (size * size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        int blockSize = BLOCK_SIZE;

        // 预计算对数表
        float logTable[25];
        for (int k = 0; k < 25; k++)
        {
            logTable[k] = log2f(static_cast<float>(k + 1));
            //printf("%f ",logTable[k]);
        }
        //printf("\n");

        clock_t start, finish;
        start = clock();
        // 调用CUDA核函数
        calculateEntropy<<<gridSize, blockSize>>>(deviceInput, deviceOutput, size, logTable);

        cudaMemcpy(hostOutput, deviceOutput, size * size * sizeof(float), cudaMemcpyDeviceToHost);
        finish = clock();
        // 输出结果
        std::cout << "Array Size: " << size << " using time: " << 1000 * double(finish - start) / CLOCKS_PER_SEC << " ms" << std::endl;

        // 释放内存
        delete[] hostInput;
        delete[] hostOutput;
        cudaFree(deviceInput);
        cudaFree(deviceOutput);
    }
    */
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
    int gridSize = (row * col + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int blockSize = BLOCK_SIZE;

    // 预计算对数表
    double* logTable=new double[26];
    for (int k = 1; k <= 25; k++)
    {
        logTable[k] = log2(double(k));
        //printf("%f\n",logTable[k-1]);
    }


    calculateEntropy<<<gridSize, blockSize>>>(deviceInput, deviceOutput, row,col);
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

    write(array,entropyArray,"output/cuda_bl_lu.in","output/cuda_bl_lu.out");
    

    // 释放内存
    delete[] hostInput;
    delete[] hostOutput;
    cudaFree(deviceInput);
    cudaFree(deviceOutput);


    return 0;
}
