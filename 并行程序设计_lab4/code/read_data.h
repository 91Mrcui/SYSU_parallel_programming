//头文件，定义了读取、写入二进制文件的函数

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
using std::string;

void read(std::vector<float>& array_1d,std::vector<std::vector<float> >& array_2d,std::vector<float>& res,string path){
    int row_size, col_size;
    std::vector<float> matrix_test;
    std::vector<float> vec;
    std::ifstream inFile_in(path, std::ios::binary);
    if (inFile_in.is_open()) {
        // First, read the size of the matrix and the vector.
        inFile_in.read(reinterpret_cast<char *>(&row_size),
            sizeof(row_size));
        inFile_in.read(reinterpret_cast<char *>(&col_size),
            sizeof(col_size));
        // Resize the matrix and vector based on the size read from the file.
        matrix_test.resize(row_size * col_size);
        vec.resize(col_size);
        // Then, read the actual data of the matrix and the vector.
        inFile_in.read(reinterpret_cast<char *>(matrix_test.data()),
            matrix_test.size() * sizeof(float));
        inFile_in.read(reinterpret_cast<char *>(vec.data()), vec.size()
            * sizeof(float));
        inFile_in.close();
    } 
    else {
        std::cout << "Unable to open file";
    }

    array_1d.resize(row_size * col_size);
    array_2d.resize(row_size);
    for(int i=0;i<row_size;i++){
        array_2d[i].resize(col_size);
    }
    
    for(int i=0;i<row_size;i++){
        for(int j=0;j<col_size;j++){
             array_2d[i][j]=matrix_test[i*col_size+j];
             array_1d[i*col_size+j]=matrix_test[i*col_size+j];
        }
    }

    res.resize(col_size);
    for(int i=0;i<col_size;i++){
        res[i]=vec[i];
    }

    printf("Read complete. Size:%d %d\n",row_size,col_size);
    //printf("##########################################################################################\n");
}

void read_res(std::vector<float> &result,string path,int size){
    std::ifstream inFile_in(path, std::ios::binary);
    if (inFile_in.is_open()) {
        // Resize the matrix and vector based on the size read from the file.
        result.resize(size);
        // Then, read the actual data of the matrix and the vector.
        inFile_in.read(reinterpret_cast<char *>(result.data()),
            result.size() * sizeof(float));
        inFile_in.close();
    } 
}

void write(std::vector<float> &result,string save_dir){
    // Write result to file
    std::ofstream outFile_out(save_dir, std::ios::binary);
    if (outFile_out.is_open()) {
        outFile_out.write(reinterpret_cast<const char *>
            (result.data()), result.size() * sizeof(float));
        outFile_out.close();
    } 
    else {
        std::cout << "Unable to open file";
    }
}

void generate_data(std::vector<float>& array_1d,std::vector<std::vector<float> >& array_2d,std::vector<float>& res,int size){
    srand(static_cast<unsigned int>(1234));
    array_1d.resize(size*size);
    array_2d.resize(size);
    res.resize(size);
    for(int i=0;i<size;i++){
        array_2d[i].resize(size);
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            array_2d[i][j] = float(rand()%10000)/10000;
            array_1d[i*size+j]=array_2d[i][j];
            //printf("%f ",array_2d[i][j]);
        }
        //printf("\n");
        res[i]=float(rand()%10000)/10000;
    }
    printf("Generate complete. Size:%d %d\n",size,size);
}