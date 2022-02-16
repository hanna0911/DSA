#include <iostream>
#include <cstring>

using namespace std;

int invalid; // 哨兵

// 多叉树节点
struct node{
    int parent;
    int child;
    int left;
    int right;
    int max_height; // 右缀最大高度
    int binary_size; // 二叉树的子树规模
    node(): parent(invalid), child(invalid), left(invalid), right(invalid), max_height(-1), binary_size(0){}
}*tree;

// array-like stack: https://blog.csdn.net/u012805073/article/details/101025491
class Stack{
private:
    int stackTop;
    int stackSize;
    int* stack;
public:
    Stack(int size){
        stackTop = -1;
        stackSize = size;
        stack = new int[stackSize];
    }
    bool empty(){
        return stackTop == -1;
    }
    int &top(){
        return stack[stackTop];
    }
    int &pop(){
        return stack[stackTop--];
    }
    void push(const int & x){
        stack[++stackTop] = x;
    }
    ~Stack(){
        delete[] stack;
    }
};

// 连接家长、孩子节点
void connectPC(int parent, int child){
    tree[parent].child = child;
    tree[child].parent = parent;
}

// 连接左、右节点
void connectLR(int left, int right){
    tree[left].right = right;
    tree[right].left = left;
}

// 用栈来模仿递归调用过程
void initTree(int n){
    Stack stack(n + 2);
    stack.push(1);
    while(!stack.empty()){
        int node = stack.top();   
        if(tree[node].child != invalid && tree[tree[node].child].max_height == -1){
            stack.push(tree[node].child);
            continue;
        }
        if(tree[node].right != invalid && tree[tree[node].right].max_height == -1){
            stack.push(tree[node].right);
            continue;
        }
        tree[node].max_height = max(tree[tree[node].child].max_height + 1, tree[tree[node].right].max_height);
        tree[node].binary_size = tree[tree[node].child].binary_size + tree[tree[node].right].binary_size + 1;
        stack.pop();
    }
}

// 建立多叉树
void createTree(int n){
    invalid = n + 1; // 初始化哨兵下标
    tree = new node[n + 3];
    for(int parent = 1; parent <= n; parent++){
        int childNum;
        cin >> childNum;
        if(childNum == 0) continue; // 1. childNum == 0
        else if(childNum == 1){ // 2. childNum == 1
            int child;
            cin >> child;
            connectPC(parent, child); // connect parent & child
        }
        else{ // 3. childNum >= 2
            int child[childNum + 1];
            cin >> child[0];
            connectPC(parent, child[0]);
            for(int childRank = 1; childRank < childNum; childRank++){
                cin >> child[childRank];
                connectLR(child[childRank - 1], child[childRank]); // connect left & right
            }
        }
    }
    initTree(n); // 初始化高度、子树规模
}

// 清空该行
void clear(int length){
    char *s = (char *)malloc((length * 8) * sizeof(char));
    fgets(s, length * 8, stdin);
    free(s);
}

// 根据路径获取节点下标
int getNode(){
    int layer, parent = 1, child, childNum;
    cin >> layer;
    while(layer > 0){ // layer: 1 ~ ...
        if(tree[parent].child == invalid){ // no child
            clear(layer);
            return parent;
        }
        child = tree[parent].child; // child[0]
        cin >> childNum;
        if(childNum < 0){ // 如果输入负数
            clear(layer);
            return parent;
        }
        while(childNum > 0){ // child[1 ~ (childNum - 1)]
            if(tree[child].right == invalid){
                clear(layer);
                return parent;
            }
            child = tree[child].right;
            childNum--;
        }
        parent = child;
        layer--;
    }
    return parent;
}

// 更新高度、子树规模
void update(int rank){
    tree[rank].max_height = max(tree[tree[rank].child].max_height + 1, tree[tree[rank].right].max_height);
    tree[rank].binary_size = tree[tree[rank].child].binary_size + tree[tree[rank].right].binary_size + 1;
    if(tree[rank].left == invalid && tree[rank].parent == invalid) return;
    if(tree[rank].left != invalid) return update(tree[rank].left);
    return update(tree[rank].parent);
}

// 删除子树
void deleteTree(int rank){
    int updateNode;
    if(tree[rank].parent != invalid){ // 1. have parent (no left)
        updateNode = tree[rank].parent;
        if(tree[rank].right == invalid){  // 1.1. no right
            tree[tree[rank].parent].child = invalid;
            tree[rank].parent = invalid;
        }
        else{ // 1.2. have right
            connectPC(tree[rank].parent, tree[rank].right);
            tree[rank].parent = invalid;
            tree[tree[rank].right].left = invalid;
            tree[rank].right = invalid;
        }
    }
    else{ // 2. have left (no parent)
        updateNode = tree[rank].left;
        if(tree[rank].right == invalid){ // 2.1. no right
            tree[tree[rank].left].right = invalid;
            tree[rank].left = invalid;
        }
        else{ // 2.2. have right
            connectLR(tree[rank].left, tree[rank].right);
            tree[rank].left = invalid;
            tree[rank].right = invalid;
        }
    }
    update(updateNode);
}

// 移动子树
void moveTree(int source, int destParent, int destChild){
    if(tree[destParent].child == invalid){ // 1. destParent have no child
        connectPC(destParent, source);
    }
    else{ // 2. destParent have child
        int child = tree[destParent].child;
        if(destChild == 0){ // 2.1. destChild is 0
            connectPC(destParent, source);
            tree[child].parent = invalid;
            connectLR(source, child);
        }
        else{ // 2.2. destChild > 0
            while(destChild > 0){
                if(tree[child].right == invalid) break;
                child = tree[child].right;
                destChild--;
            }
            if(destChild > 0) // 2.2.1. child[destChild] is invalid
                connectLR(child, source);
            else{ // 2.2.2. child[destChild] is valid
                connectLR(tree[child].left, source);
                connectLR(source, child);
            }
        }
    }
    update(source);
}

// 0: 移动操作
void move(){
    int source = getNode();
    deleteTree(source);
    int destParent = getNode();
    int destChild;
    cin >> destChild;
    moveTree(source, destParent, destChild);
}

// 1: 获取高度操作
int getHeight(){
    int node = getNode();
    return (tree[tree[node].child].max_height + 1);
}

// 2: 获取子树规模操作
int getSize(){
    int node = getNode();
    return (tree[tree[node].child].binary_size + 1);
}

// 读入 m 个操作
void getInstruct(int m){
    for(int i = 0; i < m; i++){
        int instruct;
        cin >> instruct;
        switch(instruct){
        case 0:
            move();
            break;
        case 1:
            cout << getHeight() << endl;
            break;
        case 2:
            cout << getSize() << endl;
            break;
        }
    }
}

int main(){
    int n, m;
    cin >> n >> m;
    createTree(n); // 建立多叉树
    getInstruct(m); // 读取指令
    delete[] tree;
    return 0;
}