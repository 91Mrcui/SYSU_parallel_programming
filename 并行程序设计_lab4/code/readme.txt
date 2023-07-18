matrix_vector_mul原程序
matrix_vector_mul_v2（常量内存优化）
matrix_vector_mul_v3（纹理内存优化）
matrix_vector_mul_v4（cuBLAS优化）

read_data.h为处理数据的头文件，valid.cpp为验证正确性的程序

编译命令：nvcc matrix_vector_mul_v4.cu -o matrix_vector_mul -std=c++11

读入的二进制文件放入data文件夹，运行程序会在output文件夹输出结果的二进制文件。