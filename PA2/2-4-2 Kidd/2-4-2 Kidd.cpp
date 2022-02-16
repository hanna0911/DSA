#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

struct operation{
    char instruct; // 'H' / 'Q'
    long long original_i; // 初始输入 i
    long long original_j; // 初始输入 j
};

long long *allnode; // 用于排序去重
long long sum; // 答案

struct node{
    long long interval; // “节点”表示的线段的长度
    long long lazy; // 懒惰标记
    long long value; // 翻牌次数
}; 

// 线段树模板, 参考资料: https://www.cnblogs.com/latent-Lin/p/13535000.html
class SegmentTree{
private:
    node * nodes;
protected:
    void push_lazy_down(int rank){ // 向下传递 lazy 值
        // 左孩子
        nodes[rank << 1].lazy += nodes[rank].lazy;
        nodes[rank << 1].value += nodes[rank << 1].interval * nodes[rank].lazy;
        // 右孩子
        nodes[(rank << 1) + 1].lazy += nodes[rank].lazy;
        nodes[(rank << 1) + 1].value += nodes[(rank << 1) + 1].interval * nodes[rank].lazy;
        nodes[rank].lazy = 0; // 传递完毕, parent 节点的 lazy 值清空
    }
public:
    SegmentTree(int nodeNum){
        // 建立 segment tree
        nodes = new node[nodeNum * 4 + 4];
        init(1, 1, nodeNum); // 从节点 1 开始初始化 (root, 最大的区间), 左、右端点 (均为闭) 为 [1, nodeNum]
    }
    void init(int rank, int left, int right){
        // 初始化
        nodes[rank].value = 0;
        nodes[rank].lazy = 0;
        nodes[rank].interval = allnode[right + 1] - allnode[left];
        if(left == right) return; // 递归基
        // 递归构建左子树、右子树
        int mid = (left + right) >> 1;
        init(rank * 2, left, mid); // 左子树
        init(rank * 2 + 1, mid + 1, right); // 右子树
    }
    void flip(int rank, int left, int right, int left_bound, int right_bound){
        if(left_bound <= left && right <= right_bound){ // 当前查询区间 [left, right] ⊆ 翻牌区间 [left_bound, right_bound]
            nodes[rank].value += nodes[rank].interval; // 当前节点 (区间) 翻牌 1 次
            nodes[rank].lazy++; // lazy 值增加 1 次
            return;
        }
        push_lazy_down(rank); // 向下传递 lazy 值
        int mid = (left + right) >> 1;
        if(left_bound <= mid) flip(rank * 2, left, mid, left_bound, right_bound); // 左子树翻牌
        if(mid + 1 <= right_bound) flip((rank << 1) + 1, mid + 1, right, left_bound, right_bound); // 右子树翻牌
        nodes[rank].value = nodes[rank << 1].value + nodes[(rank << 1) + 1].value; // 左右子树翻牌后，parent 节点更新为子树们翻牌数之和
    }
    void check(int rank, int left, int right, int left_bound, int right_bound){
        if(right < left_bound || right_bound < left) return; // 非法 (当前 check 的区间超出了 Q 的区间 [left_bound, right_bound])
        if(left_bound <= left && right <= right_bound){ // 当前 check 的区间 [left, right] ⊆ Q 的区间 [left_bound, right_bound]
            sum += nodes[rank].value; // 答案累加上当前查询到的区间翻牌数
            return;
        }
        push_lazy_down(rank); // 向下传递 lazy 值
        int mid = (left + right) >> 1;
        if(left_bound <= mid) check(rank << 1, left, mid, left_bound, right_bound); // 左子树查询
        if(mid + 1 <= right_bound) check((rank << 1) + 1, mid + 1, right, left_bound, right_bound); // 右子树查询
    }
    ~SegmentTree(){
        delete [] nodes;
    }
};

// 参考资料: 邓俊辉 dsa-2021fall 有序向量 二分查找（版本C）
int binSearch(long long * S, long long const & e, int lo, int hi){
    while(lo < hi){
        int mi = (lo + hi) >> 1;
        e < S[mi] ? hi = mi : lo = mi + 1;   
    }
    return lo - 1;
}

// cstdlib 库 qsort 排序, 参考资料: http://www.cplusplus.com/reference/cstdlib/qsort/
int compare(const void * a, const void * b){
    return (*(long long*)a - *(long long*)b);
}

int main(){
    // 输入
    int n, m;
    cin >> n >> m;
    operation operate[m + 2];
    allnode = new long long[2 * m + 4];
    for(int i = 1; i <= m; i++){
        cin >> operate[i].instruct >> operate[i].original_i >> operate[i].original_j;
        operate[i].original_j++; // 把区间变成左闭右开 [i, j)
        allnode[2 * i - 1] = operate[i].original_i;
        allnode[2 * i] = operate[i].original_j;
    }

    // 离散化：排序 + 去重
    qsort(&allnode[1], 2 * m, sizeof(allnode[1]), compare);
    int nodeNum = 1;
    for(int i = 2; i <= 2 * m; i++){
        if(allnode[i] != allnode[nodeNum]){ // 仅保留不重复的点, 去除重复的
            nodeNum++;
            allnode[nodeNum] = allnode[i];
        }
    }
    allnode[nodeNum + 1] = 2147483649; // 最右端数据定为 max

    // 建立线段树
    SegmentTree segmentTree(nodeNum);

    // 执行指令
    for(int i = 1; i <= m; i++){
        // 二分查找，建立离散化后坐标的映射关系
        int new_i = binSearch(allnode, operate[i].original_i, 1, nodeNum + 1);
        int new_j = binSearch(allnode, operate[i].original_j, 1, nodeNum + 1);
        if(operate[i].instruct == 'H') // 翻牌
            segmentTree.flip(1, 1, nodeNum, new_i, new_j - 1); // 从节点 1: [1, nodeNum] 开始按照左闭右闭更新 [i, j) -> [i, j - 1]
        else{ // 查询
            sum = 0;
            segmentTree.check(1, 1, nodeNum, new_i, new_j - 1); // 从节点 1: [1, nodeNum] 开始按照左闭右闭查询 [i, j) -> [i, j - 1]
            cout << sum << endl;
        }
    }

    delete[] allnode;
    return 0;
}