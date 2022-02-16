#include "hashtable.h"
#include <string>
#include <iostream>

int naive_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    else return (str[0]+N)%N;
}

int ascii_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    int len = strlen(str);
    int ans = 0;
    int power = 128; 
    for(int i = 0; i < len; i++){
        ans = ans * power + int((unsigned char)str[i]); 
        ans %= N;
    }
    return ans;
}

int utf_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    int len = strlen(str);
    int i = 0;
    long long ans = 0;
    int power = 65536; 
    while(i < len){
        if((int((unsigned char)str[i]) & 128) == 0){ // 1字节
            ans = ans * power + int((unsigned char)str[i]);
            ans %= N;
            i++;
        }
        else if((int((unsigned char)str[i]) & 224) == 192){ // 2字节
            ans = ans * power + ((int((unsigned char)str[i]) & 31) << 6) + (int((unsigned char)str[i + 1]) & 63);
            ans %= N;
            i += 2;
        }
        else{ // 3字节
            ans = ans * power + ((int((unsigned char)str[i]) & 15) << 12) + ((int((unsigned char)str[i + 1]) & 63) << 6) + (int((unsigned char)str[i + 2]) & 63);
            ans %= N;
            i += 3;
        }
    }
    return ans;
}

int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    return (last_choice + 1) % table_size;
}
void linear_probe::init(){
    return;// do nothing
}

int twoSquare_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    int ans;
    if(negative){ // 反向
        ans = (last_choice - square * square) % table_size; // 减去square^2
        if(ans < 0) ans += table_size;
        square++; // 当前square两个方向都已完成
        negative = !negative; // 换向
    }
    else{ // 正向
        ans = (last_choice + square * square) % table_size; // 加上square^2
        negative = !negative; // 换向
    }
    return ans;
}
void twoSquare_probe::init(){
    square = 1; // 记录试探距离square^2
    negative = false; // 记录试探方向（正、负）
    return;
}

int overflow_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    int ans = table_size + counter; // 试探位置（公共溢出区起点 + 偏移量）
    counter++; // 下一试探位置
    return ans;
}
void overflow_probe::init(){
    counter = 0; // 记录试探位置偏移量（试探位置为table_size + counter）
    return;
}


