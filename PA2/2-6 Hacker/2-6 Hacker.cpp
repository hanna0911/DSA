#include "crc32.h"
#include <iostream>
#include <cstring>
#include <typeinfo>

using namespace std;

struct Entry{ // 键值对
    long long key; // 键（密文）
    long long value; // 值（明文）
    int next; // 存放下一 Entry 所在位置（在内存池 bucket 中的下标）
    Entry(): key(-1), value(-1), next(-1){} // 初始化
};

long long duplicate = 16983563041; // 存储的字符串转数值，最大为 18*19^0 + 18*19^1 + 18*19^2 + 18*19^3 + 18*19^4 + 18*19^5 + 18*19^6 + 18*19^7 = 16983563040

class HashTable{ // 哈希表
private:
    int M; // 存储空间
    int head[10000871]; // 存储链表表头
    int top; // 内存池栈顶指针
    Entry bucket[5000800]; // 内存池，初始 18^5 + 18^4 + 18^3 + 18^2 + 18^1 = 2000718，后续最多 1000000 * 3 = 3000000
    static size_t hashCode(long long i){ // 长长整数
        return (size_t)((i >> 32) + (int)i); 
    }
protected:
    int probe4Hit(const long long &key){ // 定位到以 key 为键（需要存储 key, value）的位置
        int headRank = hashCode(key) % M; // 以 head 的总存储长度取余
        int bucketRank = head[headRank];
        if(bucketRank == -1) return -1; // 表头为 -1
        else{
            while(bucket[bucketRank].next != -1){ // 下一 Entry 合法
                if(bucket[bucketRank].key != key) bucketRank = bucket[bucketRank].next; // 键没对上，查看下一合法 Entry
                else return bucketRank; // 键对上了，返回该位置
            }
            if(bucket[bucketRank].key != key) return -1; // 键没对上，没有下一合法 Entry，查询失败
            else return bucketRank; // 键对上了，返回该位置
        }
    }
    int probe4Free(const long long &key){ // 定位到以 key 为键（还没有存 key, value）的空位置
        int headRank = hashCode(key) % M; // 取余
        int bucketRank = head[headRank];
        if(bucketRank == -1){ // 表头为 -1
            head[headRank] = top; // 表头连上内存池中空位置
            top++; // 内存池开放位置（下一空位置）后移
            return (top - 1); // 返回该位置
        }
        else{
            while(bucket[bucketRank].next != -1) bucketRank = bucket[bucketRank].next; // 沿着链表顺序查找到结尾 Entry
            bucket[bucketRank].next = top; // 结尾 Entry 指向内存池中空位置
            top++; // 内存池栈顶指针后移
            return (top - 1); // 返回该位置
        }
    }
public:
    HashTable(){
        M = 10000871; // 表头总存储大小
        memset(head, -1, sizeof(head)); // 初始化为非法 -1
        top = 0; // 栈顶指针初始化为 0
    }
    bool put(long long key, long long value){ // 将键值对 (key, value) 加入 HashTable
        if(probe4Hit(key) != -1) return false; // 该 key 已存在，添加失败
        int bucketRank = probe4Free(key); // 定位到存储位置
        bucket[bucketRank].key = key; // 存储键值对，并令指向的下一 Entry 的地址为非法
        bucket[bucketRank].value = value;
        bucket[bucketRank].next = -1;
        return true;
    }
    bool markDuplicate(long long key, long long value){ // 标记为该 key 处 为重复的 Duplicate
        int bucketRank = probe4Hit(key);
        if(bucketRank == -1) return false; // 该 key 不存在，不重复
        if(bucket[bucketRank].value > duplicate) return false; // 该 key 处已经标记过重复了，无需重复标记
        if(bucket[bucketRank].value == value) return false; // 该 key 处存的 (key, value) 和 检查的 (key, value) 完全相同，不“重复”而是完全相同！
        bucket[bucketRank].value += duplicate; // 确实有重复的 (key, value1) 和 (key, value2)，其中 value1 != value2，标记重复
        return true;
    }
    long long* get(long long key){ // 获取 key 处的 value
        int bucketRank = probe4Hit(key); // 定位到 key 处的下标
        return (bucketRank == -1) ? NULL : &bucket[bucketRank].value; // 如果 key 不存在则返回 NULL 指针，若存在则返回 value 的引用
    }
};

// reference: https://github.com/WuYiMi/Myrepository/blob/master/DS/Queue.java
class Queue{ // 循环队列
private:
    char *queue;
    int front;
    int rear;
    int capacity;
public:
    Queue(int c = 9){ // 初始化
        capacity = c;
        front = 0;
        rear = 0;
        queue = new char[capacity];
    }
    ~Queue(){ // 释放内存
        delete [] queue;
    }
    void enqueue(int data){ // 入队
        if(full()) dequeue(); // 如果队列已满，把先前最早入队的元素弹出
        queue[rear] = data;
        rear = (rear + 1) % capacity;
    }
    int dequeue(){ // 出队，并返回出队元素
        int data;
        data = queue[front];
        front = (front + 1) % capacity;
        return data;
    }
    bool full(){ // 判断队列是否已满
        return ((rear + 1) % capacity == front);
    }
    int size(){ // 获取队列元素个数
        return ((rear - front + capacity) % capacity);
    }
    string output(){ // 将队列中元素返回到string中
        string ans = "";
        int t = front;
        if(front < rear){
            while(t < rear){
                ans += queue[t];
                t++;
            }
        }
        else{        
            while(t < capacity){
                ans += queue[t];
                t++;
            }
            t = 0;
            while(t < rear){
                ans += queue[t];
                t++;
            }
        }
        return ans;
    }
};

unsigned char salt[5]; // 存放salt
unsigned char allChar[20] = "x0123456789tsinghua"; // 字符集，首位x用来占位（占0的位置，以便后续计算）
long long power[9] = {1, 19, 361, 6859, 130321, 2476099, 47045881, 893871739, 16983563041}; // 提前算好 19 的幂次
HashTable* ht; // HashTable
Queue queue(9); // 大小为8的循环队列

void Hash(unsigned char * phrase, long long value){ // 将字符串phrase（数值对应为value）存储到HashTable中
    long long key = crc32(crc32(0, phrase, strlen((char *)phrase)), salt, strlen((char *)salt)); // 流式函数运算，等价于求 hash(phrase) = crc(phrase + salt)
    if(!ht->put(key, value)) ht->markDuplicate(key, value); // 如果存放失败，查看是否需要标记Duplicate
}

void generateChar(int total, int x, unsigned char * phrase, long long convert){ // 生成所有初始字符串的可能，total为最大位数，x为当前填充的位
    if(x == total) return; // 如果已经到达最大位数，停止填充并开始回溯
    for(int i = 1; i <= 18; i++){ // 遍历字符串集合中所有可能字符
        phrase[x] = allChar[i]; // 填充第x位
        convert += i * power[x]; // 字符串转19进制long long，便于后续value值的比较
        phrase[x + 1] = '\0';
        Hash(phrase, convert); // hash(phrase)
        generateChar(5, x + 1, phrase, convert); // 深搜下一位
        convert -= i * power[x]; // 回溯归位
        phrase[x] = '\0';
    }
}

void createHashTable(){ // 建立初始HashTable
    ht = new HashTable;
    unsigned char phrase[6] = "";
    generateChar(5, 0, phrase, 0); // 1~5位, 从首位开始，从空串开始，从对应19进制数值为0开始
}

string toString(long long num){ // 将19进制的数值，转换为对应的字符串
    string ans = "";
    while(num > 0){
        ans += allChar[num % 19];
        num /= 19;
    }
    return ans;
}

long long position(char x){ // 根据字符x在字符串集合allChar中的下标，返回其位置，以便于转19进制
    if(int('0') <= int(x) && int(x) <= int('9'))
        return ((long long)(x) - (long long)('0') + 1);
    switch(x){
    case 't': return 11;
    case 's': return 12;
    case 'i': return 13;
    case 'n': return 14;
    case 'g': return 15;
    case 'h': return 16;
    case 'u': return 17;
    case 'a': return 18;
    }
}

long long ToInt(unsigned char * phrase){ // 将字符串转19进制long long
    int len = strlen((char *)phrase);
    long long ans = 0;
    for(int i = 0; i < len; i++)
        ans += position(phrase[i]) * power[i];
    return ans;
}

void addHashTable(){ // 后续对HashTable添加新的Entry
    string nodeSet = queue.output(); // 获取字符集
    int len = nodeSet.size();
    for(int i = len; i >= 6; i--){ // 将长度大于5的Entry存入HashTable（因为长度为5及以下的字符串在处事创建HashTable时已经加入过）
        unsigned char phrase[9];
        nodeSet.copy((char *)phrase, i, len - i);
        *(phrase + i) = '\0';
        Hash(phrase, ToInt(phrase));
    }
}

void solve(int n){ // 对n个输入的密文进行解密
    for(int i = 0; i < n; i++){
        long long key;
        cin >> hex >> key;
        long long *pValue = ht->get(key);
        if(pValue){ // pValue合法（key存在）
            if(*pValue > duplicate) cout << "Duplicate" << endl; // 有多个明文
            else{ // 只有1个明文
                string ans = toString(*pValue); // 获取明文
                cout << ans << endl;
                queue.enqueue(ans[0]); // 将明文的首个字符入队
                if(queue.size() > 5) addHashTable(); // 如果队列中元素个数大于5个，则将新字符串加入HashTable
            } 
        }
        else cout << "No" << endl; // pValue不合法（该key不存在）
    }
}

int main(){
    int n;
    cin >> n >> salt; 
    createHashTable(); // 创建初始HashTable
    solve(n); // 输入n个密文进行解密
    return 0;
}