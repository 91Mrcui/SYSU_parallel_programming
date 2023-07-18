#include "deal_binary.h"


void read_my_res(std::vector<std::vector<int> >& array,std::vector<float>& res){
    int row_size, col_size;
    std::vector<int> matrix_test;
    std::ifstream inFile("output/baseline.in", std::ios::binary);
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
    printf("%d %d\n",row_size,col_size);
    printf("##########################################################################################\n");
    
    res.resize(row_size * col_size);
    std::ifstream inFile_2("output/baseline.out", std::ios::binary);
    if (inFile_2.is_open()) {
    // Resize the matrix based on the size read from thefile.
    // Then, read the actual data of the matrix.
    inFile_2.read(reinterpret_cast<char *>(res.data()), res.size() * sizeof(float));
    }
}

int main(){
    std::vector<std::vector<int> > array;
    std::vector<float> res;
    read(array,res,"output/baseline.in","output/baseline.out");

    std::vector<std::vector<int> > array2;
    std::vector<float> res2;
    read_my_res(array2,res2);

    int row=array.size();
    int col=array[0].size();

    for(int i=0;i<array.size();i++){
        for(int j=0;j<array[0].size();j++){
            //printf("%d %d\n",array[i][j],array2[i][j]);
            //if(fabs(res[i*col+j]-res2[i*col+j])>1e-5)
            printf("%f %f\n",res[i*col+j],res2[i*col+j]);
        }
    }

   

    return 0;
}