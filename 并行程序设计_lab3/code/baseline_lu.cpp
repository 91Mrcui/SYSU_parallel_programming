#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "deal_binary.h"
using std::string;


const int ARRAY_SIZE[] = {5, 16, 128, 1024, 2048, 3000,4000};
const int ELEMENT_RANGE = 16;
const int WINDOW_SIZE = 5;
const int NUM_ARRAYS = sizeof(ARRAY_SIZE) / sizeof(ARRAY_SIZE[0]);
const int KERNEL_RADIUS = WINDOW_SIZE / 2;

const int LOG_TABLE_SIZE = 25;
std::vector<double> logTable(LOG_TABLE_SIZE);

// 初始化对数表
void initializeLogTable() {
    for (int i = 1; i <= LOG_TABLE_SIZE; i++) {
        logTable[i - 1] = log2(i);
    }
}

// 查找对数值
double lookupLog(int n) {
    if (n >= 1 && n <= LOG_TABLE_SIZE) {
        return logTable[n - 1];
    } else {
        // 处理超出查表范围的情况
        return log2(n);
    }
}

// 生成随机二维数组
void generateRandomArray(std::vector<std::vector<int> >& array,int row,int col) {
    array.resize(row);
    for(int i=0;i<row;i++){
        array[i].resize(col);
    }
	srand(static_cast<unsigned int>(2234));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            array[i][j] = rand() % ELEMENT_RANGE;
        }
        //printf("\n");
    }
}

// 计算熵
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
            entropy -= probability * (lookupLog(counts[i])-lookupLog(windowSize));
        }
    }
	//printf("\n");
    return entropy;
}

int main() {
    // 设置随机种子
    srand(static_cast<unsigned int>(time(NULL)));

    // 初始化对数表
    initializeLogTable();

    /*
    for (int i = 0; i < NUM_ARRAYS; i++) {
        std::vector<std::vector<int> > array;
        generateRandomArray(array, ARRAY_SIZE[i]);

        std::vector<std::vector<double> > entropyArray(ARRAY_SIZE[i], std::vector<double>(ARRAY_SIZE[i]));

        clock_t start, finish;
        start = clock();

        for (int x = 0; x < ARRAY_SIZE[i]; x++) {
            for (int y = 0; y < ARRAY_SIZE[i]; y++) {
                entropyArray[x][y] = calculateEntropy(array, x, y);
            }
        }

        finish = clock();
        std::cout << "Array Size: " << ARRAY_SIZE[i] << " using time: " << 1000 * double(finish - start) / CLOCKS_PER_SEC << " ms" << std::endl;
        
    }*/
    std::vector<std::vector<int> > array;
    std::vector<float> res;
    read(array,res,"test/test.in","test/test.out");
    int row=array.size();
    int col=array[0].size();
    generateRandomArray(array,row,col);
    std::vector<std::vector<float> > entropyArray;
    entropyArray.resize(row);
    for(int i=0;i<row;i++){
        entropyArray[i].resize(col);
    }
    for (int x = 0; x < row; x++) {
        for (int y = 0; y < col; y++) {
            entropyArray[x][y] = calculateEntropy(array, x, y);
        }
    }
    write(array,entropyArray,"output/baseline_lu.in","output/baseline_lu.out");
    return 0;
}
