//
//  main.cpp
//  CST2021F 1-6 ⑨的完美文本编辑器 new
//
//  Created by Hanna Wang on 2021/10/8.
//  Copyright © 2021 Hanna Wang. All rights reserved.
//

// 更新1: 使用2个双向链表, reverse 时间复杂度降为 O(1)
// 参考资料: 《数据结构》-CST 2021 第一次习题课，用2个链表实现O(1)时间复杂度的逆序操作

// 更新2: 输入输出用 fgets, getchar, putchar 优化

#include <iostream>
#include <cstring>

using namespace std;

char text[3200002]; // 初始文本

// 2个双向链表，共用以下数组
char content[14400004]; // 存储链表节点
int nextNode[14400004]; // 存储下一节点
int prevNode[14400004]; // 存储上一节点
int base = 7200002; // 第2个双向链表 list 2 在数组中的偏移量

// 封装双向链表
class linkedList{
public:
    int head;
    int top;
    int leftPointer;
    int rightPointer;
    void init(int t_head, int t_top){ // 初始化链表
        head = t_head;
        top = t_top;
    }
    void insertAfter(int pos, char x){ // 在 pos 处后面的节点位置处插入字符 x 
        if(pos >= 0){
            content[top] = x;
            nextNode[top] = nextNode[pos];
            prevNode[top] = pos;
            prevNode[nextNode[pos]] = top;
            nextNode[pos] = top;
            top++;
        }
        else{ // pos == -1 即 插入位置在首位 (插入在 header 的后面) 的特殊情况
            content[top] = x;
            nextNode[top] = head; 
            prevNode[top] = prevNode[head];
            prevNode[head] = top;
            head = top;
            top++;
        }
    }
    void insertFirst(char x){ // 在首位插入 x
        content[top] = x;
        nextNode[top] = head;
        prevNode[top] = prevNode[head];
        prevNode[head] = top;
        head = top;
        top++;
    }
    void Delete(int pos){ // 删除 pos 处的下一节点
        nextNode[pos] = nextNode[nextNode[pos]];
        prevNode[nextNode[pos]] = pos;
    }
}list1, list2; // 2个双向链表

int header, trailer; // list 1 的哨兵位置 ( list 2 的哨兵位置要加上偏移量 base )
bool pointerOverlap = false; // 能否进行 reverse 操作 (即 left & right pointer 是否重合或逆序)

void createLinkedList(){
    int len = strlen(text);
    
    // 初始化链表
    list1.init(-1, 0);
    list2.init(base - 1, base);
    
    // 插入 list 1 的 header 'H' (list 2 的 trailer)
    list1.insertAfter(list1.top - 1, 'H'); 
    list2.insertFirst('h'); 
    header = 0;
    
    // 输入初始序列 text
    for(int i = 0; i < len; i++){
        list1.insertAfter(list1.top - 1, text[i]);
        list2.insertFirst(text[i]);
    }
    
    // 插入 list 1 的 trailer 'T' (list 2 的 header)
    list1.insertAfter(list1.top - 1, 'T'); 
    list2.insertFirst('t'); 
    trailer = list1.top - 1;
    
    // 设置 left & right pointer，均在该位置的右侧 (例. 12[34]5 中 left 和 2 位置一致，right 和 4 位置一致)
    list1.leftPointer = nextNode[list1.head] - 1;
    list1.rightPointer = list1.top - 2;
    list2.leftPointer = list1.leftPointer + base + 1; // list 2 考虑偏移量 base
    list2.rightPointer = list1.rightPointer + base + 1; 
    
}

void moveLeft(char pointer){
    switch(pointer){
        case 'L':
            if(nextNode[list2.leftPointer] != base + header){ // leftpointer2 的后一个 (leftpointer1) 不是 header
                list2.leftPointer = nextNode[list2.leftPointer];
                list1.leftPointer = prevNode[list1.leftPointer];
                putchar('T');
            }
            else{ // leftpointer2 的后一个是 header
                putchar('F');
            }
            putchar('\n');
            break;
        case 'R':
            if(nextNode[list2.rightPointer] != base + header){ // rightpointer2 的后一个 (rightpointer1) 不是 header
                list2.rightPointer = nextNode[list2.rightPointer];
                list1.rightPointer = prevNode[list1.rightPointer];
                putchar('T');
            }
            else{ // rightpointer2 的后一个是 header
                putchar('F');
            }
            putchar('\n');
            break;
    }
}

void moveRight(char pointer){
    switch(pointer){
        case 'L':
            if(nextNode[list1.leftPointer] != trailer){ // leftpointer1 的后一个 (leftpointer2) 不是 trailer
                list1.leftPointer = nextNode[list1.leftPointer];
                list2.leftPointer = prevNode[list2.leftPointer];
                putchar('T');
            }
            else{ // leftpointer1 的后一个是 trailer
                putchar('F');
            }
            putchar('\n');            
            break;
        case 'R':
            if(nextNode[list1.rightPointer] != trailer){ // rightpointer1 的后一个 (rightpointer2) 不是 trailer
                list1.rightPointer = nextNode[list1.rightPointer];
                list2.rightPointer = prevNode[list2.rightPointer];
                putchar('T');
            }
            else{ // rightpointer1 的后一个是 trailer
                putchar('F');
            }
            putchar('\n');
            break;
    }
}


void insert(char pointer, char character){
    switch(pointer){
        case 'L':
            list1.insertAfter(list1.leftPointer, character); 
            if(list1.leftPointer != list1.rightPointer) // 两光标不重合
                list1.leftPointer = nextNode[list1.leftPointer]; 
            else{ // [], 两光标重合
                list1.rightPointer = nextNode[list1.rightPointer]; 
                list1.leftPointer = nextNode[list1.leftPointer]; 
            }
            list2.insertAfter(list2.leftPointer, character); 
            break;
        case 'R':
            if(list1.leftPointer != list1.rightPointer){ // 两光标不重合
                list1.insertAfter(list1.rightPointer, character);
                list1.rightPointer = nextNode[list1.rightPointer];
                list2.insertAfter(list2.rightPointer, character);
                break;
            }
            else{ // [], 两光标重合
                list1.insertAfter(list1.rightPointer, character);
                list1.rightPointer = nextNode[list1.rightPointer];
                list2.insertAfter(list2.rightPointer, character);
                list1.leftPointer = nextNode[list1.leftPointer];
                break;
            }
    }
    putchar('T');
    putchar('\n');
}

void remove(char pointer){
    switch(pointer){
        case 'L':
            if(nextNode[list1.leftPointer] != trailer){
                if(list1.leftPointer != list1.rightPointer){
                    if(nextNode[list1.leftPointer] == list1.rightPointer){ // [i], leftpointer 待删除处有 rightpointer
                        list1.rightPointer = prevNode[list1.rightPointer];
                        list1.Delete(list1.leftPointer);
                        list2.leftPointer = prevNode[list2.leftPointer];
                        list2.Delete(list2.leftPointer);
                    }
                    else{ // 普通情况
                        list1.Delete(list1.leftPointer);
                        list2.leftPointer = prevNode[list2.leftPointer];
                        list2.Delete(list2.leftPointer);
                    }
                }
                else{ // []i, leftpointer == rightpointer
                    list1.Delete(list1.leftPointer);
                    list2.leftPointer = prevNode[list2.leftPointer];
                    list2.rightPointer = prevNode[list2.rightPointer];
                    list2.Delete(list2.leftPointer);
                }
                putchar('T');
            }
            else{
                putchar('F');
            }
            putchar('\n');
            break;
        case 'R': 
            if(nextNode[list1.rightPointer] != trailer){
                if(list1.rightPointer != list1.leftPointer){
                    if(nextNode[list1.rightPointer] == list1.leftPointer){ // ]i[, rightpointer 待删除处有 leftpointer
                        list1.leftPointer = prevNode[list1.leftPointer];
                        list1.Delete(list1.rightPointer);
                        list2.rightPointer = prevNode[list2.rightPointer];
                        list2.Delete(list2.rightPointer);
                    }
                    else{ // 普通情况
                        list1.Delete(list1.rightPointer);
                        list2.rightPointer = prevNode[list2.rightPointer];
                        list2.Delete(list2.rightPointer);
                    }
                }
                else{ // rightpointer == leftpointer
                    list1.Delete(list1.rightPointer);
                    list2.rightPointer = prevNode[list2.rightPointer];
                    list2.leftPointer = prevNode[list2.leftPointer];
                    list2.Delete(list2.rightPointer);
                }
                putchar('T');
            }
            else{
                putchar('F');
            }
            putchar('\n');
            break;
    }
}

void reverse(){
    if(pointerOverlap){ // 光标位置不合法
        putchar('F');
    }
    else{ // 光标位置合法，可以执行reverse
        
        int temp1 = nextNode[list1.leftPointer], temp2 = nextNode[list2.leftPointer];
        
        // 更新 list 1
        nextNode[list1.leftPointer] = nextNode[list2.rightPointer];
        prevNode[nextNode[list2.rightPointer]] = list1.leftPointer;
        nextNode[list2.leftPointer] = nextNode[list1.rightPointer];
        prevNode[nextNode[list1.rightPointer]] = list2.leftPointer;
    
        // 更新 list 2
        nextNode[list2.rightPointer] = temp1;
        prevNode[temp1] = list2.rightPointer; 
        nextNode[list1.rightPointer] = temp2;
        prevNode[temp2] = list1.rightPointer;
    
        // 交换 pointers
        swap(list1.rightPointer, list2.leftPointer);
        
        putchar('T');
    }
    putchar('\n');
}

void show(){
    // 按 list 1 的顺序输出
    int t = nextNode[list1.head];
    while(t != trailer){
        putchar(content[t]);
        t = nextNode[t];
    }
    putchar('\n');
}

void checkIfCanReverse(){ // 查看光标是否合法，是否可以执行 reverse
    if(list1.leftPointer == list1.rightPointer){
        pointerOverlap = true;
    }
    else if(nextNode[list1.leftPointer] == list1.rightPointer){
        pointerOverlap = false;
    }
    else if(nextNode[list1.rightPointer] == list1.leftPointer){
        pointerOverlap = true;
    }
}

void readInstruct(char instruct){
    checkIfCanReverse(); // 更新是否可以执行 reverse 的状态信息 pointerOverlap
    char pointer, character;
    switch(instruct){
        case '<': // < (move left)
            pointer = getchar();
            getchar();
            moveLeft(pointer);
            break;
        case '>': // > (move right)
            pointer = getchar();
            getchar();
            moveRight(pointer);
            break;
        case 'I': // I (insert)
            pointer = getchar();
            getchar();
            character = getchar();
            getchar();
            insert(pointer, character);
            break;
        case 'D': // D (delete)
            pointer = getchar();
            getchar();
            remove(pointer);
            break;
        case 'R': // R (reverse)
            reverse();
            break;
        case 'S': // S (show)
            show();
            break;
    }
}

int main(int argc, const char * argv[]) {
    
    fgets(text, sizeof(text), stdin); // 输入初始文本
    text[strlen(text) - 1] = '\0';
    createLinkedList(); // 创建2个双向链表

    int n = 0;
    scanf("%d", &n);
    getchar();

    for(int i = 0; i < n; i++){
        char instruct;
        instruct = getchar();
        getchar();
        readInstruct(instruct); // 读取指令
    }

    return 0;
}

