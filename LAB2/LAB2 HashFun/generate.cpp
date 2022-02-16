#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct User{
    char ID[1000];
    int rank;
    bool inserted;
}dataset[700000];

int main(int argc, char* argv[]){

    std::string datafile_temp = argv[1];
    const char *datafile = ("dataset/" + datafile_temp + ".txt").c_str(); // 注意dataset文件夹的位置！需要根据实际位置调整读取文件的路径！
    freopen(datafile, "r", stdin);

    int unused, users = 0;
    while(cin >> unused){
        cin >> dataset[users].ID >> dataset[users].rank >> unused;
        dataset[users].inserted = false;
        users++;
    }

    int insert = atoi(argv[2]); // 测试数据中插入操作的次数
    int query = atoi(argv[3]); // 查询操作的次数

    string filename_temp = argv[4];
    const char *filename = (filename_temp + ".in").c_str();
    freopen(filename, "w", stdout);

    srand((unsigned int)time(NULL));
    int pos, operate;
    while(insert + query > 0){
        pos = rand() % users;
        operate = rand() % 2;
        if((operate == 0 && insert > 0) || query <= 0){
		    while(dataset[pos].inserted) pos = rand() % users;
            cout << 0 << " " << dataset[pos].ID << " " << dataset[pos].rank << endl;
            dataset[pos].inserted = true;
            insert--;
        }
        else{
            cout << 1 << " " << dataset[pos].ID << endl;
            query--;
        }
    }
    cout << 2 << endl;

    fclose(stdin);
    fclose(stdout);
    return 0;
}