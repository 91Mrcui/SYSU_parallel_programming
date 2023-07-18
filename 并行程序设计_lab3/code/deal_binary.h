#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
using std::string;

void read(std::vector<std::vector<int> >& array,std::vector<float>& res,string instr,string out_str){
    int row_size, col_size;
    std::vector<int> matrix_test;
    std::ifstream inFile(instr, std::ios::binary);
    if (inFile.is_open()) {
    // First, read the size of the matrix (ROWS and COLS).
    inFile.read(reinterpret_cast<char *>(&row_size),sizeof(row_size));
    inFile.read(reinterpret_cast<char *>(&col_size),sizeof(col_size));
    // Resize the matrix based on the size read from thefile.
    matrix_test.resize(row_size * col_size);
    // Then, read the actual data of the matrix.
    inFile.read(reinterpret_cast<char *>
    (matrix_test.data()), matrix_test.size() * sizeof(int));
    }

    array.resize(row_size);
    for(int i=0;i<row_size;i++){
        array[i].resize(col_size);
    }
    
    for(int i=0;i<row_size;i++){
        for(int j=0;j<col_size;j++){
             array[i][j]=matrix_test[i*col_size+j];
        }
    }
    printf("size:%d %d\n",row_size,col_size);
    //printf("##########################################################################################\n");
    
    res.resize(row_size * col_size);
    std::ifstream inFile_2(out_str, std::ios::binary);
    if (inFile_2.is_open()) {
    // Resize the matrix based on the size read from thefile.
    // Then, read the actual data of the matrix.
    inFile_2.read(reinterpret_cast<char *>(res.data()), res.size() * sizeof(float));
    }
}



void write(std::vector<std::vector<int> >& array,std::vector<std::vector<float> > &res,string in_str,string out_str){
    std::ofstream inFile(in_str, std::ios::binary);
    std::ofstream outFile(out_str, std::ios::binary);

    int row=array.size();
    int col=array[0].size();

    std::vector<int> in_matrix;
    std::vector<float> entropy_matrix;
    in_matrix.resize(row*col+2);
    entropy_matrix.resize(row*col);

    in_matrix[0]=row;
    in_matrix[1]=col;

    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            in_matrix[i*col+j+2]=array[i][j];
            entropy_matrix[i*col+j]=res[i][j];
        }
    }

    //写输入
    if (inFile.is_open()) {
        inFile.write(reinterpret_cast<const char *>(in_matrix.data()), in_matrix.size() *sizeof(int));
        inFile.close();
    } 
    else {
        std::cout << "Unable to open file";
    }

    //写熵矩阵
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char *>(entropy_matrix.data()), entropy_matrix.size() *sizeof(float));
        outFile.close();
    } 
    else {
        std::cout << "Unable to open file";
    }
}