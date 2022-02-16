#include <iostream>
#include <cstring>

using namespace std;

char S[20000002];
int n = 0;
int *Next;

// 邓俊辉《数据结构》KMP改进前Next表 (有所改动)
void buildNext(){
    int j = 0;
    int t = Next[0] = -1;
    while(j < n){ // 这里改成j<n (原来为j<n-1)
        if(0 > t || S[j] == S[t]){
            j++;
            t++;
            Next[j] = t;
        }
        else t = Next[t];
    }
}

void input(){ // 输入字符串
    fgets(S, 20000002, stdin);
    n = strlen(S) - 1;
    S[n] = '\0';
}

long long dp(){ // 动态规划计算每个以S[i-1]为结尾字符的prefix (即prefix=S[:i]) 有多少个
    int status[n + 1]; // status[i]=j, 即存在j个prefix可以以S[i-1]为结尾字符, 其中j满足Next^j[i]=0 (Next^2[i]=Next[Next[i]])
    status[0] = 0; // 初始化
    long long sum = 0; // 累加所有前缀出现的次数之和, 即对status[i]求和
    for(int i = 1; i <= n; i++){
        status[i] = status[Next[i]] + 1; // 递推公式
        sum += status[i];
    }
    return sum;
}

int main(){
    input(); // 输入
    Next = new int[n + 1];
    memset(Next, 0, n + 1);
    buildNext(); // 建立KMP的Next表
    cout << dp() << endl; // 动态规划计算所有前缀出现的次数之和
    delete [] Next;
    return 0;
}