#include "sort.h"

void two_way_merge(int * elem, int* &C, int* &D, int &ci, int &lc, int &di, int &ld, int &ei){
    int min, max;
    if(ci + 1 < lc){ // c路中至少还有2个元素
        compare(C[ci], C[ci + 1], D[di], &max, &min); // 比较c路中前2个与d路中第1个元素
        if(min == C[ci]){ // 放入最小元素
            elem[ei] = C[ci];
            ei++;
            ci++;
            if(max == C[ci]){ // c路中第“2”个(放入最小之前的第2个)最大
                elem[ei] = D[di]; // 放入d路首个元素
                ei++;
                di++;
            }
            else{ // d路首个元素最大
                elem[ei] = C[ci]; // 放入c路中第“2”个元素
                ei++;
                ci++;
            }
        }
        else{ // min == D[di]
            elem[ei] = D[di];
            ei++;
            di++;
        }
    }
    else if(di + 1 < ld){
        compare(C[ci], D[di], D[di + 1], &max, &min);
        if(min == C[ci]){
            elem[ei] = C[ci];
            ei++;
            ci++;
        }
        else{ // min == D[di]
            elem[ei] = D[di];
            ei++;
            di++;
            if(max == C[ci]){
                elem[ei] = D[di];
                ei++;
                di++;
            }
            else{ // max == D[di + 1], 注意此处di含义是++之前的di
                elem[ei] = C[ci];
                ei++;
                ci++;
            }
        }
    }
    else{ // 两路均各只剩下1个元素
        compare(C[ci], D[di], D[di], &max, &min);
        if(min == C[ci]){ // 谁小先放谁
            elem[ei] = C[ci];
            ei++;
            ci++;
            elem[ei] = D[di];
            ei++;
            di++;
        }
        else{
            elem[ei] = D[di];
            ei++;
            di++;
            elem[ei] = C[ci];
            ei++;
            ci++;
        }
    }
}

void mergeTwo(int * elem, int* &C, int* &D, int &ci, int &lc, int &di, int &ld, int &ei){
    while(ci < lc && di < ld) two_way_merge(elem, C, D, ci, lc, di, ld, ei); // 二路归并
    if(ci >= lc){ // c路耗尽
        while(di < ld){ // 把d中剩余元素全部放入elem
            elem[ei] = D[di];
            ei++;
            di++;
        }
    }
    else{
        while(ci < lc){
            elem[ei] = C[ci];
            ei++;
            ci++;
        }
    }
}

void three_way_merge(int * elem, int* &B, int* &C, int* &D, int &bi, int &ci, int &di, int &ei, bool &b, bool &c, bool &d){
    int min, max;
    compare(B[bi], C[ci], D[di], &max, &min);
    if(min == B[bi]){ // 把最小的元素放入elem
        elem[ei] = B[bi];
        ei++;
        bi++;
        if(max == C[ci]) c = true; // 并标记“非最小元”
        else d = true;
    }
    else if(min == C[ci]){
        elem[ei] = C[ci];
        ei++;
        ci++;
        if(max == B[bi]) b = true;
        else d = true;
    }
    else{
        elem[ei] = D[di];
        ei++;
        di++;
        if(max == B[bi]) b = true;
        else c = true;
    }
}

void mergeThree(int * elem, int* &B, int* &C, int* &D, int &bi, int &lb, int &ci, int &lc, int &di, int &ld, int &ei, bool &b, bool &c, bool &d){
    while(bi < lb && ci < lc && di < ld) three_way_merge(elem, B, C, D, bi, ci, di, ei, b, c, d); // 三路归并
    if(bi >= lb) mergeTwo(elem, C, D, ci, lc, di, ld, ei); // b路元素耗尽
    else if(ci >= lc) mergeTwo(elem, B, D, bi, lb, di, ld, ei);
    else mergeTwo(elem, B, C, bi, lb, ci, lc, ei);
}

void merge(int * elem, int lo, int leftmi, int mi, int rightmi, int hi){ //[lo,leftmi), [leftmi,mi), [mi,rightmi)和[rightmi, hi)各自有序，lo < leftmi < mi < rightmi < hi
    // 分配内存提供给4路
    int la = leftmi - lo;
    int lb = mi - leftmi;
    int lc = rightmi - mi;
    int ld = hi - rightmi;
    int *A = new int[la];
    int *B = new int[lb];
    int *C = new int[lc];
    int *D = new int[ld];
    for(int i = lo; i < leftmi; i++) A[i - lo] = elem[i];
    for(int i = leftmi; i < mi; i++) B[i - leftmi] = elem[i];
    for(int i = mi; i < rightmi; i++) C[i - mi] = elem[i];
    for(int i = rightmi; i < hi; i++) D[i - rightmi] = elem[i];
    int ei = lo, ai = 0, bi = 0, ci = 0, di = 0; // 初始位置
    // 进行归并
    int min, max, min2, max2; // 用于初始情况
    bool a = false, b = false, c = false, d = false; // 用于标记是否为“非最小元”
    if(ai < la && bi < lb && ci < lc && di < ld){
        // 初始情况要把4个元素全部比较才能得到最小的元素
        compare(A[ai], B[bi], C[ci], &max, &min);
        compare(min, max, D[di], &max2, &min2);
        if(min2 == min){ // 4路中最小的放入elem
            if(min == A[ai]){
                elem[ei] = A[ai];
                ei++;
                ai++;
            }
            else if(min == B[bi]){
                elem[ei] = B[bi];
                ei++;
                bi++;
            }
            else{ 
                elem[ei] = C[ci];
                ei++;
                ci++;
            }
        }
        else{ // min2 == D[di])
            elem[ei] = D[di];
            ei++;
            di++;
        }
        if(max2 == max){ // 4路中最大的标记为“非最小元”
            if(max == A[ai]) a = true;
            else if(max == B[bi]) b = true;
            else c = true;
        }
        else d = true; // max2 == D[di])
        while(ai < la && bi < lb && ci < lc && di < ld){ // 其他情况只需在排除“非最小元”后的三个元素中选出最小的
            if(a){ // a是上次三元素比较中最大的, 即在此次选择中其一定为“非最小元”, 因此排除
                a = false; // 复位
                three_way_merge(elem, B, C, D, bi, ci, di, ei, b, c, d);
            }
            else if(b){
                b = false;
                three_way_merge(elem, A, C, D, ai, ci, di, ei, a, c, d);
            }
            else if(c){
                c = false;
                three_way_merge(elem, A, B, D, ai, bi, di, ei, a, b, d);
            }
            else{
                d = false;
                three_way_merge(elem, A, B, C, ai, bi, ci, ei, a, b, c);
            }
        }
        if(ai >= la) mergeThree(elem, B, C, D, bi, lb, ci, lc, di, ld, ei, b, c, d); // a路元素耗尽
        else if(bi >= lb) mergeThree(elem, A, C, D, ai, la, ci, lc, di, ld, ei, a, c, d);
        else if(ci >= lc) mergeThree(elem, A, B, D, ai, la, bi, lb, di, ld, ei, a, b, d);
        else mergeThree(elem, A, B, C, ai, la, bi, lb, ci, lc, ei, a, b, c);
    }
    else if(ai < la && bi < lb && ci < lc) mergeThree(elem, A, B, C, ai, la, bi, lb, ci, lc, ei, a, b, c); // d路中无元素
    else if(ai < la && bi < lb && di < ld) mergeThree(elem, A, B, D, ai, la, bi, lb, di, ld, ei, a, b, d);
    else if(bi < lb && ci < lc && di < ld) mergeThree(elem, B, C, D, bi, lb, ci, lc, di, ld, ei, b, c, d);
    else if(ai < la && ci < lc && di < ld) mergeThree(elem, A, C, D, ai, la, ci, lc, di, ld, ei, a, c, d); 
    else{
        if(ai < la && bi < lb) mergeTwo(elem, A, B, ai, la, bi, lb, ei); // c,d路中无元素
        else if(ai < la && ci < lc) mergeTwo(elem, A, C, ai, la, ci, lc, ei);
        else if(ai < la && di < ld) mergeTwo(elem, A, D, ai, la, di, ld, ei);
        else if(bi < lb && ci < lc) mergeTwo(elem, B, C, bi, lb, ci, lc, ei);
        else if(bi < lb && di < ld) mergeTwo(elem, B, D, bi, lb, di, ld, ei);
        else if(ci < lc && di < ld) mergeTwo(elem, C, D, ci, lc, di, ld, ei);
    }
    // 释放内存
    delete [] A; 
    delete [] B;
    delete [] C;
    delete [] D;
}

void mergeSort(int * elem, int lo, int hi){ // 0 <= lo < hi <= size
    if(hi - lo < 2) return; // 单元素区间自然有序
    int mi = (lo + hi) / 2; // 中点
    int leftmi = (lo + mi) / 2, rightmi = (mi + hi) / 2; // 1/4点与3/4点
    // 对共4部分分别排序
    mergeSort(elem, lo, leftmi); 
    mergeSort(elem, leftmi, mi);
    mergeSort(elem, mi, rightmi); 
    mergeSort(elem, rightmi, hi);
    merge(elem, lo, leftmi, mi, rightmi, hi); // 归并
}

void sort(int n, int limit, int *a) {
	return mergeSort(a, 0, n); // 四路归并
}