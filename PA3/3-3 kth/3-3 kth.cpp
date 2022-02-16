#include "kth.h"
#include <iostream>

using namespace std;

#define HeapSize 6000005 // 2000000*3最坏情况出1入3
#define Parent(i) (((i) - 1) >> 1) //PQ[i]的父节点（floor((i-1)/2)，i无论正负）
#define LChild(i) (1 + ((i) << 1)) //PQ[i]的左孩子
#define RChild(i) ((1 + (i)) << 1) //PQ[i]的右孩子
#define InHeap(n, i) (((-1) < (i)) && ((i) < (n))) //判断PQ[i]是否合法
#define LChildValid(n, i) InHeap(n, LChild(i)) //判断PQ[i]是否有一个（左）孩子
#define RChildValid(n, i) InHeap(n, RChild(i)) //判断PQ[i]是否有两个孩子
#define Smaller(PQ, i, j) ((PQ[j] < PQ[i]) ? j : i) // 取小者（等时前者优先）
#define ProperParent(PQ, n, i) /*父子（至多）三者中的小者*/ (RChildValid(n, i) ? Smaller(PQ, Smaller(PQ, i, LChild(i)), RChild(i)) : (LChildValid(n, i) ? Smaller(PQ, i, LChild(i)) : i)) // 相等时父节点优先，如此可避免不必要的交换

int *a, *b, *c;
class Node{ // 堆中存储的元素类型: 三维坐标 (x, y, z)
public:
	int x, y, z; // 存储三个坐标轴对应的rank
	bool operator < (Node const& node){ // 重载比较运算符
    	return compare(a[this->x], b[this->y], c[this->z], a[node.x], b[node.y], c[node.z]);
	}
};

// 邓俊辉《数据结构》完全二叉堆 (pq_complheap, 有所改动), https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/
int percolateDown(Node* A, int n, int i){
	int j; // i及其（至多两个）孩子中，堪为父者
	while(i != (j = ProperParent(A, n, i))){ // 只要i非j，则
		swap(A[i], A[j]); 
		i = j; 
	} // 二者换位，并继续考查下降后的i
	return i; // 返回下滤抵达的位置（亦i亦j）
}
int percolateUp(Node* A, int i){
    while(0 < i){ // 在抵达堆顶之前，反复地
       	int j = Parent(i); // 考查[i]之父亲[j]
       	if(A[j] < A[i]) break; // 一旦父子顺序，上滤旋即完成；否则
       	swap(A[i], A[j]); 
		i = j; // 父子换位，并继续考查上一层
    } 
    return i; // 返回上滤最终抵达的位置
}
class Heap{ // 完全二叉堆
public:
	Node* data;
	int size;
	Heap(int n = HeapSize){
		size = 0;
		data = new Node[n];
	}
	~Heap(){
		delete [] data;
	}
    void insert(Node e){ // 将词条插入完全二叉堆中
    	data[size] = e; // 首先将新词条接至向量末尾
		size++;
    	percolateUp(data, size - 1); // 再对该词条实施上滤调整
 	}
    Node delMin(){ // 删除非空完全二叉堆中优先级最高的词条
		Node minElem = data[0]; 
		data[0] = data[--size]; // 摘除堆顶（首词条），代之以末词条
		percolateDown(data, size, 0); // 对新堆顶实施下滤
		return minElem; // 返回此前备份的最小词条
 	}
};

int compare_x(const void * a, const void * b){
	return compare(*(int*)a, 1, 1, *(int*)b, 1, 1);
}
int compare_y(const void * a, const void * b){
	return compare(1, *(int*)a, 1, 1, *(int*)b, 1);
}
int compare_z(const void * a, const void * b){
	return compare(1, 1, *(int*)a, 1, 1, *(int*)b);
}

void get_kth(int n, int k, int *x, int *y, int *z) {
	// 对每个数组分别排序 (利用控制变量法设计比较器)
    a = new int[n];
	b = new int[n];
	c = new int[n];
	for(int i = 1; i <= n; i++){ // 初始化, 存储三个坐标轴的坐标分量1～n
		a[i - 1] = i;
		b[i - 1] = i;
		c[i - 1] = i;
	}
	qsort(a, n, sizeof(a[0]), compare_x);
	qsort(b, n, sizeof(b[0]), compare_y);
	qsort(c, n, sizeof(c[0]), compare_z);
	
	// 规定: x轴上的点出1入3, xy平面上的点出1入2, z轴及yz面及xz面上的点出1入1
	Node cur = {n - 1, n - 1, n - 1};
	Heap heap;
	heap.insert(cur);
	int del = 0;
	while(del < k){
		cur = heap.delMin(); // 摘除堆顶并存到cur
        del++;
		// 将与cur相连的点加入堆中 (按照规定界定相连的点并添加)
        if(cur.y == n - 1 && cur.z == n - 1){ // x轴上的点
            if(cur.x - 1 >= 0){
                Node addtemp = {cur.x - 1, cur.y, cur.z};
                heap.insert(addtemp);
            }
            if(cur.y - 1 >= 0){
                Node addtemp = {cur.x, cur.y - 1, cur.z};
                heap.insert(addtemp);
            }
            if(cur.z - 1 >= 0){
                Node addtemp = {cur.x, cur.y, cur.z - 1};
                heap.insert(addtemp);
            }
        }
        else if(cur.z == n - 1){ // xy面
            if(cur.y - 1 >= 0){
                Node addtemp = {cur.x, cur.y - 1, cur.z};
                heap.insert(addtemp);
            }
            if(cur.z - 1 >= 0){
                Node addtemp = {cur.x, cur.y, cur.z - 1};
                heap.insert(addtemp);
            }
        }
        else{ // 其他只沿z方向移动的
            if(cur.z - 1 >= 0){
                Node addtemp = {cur.x, cur.y, cur.z - 1};
                heap.insert(addtemp);
            }
        }
	}
	// 返回答案
    *x = a[cur.x];
	*y = b[cur.y];
	*z = c[cur.z];
	delete [] a;
	delete [] b;
	delete [] c;
}
