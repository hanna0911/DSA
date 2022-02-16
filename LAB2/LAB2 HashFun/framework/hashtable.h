#include <cstring>
#include <cstdio>
#include <iostream>

const int TABLE_SIZE = 600011; // 499883 = 124970 * 4 + 3, 1000039, 608299, 500083, 600011
struct hash_entry{
    char* my_string;
    int my_data; 
    hash_entry(){
        my_string = NULL;my_data = 0;
    }
    hash_entry(char* str, int data):my_data(data){
        my_string = new char[strlen(str)+1];
        strcpy(my_string,str);
    }
};
struct hashing_strategy{
    virtual int operator()(char* str, int N)=0;
};
struct naive_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};
struct ascii_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};
struct utf_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};

struct collision_strategy{
    virtual void init()=0;// pure virtual function
    virtual int operator()(hash_entry* Table, int table_size, int last_choice)=0;
};
struct linear_probe: public collision_strategy{
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
struct twoSquare_probe: public collision_strategy{
    int square;
    bool negative;
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
struct overflow_probe: public collision_strategy{
    int counter;
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};

struct hashtable{
    hash_entry* Table;
    int table_size;
    hashing_strategy* my_hashing; // 如果改为hashing_strategy my_hashing, 即不是用指针作为hashtable的成员, 而是让hashing_strategy结构体直接作为hashtable的成员. 会发生什么bug?
    collision_strategy* my_collision;
    hashtable(int size, hashing_strategy* hashing, collision_strategy* collision)
        :table_size(size),my_hashing(hashing),my_collision(collision)
    {
        Table = new hash_entry[table_size];
    }
    bool insert(hash_entry entry){ 
        int last_choice;
        overflow_probe* pObj = dynamic_cast<overflow_probe*>(my_collision);
        if(pObj != nullptr){ // overflow_probe
            last_choice = (*my_hashing)(entry.my_string,table_size*3/4);
            my_collision->init();
            while(Table[last_choice].my_string!=NULL){ // loop infinitely? return false when no more space?
                last_choice = (*my_collision)(Table, table_size*3/4, last_choice);
            }
            Table[last_choice] = entry;
            return true;
        }
        else{
            last_choice = (*my_hashing)(entry.my_string,table_size);
            my_collision->init();
            while(Table[last_choice].my_string!=NULL){ // loop infinitely? return false when no more space?
                last_choice = (*my_collision)(Table, table_size, last_choice);
            }
            Table[last_choice] = entry;
            return true;
        }
    }
    int query(char* query_string){
        int last_choice;
        overflow_probe* pObj = dynamic_cast<overflow_probe*>(my_collision);
        if(pObj != nullptr){ // overflow_probe
            last_choice = (*my_hashing)(query_string,table_size*3/4);
            my_collision->init();
            while(Table[last_choice].my_string!=NULL && 
                strcmp(Table[last_choice].my_string, query_string)!=0){ // 未处理的情况: 哈希表已满?
                last_choice = (*my_collision)(Table, table_size*3/4, last_choice);
            }
            if(Table[last_choice].my_string == NULL){
                return -1;
            }
            else {
                return Table[last_choice].my_data;
            }
        }
        else{
            last_choice = (*my_hashing)(query_string,table_size);
            my_collision->init();
            while(Table[last_choice].my_string!=NULL && 
                strcmp(Table[last_choice].my_string, query_string)!=0){ // 未处理的情况: 哈希表已满?
                last_choice = (*my_collision)(Table, table_size, last_choice);
            }
            if(Table[last_choice].my_string == NULL){
                return -1;
            }
            else {
                return Table[last_choice].my_data;
            }
        }
    }
};
