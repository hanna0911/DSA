//
//  main.cpp
//  CST2021F 1-2 Graphics
//
//  Created by Hanna Wang on 2021/10/6.
//  Copyright © 2021 Hanna Wang. All rights reserved.
//

#include <iostream>
#include <cstdlib>

using namespace std;

// cstdlib库qsort排序
// 参考资料: http://www.cplusplus.com/reference/cstdlib/qsort/
int compare(const void * a, const void * b){
    return (*(long long*)a - *(long long*)b);
}

// 参考资料: 邓俊辉 dsa-2021fall 有序向量 二分查找（版本C）
// 返回大于等于目标 (xp, yp) 的坐标 (xi, yi) 的 i 值 (对同一个 x ，返回满足 y[i] <= py < y[i + 1] 的 i 值)
int binSearch(long long * x, long long * y, long long const & px, long long const & py, int lo, int hi){
    while(lo < hi){
        int mi = (lo + hi) >> 1;
        py * x[mi] < y[mi] * (x[mi] - px) ? hi = mi : lo = mi + 1; // 比较方法该为坐标比较
    }
    return lo;
}

int main(int argc, const char * argv[]) {
    
    // 输入 (xi, yi)
    int n = 0;
    cin >> n;
    long long x[n + 1], y[n + 1];
    for(int i = 0; i < n; i++) scanf("%lld", &x[i]);
    for(int i = 0; i < n; i++) scanf("%lld", &y[i]);
    
    // cstdlib库qsort排序
    // 参考资料: http://www.cplusplus.com/reference/cstdlib/qsort/
    qsort(x, n, sizeof(x[0]), compare);
    qsort(y, n, sizeof(y[0]), compare);
    
    // 输入测例 P(px, py)
    int m = 0;
    cin >> m;
    for(int i = 0; i < m; i++){
        long long px = 0, py = 0;
        scanf("%lld%lld", &px, &py);
        printf("%d\n", binSearch(x, y, px, py, 0, n));
    }
    
    return 0;
}
