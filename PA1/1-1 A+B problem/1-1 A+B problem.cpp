//
//  main.cpp
//  CST2021F 1-1 A+B problem new
//
//  Created by Hanna Wang on 2021/10/6.
//  Copyright © 2021 Hanna Wang. All rights reserved.
//

// 更新1: 压位由8位更新到9位
// 更新2: 字符串转数值优化
// 更新3: 乘法、加法一起做 (一个for循环)

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <cstdlib>

int mark_ansLen = 0; // 记录答案的长度，便于输出

using namespace std;

// 更快的 atoi / atoll 方法
// 参考资料: https://tombarta.wordpress.com/2008/04/23/specializing-atoi/
long long char_to_int(const char *x){ // 将字符串转为long long
    int len = strlen(x);
    long long sum = 0;
    switch(len){ // 9位数
        case 10:
            sum += (x[len - 10] - '0') * 1000000000;
        case 9:
            sum += (x[len - 9] - '0') * 100000000;
        case 8:
            sum += (x[len - 8] - '0') * 10000000;
        case 7:
            sum += (x[len - 7] - '0') * 1000000;
        case 6:
            sum += (x[len - 6] - '0') * 100000;
        case 5:
            sum += (x[len - 5] - '0') * 10000;
        case 4:
            sum += (x[len- 4] - '0') * 1000;
        case 3:
            sum += (x[len - 3] - '0') * 100;
        case 2:
            sum += (x[len - 2] - '0') * 10;
        case 1:
            sum += (x[len- 1] - '0');
    }
    return sum;
}

long long* convertion(const char * x, int xLen, int xIntLen){ // 将输入字符串转为long long型数组
    char t[10] = ""; // 临时变量，用于传递strncpy过程的值
    long long *xInt = new long long[xIntLen];
    for(int i = 0; i < xIntLen - 1; i++){
        strncpy(t, &x[xLen - (i + 1) * 9], 9); // 每9位记为1个long long型数
        t[9] = '\0';
        xInt[xIntLen - i - 1] = char_to_int(t);
    }
    strncpy(t, x, xLen - (xIntLen - 1) * 9); // 首位long long数因为可能不足9位数，进行特殊处理
    t[xLen - (xIntLen - 1) * 9] = '\0';
    xInt[0] = char_to_int(t);
    return xInt;
}

void multiply(long long * aInt, int aIntLen, long long num, long long * answer, int base){
    long long jinwei = 0; 
    int pos = base; // base是计算的该位乘法的偏移量
    for(int i = aIntLen - 1; i >= 0; i--){
        long long sum = answer[pos] + aInt[i] * num + jinwei; // 乘法和加法同时做，减少循环次数
        if(sum >= 1000000000){
            // 计算合法答案及进位
            jinwei = sum / 1000000000;
            sum = sum - jinwei * 1000000000;
        }
        else jinwei = 0;
        answer[pos] = sum;
        pos++;
    }
    if(jinwei > 0){ // 做完所有乘法与累加后，考虑最后一个进位
        answer[pos] += jinwei;
        pos++;
    }
    mark_ansLen = pos; // 记录最终答案的长度，以便于输出
}

int main(int argc, const char * argv[]) {
    
    int n = 0;
    scanf("%d", &n);
    
    for(int i = 0; i < n; i++){

        // 输入格式为字符串
        char a[5002], b[5002];
        scanf("%s%s", a, b);

        if(a[0] == '0' || b[0] == '0'){ // 特判乘数为0的情况
            cout << 0 << endl;
            continue;
        }

        int aLen = strlen(a), bLen = strlen(b);
        
        // 一个long long型数为9位数，即10^10进制, 并向右对齐 (例. 12 345678901)
        int aIntLen = (aLen + 8) / 9, bIntLen = (bLen + 8) / 9; // 9个字符 -> 1个long long型数
        long long * aInt = convertion(a, aLen, aIntLen);
        long long * bInt = convertion(b, bLen, bIntLen);
        
        // 乘法 + 加法
        int ansLen = aIntLen + bIntLen;
        long long answer[ansLen]; // 倒序存储答案 (因为竖式乘法计算时，由右至左即逆序计算)
        memset(answer, 0, sizeof(answer));
        
        for(int j = bIntLen - 1; j >= 0; j--){
            multiply(aInt, aIntLen, bInt[j], answer, bIntLen - 1 - j);
        }
        
        cout << answer[mark_ansLen - 1]; // 首位不补零，特殊输出
        for(int j = mark_ansLen - 2; j >= 0; j--){
            printf("%09lld", answer[j]); // 补零 (例. 12 -> 000000012)
        }
        cout << endl;
        
        // 释放内存
        delete[] aInt;
        delete[] bInt;
    }
    return 0;
}

