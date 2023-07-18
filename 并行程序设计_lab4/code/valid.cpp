#include "read_data.h"

int SIZE=2048;

void test(string file1,string file2){
    std::vector<float> res1;
    std::vector<float> res2;
    read_res(res1,file1,SIZE);
    read_res(res2,file2,SIZE);
    bool flag=false;
    for(int i=0;i<SIZE;i++){
        if(fabs(res1[i]-res2[i])>1e-5){
            printf("%.5f %.5f\n",res1[i],res2[i]);
            flag=true;
        }
    }
    if(flag==false)
    printf("The result is right.\n");
}

int main(){
    string file1="data/test5.out";
    string file2="output/res5.out";
    test(file1,file2);
    return 0;
}