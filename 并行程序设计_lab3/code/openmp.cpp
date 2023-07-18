#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <iomanip>
#include "deal_binary.h"
using std::string;
#define ELEMENT_RANGE 16

const int WINDOW_SIZE = 5;
// 生成随机二维数组
void generateRandomArray(std::vector<std::vector<int> >& array, int size) {
	srand(static_cast<unsigned int>(1234));
    array.resize(size, std::vector<int>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            array[i][j] = rand() % ELEMENT_RANGE;
        }
        //printf("\n");
    }
}


// 计算窗口中的熵
double calculateEntropy(const std::vector<std::vector<int> >& array, int x, int y) {
    std::vector<int> counts(ELEMENT_RANGE, 0);

    int startX = std::max(0, x - WINDOW_SIZE / 2);
    int startY = std::max(0, y - WINDOW_SIZE / 2);
    int endX = std::min(static_cast<int>(array.size()) - 1, x + WINDOW_SIZE / 2);
    int endY = std::min(static_cast<int>(array[0].size()) - 1, y + WINDOW_SIZE / 2);

	//printf("(%d %d),(%d %d)\n",startX,startY,endX,endY);

    for (int i = startX; i <= endX; i++) {
        for (int j = startY; j <= endY; j++) {
            counts[array[i][j]]++;
        }
    }

    float entropy = 0.0;
    int windowSize = (endX - startX + 1) * (endY - startY + 1);
    //printf("%d\n",windowSize);
    for (int i = 0; i < ELEMENT_RANGE; i++) {
        float probability = float(counts[i]) / windowSize;
        if (counts[i]!=0) {
        	//printf("%d ",i);
            entropy -= probability * log2(probability);
        }
    }
	//printf("\n");
    return entropy;
}

int main() {
    // 设置随机种子
    std::srand(static_cast<unsigned int>(1234));
    /*
    // 定义数组大小
    int sizesArr[] = {5, 16, 128, 1024, 2048, 3000,4000};
    std::vector<int> sizes;
    for(int i=0;i<7;i++)
        sizes.push_back(sizesArr[i]);
    // 并行计算熵
    #pragma omp parallel for num_threads(40)
    //#pragma omp parallel for collapse(24)
    for (int i=0;i<sizes.size();i++) {
        // 生成随机二维数组
        int size=sizes[i];
        std::vector<std::vector<int> > array(size, std::vector<int>(size));
        generateRandomArray(array,size);
        

        clock_t start, finish;
    	start = clock();
        // 计算熵
        std::vector<std::vector<double> > entropyArray(size, std::vector<double>(size));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                entropyArray[i][j] = calculateEntropy(array, i, j);
            }
        }
        finish=clock();
        // 输出结果
        std::cout << "Array Size: " << size << " using time: "<<1000*double(finish - start) / CLOCKS_PER_SEC<<" ms"<<std::endl;

        
    }
    */
    std::vector<std::vector<int> > array;
    std::vector<float> res;
    read(array,res,"test/test.in","test/test.out");
    int row=array.size();
    int col=array[0].size();
    std::vector<std::vector<float> > entropyArray;
    entropyArray.resize(row);
    for(int i=0;i<row;i++){
        entropyArray[i].resize(col);
    }
    #pragma omp parallel for num_threads(20)
    for (int x = 0; x < row; x++) {
        for (int y = 0; y < col; y++) {
            entropyArray[x][y] = calculateEntropy(array, x, y);
        }
    }
    write(array,entropyArray,"output/openmp.in","output/openmp.out");

    return 0;
}
