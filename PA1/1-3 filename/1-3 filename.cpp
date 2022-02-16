//
//  main.cpp
//  CST2021F 1-3 filename
//
//  Created by Hanna Wang on 2021/9/21.
//  Copyright © 2021 Hanna Wang. All rights reserved.
//

// 更新1: m 行矩阵 -> 2 行矩阵（降低空间复杂度）
// 更新2: 动态规划搜索范围 n^2 -> diagonal n * (2k + 1) (对角线处偏移2k，降低时间复杂度)

#include <iostream>

using namespace std;

int min_of_two(int x, int y){
    if(x < y) return x;
    return y;
}

int max_of_two(int x, int y){
    if(x > y) return x;
    return y;
}

int max_of_three(int x, int y, int z){
    if(x > y){
        if(x > z) return x;
        else return z;
    }
    else{
        if(y > z) return y;
        else return z;
    }
}

int main(int argc, const char * argv[]) {
    
    int n = 0, m = 0, k = 0;
    cin >> n >> m >> k;
    
    char old[n];
    char goal[m];
    cin >> old >> goal;
    
    int st[2][n + 2]; // 状态矩阵
    
    // 初始化状态矩阵
    for(int i = 0; i < 2; i++){
        for(int j = 0; j <= n; j++){
            st[i][j] = 0;
        }
    }
    int previous = 1;
    int current = 0;

    // 动态规划
    int ans = 0;
    for(int i = 1; i <= m; i++){
        swap(previous, current); // 切换更新所需参考的“上一行”
        for(int j = max_of_two(1, i - k); j <= min_of_two(i + k, n); j++){ // 只需要更新对角线处 ± k，注意不要越界
            if(old[j - 1] == goal[i - 1])
                st[current][j] = max_of_three(st[current][j - 1], st[previous][j], st[previous][j - 1] + 1); // 状态转移方程
            else st[current][j] = max_of_two(st[current][j - 1], st[previous][j]); // 状态转移方程
        }
    }
    
    // 计算修改次数
    ans = m + n - 2 * st[current][n]; // 修改次数与最长公共序列长度的关系
    if(ans <= k) cout << ans;
    else cout << -1;
    
    return 0;
}
