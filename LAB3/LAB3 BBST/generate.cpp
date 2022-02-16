#include <iostream>
#include <string.h>


using namespace std;

inline void randomData(){
    srand((unsigned int)time(NULL));
    int n = 1000000; // 总操作数
    int A = 500000, C = 500000; // 插入、查询数量
    cout << n << endl;
    for(int i = 0; i < A; i++)
        cout << "A " << rand() % n << endl;
    for(int i = 0; i < C; i++)
        cout << "C " << rand() % n << endl;
}

inline void orderData(){
    int n = 1000000; // 总操作数
    int A = 500000, C = 500000; // 插入、查询数量
    cout << n << endl;
    for(int i = 1; i <= A; i++)
        cout << "A " << i << endl;
    for(int i = 1; i <= C; i++)
        cout << "C " << i << endl;
}

inline void localData(){
    int n = 1000000; // 总操作数
    int A = 500000, C = 500000; // 插入、查询数量
    cout << n << endl;
    int k = 10; // 局部扰动区间大小
    for(int i = 0; i < A; i++)
        cout << "A " << rand() % n << endl;
    for(int i = 0; i < C; i++)
        cout << "C " << rand() % k << endl;
}

int main(int argv, char* argc[]){
    int type = atoi(argc[1]);
    string filename_temp = "";
    filename_temp = filename_temp + argc[1];
    filename_temp = filename_temp + ".in";
    const char * filename = filename_temp.c_str();
    freopen(filename, "w", stdout);
    switch(type){
        case 0: // 随机数据
            randomData();
            break;
        case 1: // 顺序数据
            orderData();
            break;
        case 2: // 局部数据
            localData();
            break;
    }
    fclose(stdout);
    return 0;
}