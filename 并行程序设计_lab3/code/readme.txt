CPU串行版本：
baseline.cpp
baseline_lu.cpp

CPU并行版本：
openmp.cpp
openmp_lu.cpp

GPU版本：
cuda_bl.cu
cuda_bl_lu.cu
cuda_2d.cu
share_mem.cu

deal_binary.h是读写二进制文件的头文件

若要运行程序来测试其他测例，则要修改源文件中的路径：
在main函数中可以看到有read()和write()两个函数，修改参数中的路径即可，如：
read(array,res,"test/test.in","test/test.out")中，"test/test.in"是要读取的输入文件的路径，"test/test.out"是要读取的输出文件的路径（可以忽略）
write(array,entropyArray,"output/share_mem.in","output/share_mem.out")中，"output/share_mem.out"是该程序计算得到的熵矩阵的保存路径
修改这两个路径即可。

编译命令：
nvcc cuda_bl.cu -o cuda_bl -std=c++11
g++ baseline.cpp -o baseline