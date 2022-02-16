#include <iostream>
#include <cstring>

using namespace std;

#define numLength 64
#define invalid -1

// 空间: 500000*65*12/1024/1024+500000*8/1024/1024 = 375.7476806641 MB

// reference: 邓俊辉《数据结构》02-G1 向量-位图：数据结构
class Bitmap{
private:
    int N;
    char * M;
public:
    Bitmap(int n = numLength){
        M = new char[N = (n + 7) / 8];
        memset(M, 0, N);
    }
    ~Bitmap(){
        delete[] M;
        M = nullptr;
    }
    void set(int k){
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }
    char operator [](int k){ // 重载[]运算符, 后续方便直接取k处所存的“字符”; 等价于原来bool test(int k)效果
        if(M[k >> 3] & (0x80 >> (k & 0x07))) return '1';
        return '0';
    }
}*notepad;

struct treeNode{ // 字典树节点
    int count, left, right; // 多少路径经过该点、左孩子在bucket中的rank、右孩子在bucket中的rank
    treeNode(): count(0), left(invalid), right(invalid){} // 初始化
}bucket[32500005]; // 65*5000000 = 32500000
int head = 1; // head=0处存放根节点, 栈顶从head=1开始

class Trie{ // 字典树
public:
    int root;
    Trie(): root(0){} // 根节点rank=0
    void add(int id, Bitmap& num){ // 添加路径
        int cur = root;
        int pos = 0;
        while(pos < numLength){
            if(num[pos] == '0'){ // 0: 走左边
                if(bucket[cur].left == invalid){ // 新创立分支，分配空间
                    bucket[cur].left = head;
                    head++;
                }
                bucket[cur].count++; // 经过次数++
                cur = bucket[cur].left; // 下一个探访的节点
            }
            else{ // 1: 走右边
                if(bucket[cur].right == invalid){ // 新创立分支，分配空间
                    bucket[cur].right = head;
                    head++;
                }
                bucket[cur].count++; // 经过次数++
                cur = bucket[cur].right; // 下一个探访的节点
            }
            pos++;
        }
        bucket[cur].count++;
        bucket[cur].left = id; // 保存id
    }
    void remove(Bitmap& num){ // 删除路径
        int cur = root;
        int pos = 0;
        while(pos < numLength){
            if(num[pos] == '0'){ // 0: 走左边 
                if(bucket[cur].count == 1){ // 此节点处只剩待删除路径这1条路径
                    bucket[cur].count--;
                    int temp = bucket[cur].left;
                    bucket[cur].left = invalid;
                    cur = temp;
                }
                else{ // 还剩多条路径
                    bucket[cur].count--;
                    cur = bucket[cur].left;
                }
            }
            else{ // 1: 走右边
                if(bucket[cur].count == 1){ // 此节点处只剩待删除路径这1条路径
                    bucket[cur].count--;
                    int temp = bucket[cur].right;
                    bucket[cur].right = invalid;
                    cur = temp;
                }
                else{ // 还剩多条路径
                    bucket[cur].count--;
                    cur = bucket[cur].right;
                }
            }
            pos++;
        }
        if(bucket[cur].count == 1) bucket[cur].left = invalid;
        bucket[cur].count--;
    }
    int search(int id, Bitmap& num){ // 查询路径num对应的异或值最大的路径
        int cur = root;
        int pos = 0;
        while(pos < numLength){
            if(num[pos] == '0'){ // 0: 尽量走右边
                if(bucket[cur].right != invalid && bucket[bucket[cur].right].count > 0) // 走右边
                    cur = bucket[cur].right;
                else cur = bucket[cur].left; // 走左边
            }
            else{ // 1: 尽量走左边
                if(bucket[cur].left != invalid && bucket[bucket[cur].left].count > 0) // 走左边
                    cur = bucket[cur].left;
                else cur = bucket[cur].right; // 走右边
            }
            pos++;
        }
        return bucket[cur].left;
    }
}myTrie;

void read(int pos){ // 读取输入
    char temp[numLength + 1];
    cin >> temp;
    for(int i = 0; i < numLength; i++){
        if(temp[i] == '0') notepad[pos].set(i);
    }
}

void mirror_process(){
    int n = 0, k = 0;
    cin >> n >> k;
    notepad = new Bitmap[n + 1]; // 用于存储输入数据
    for(int i = n - 1; i >= 0; i--)
        read(i);
    int ans[n + 1]; // 用于记录答案, 最后逆序输出
    if(k + 2 >= n){ // 1/2窗口大小覆盖住全部输入数据
        // 建立Trie
        for(int i = 0; i < n; i++)
            myTrie.add(n - 1 - i, notepad[i]);
        // 查找
        for(int i = 0; i < n; i++)
            ans[i] = myTrie.search(n - 1 - i, notepad[i]);
    }
    else{
        // 预处理: 一口气加好id为0~k+1的路径
        for(int i = 0; i < k + 1; i++)
            myTrie.add(n - 1 - i, notepad[i]);
        // 0~k+1: 每次多加右侧的一个路径
        for(int i = 0; i < min(k + 2, n - (k + 1)); i++){ 
            myTrie.add(n - 1 - (i + k + 1), notepad[i + k + 1]); // 在右侧多加的路径id=i+k+1
            ans[i] = myTrie.search(n - 1 - i, notepad[i]); // 当前查询id=i
        }
        if(k + 2 <= n - (k + 1)){ 
            // k+2~n-(k+2): 每次多加右侧一个路经、删掉原有最左侧的路经
            for(int i = k + 2; i < n - (k + 1); i++){
                myTrie.remove(notepad[i - (k + 2)]); // 删除“最左侧的”的路径
                myTrie.add(n - 1 - (i + k + 1), notepad[i + k + 1]); // 在右侧多加的路径id=i+k+1
                ans[i] = myTrie.search(n - 1 - i, notepad[i]); 
            }
        }
        else{ // n-(k+1)<k+2
            // n-(k+1)~k+1: 不变
            for(int i = n - (k + 1); i < k + 2; i++)
                ans[i] = myTrie.search(n - 1 - i, notepad[i]); 
        }
        // n-(k+1)~n-1: 删掉原有最左侧一个路经
        for(int i = max(n - (k + 1), k + 2); i < n; i++){
            myTrie.remove(notepad[i - (k + 2)]); // 删除“最左侧的”的路径
            ans[i] = myTrie.search(n - 1 - i, notepad[i]); 
        }
    }
    // 输出答案
    if(ans[n - 1] == 0) cout << 1 << endl; // 特殊情况: 前k+2个输入一样
    else cout << ans[n - 1] << endl;
    for(int i = n - 2; i >= 0; i--) 
        cout << ans[i] << endl;
}

int main(){
    ios::sync_with_stdio(false); // 输入输出加速
    cin.tie(0);
    cout.tie(0);
    mirror_process(); // 输入后, “窗口移动”从后往前处理
    return 0;
}