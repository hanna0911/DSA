#include <cstdio>
#include <string>
#include "hashtable.h"
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
    clock_t start_time;
	start_time = clock();
    int hashing_S = atoi(argv[1]), collision_S = atoi(argv[2]); // 用命令行参数选择不同的strategy
    hashing_strategy* my_hashing;
    collision_strategy* my_colision;
    switch(hashing_S){
        case(0):
            my_hashing = new naive_hashing(); // 0：naive hashing
            break;
        case(1):
            my_hashing = new ascii_hashing(); // 1：针对ascii的hashing
            break;
        case(2):
            my_hashing = new utf_hashing(); // 2：针对utf-8的hashing
            break;
    }
    switch(collision_S){
        case(0):
            my_colision = new linear_probe(); // 0：线性试探
            break;
        case(1):
            my_colision = new twoSquare_probe(); // 1：双向平方试探
            break;
        case(2):
            my_colision = new overflow_probe(); // 2：公共溢出区试探
            break;
    }
    hashtable table(600011, my_hashing, my_colision);
    string filename_temp = "../";
    filename_temp = filename_temp + argv[3]; // 第三个命令行参数输入input文件的文件名
    const char *filename_in = (filename_temp + ".in").c_str();
    freopen(filename_in, "r", stdin);
    // string filename = "../";
    // filename = filename + char(hashing_S + int('0')) + "_" + char(collision_S + int('0')) + "_" + filename_temp + ".out";
    // const char *filename_out = filename.c_str();
    // freopen(filename_out, "w", stdout);
    char buffer[1000];int data;
    int type;
    while(true){
        scanf("%d", &type);
        if(type == 0){
            scanf("%s", buffer);scanf("%d",&data);
            table.insert(hash_entry(buffer, data));
        }else if(type == 1){
            scanf("%s",buffer);
            // printf("%d\n", table.query(buffer)); 
            table.query(buffer); // 此处注释掉了输出数据而仅保留查询操作，以减少输出所需时间带来的干扰
        }else break;
        clock_t end_time = clock();
        if(((float) (end_time - start_time)) / CLOCKS_PER_SEC >= 10) break; // 若超过10s，则终止运行
    }

    clock_t end_time = clock();
    string hashingList[3] = {"naive_hashing", "ascii_hashing", "utf8_hashing"};
    string collisionList[3] = {"linear_probe", "two_square_probe", "overflow_probe"};
    fclose(stdin);
    // fclose(stdout);
    ofstream fout;
    fout.open("../result.txt", ios::app);
    fout << hashingList[hashing_S] << ", " << collisionList[collision_S] << ", " << filename_temp << ".in";
    fout << ", time: " << ((float) (end_time - start_time)) / CLOCKS_PER_SEC << " s"; // 程序用时
    if(((float) (end_time - start_time)) / CLOCKS_PER_SEC >= 10) fout << " (break)";
    fout << endl;
    fout.close();
    return 0;
}