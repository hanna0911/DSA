#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

// reference: 邓俊辉《数据结构》02-G1 向量-位图：数据结构
class Bitmap{
private:
    int N;
    char * M;
public:
    Bitmap(int n = 8){
        M = new char[N = (n + 7) / 8];
        memset(M, 0, sizeof(M)); // 优化！用 sizeof(M) 大幅节省空间
    }
    ~Bitmap(){
        delete[] M;
        M = nullptr;
    }
    void set(int k){
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }
    void clear(int k){
        M[k >> 3] &= ~(0x80 >> (k & 0x07));
    }
    bool test(int k){
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }
};

int main(){
    // input
    Bitmap input((1 << 24) + 1);
    char a;
    int length = 0;
    while((a = getchar()) != '\n'){
        if(a == '1') input.set(length);
        length++;
    }

    // maxLen of answer
    int maxLen = 0; 
    int pro = 1;
    while(pro + maxLen - 1 <= length){
        pro *= 2;
        maxLen++;
    }
    
    // mark appeared segment
    Bitmap notepad((1 << 24) + 1);
    for(int curSegLen = 1; curSegLen <= maxLen; curSegLen++){
        int rank = 0; 
        int count = 0;
        int maxCount = 1 << curSegLen;
        // first segment
        int segment = 0;
        for(rank = 0; rank < curSegLen; rank++){
            segment += (input.test(rank) << (curSegLen - rank - 1));
        }
        if(notepad.test(segment) == 0){
            notepad.set(segment);
            count++;
        }
        // rest of the segments
        while(count < maxCount && rank < length){
            segment &= ((1 << (curSegLen - 1)) - 1);
            segment <<= 1;
            segment += input.test(rank);
            if(notepad.test(segment) == 0){
                notepad.set(segment);
                count++;
            }
            rank++;
        }

        // have answer
        if(count < maxCount){
            int ans = 0;
            while(notepad.test(ans) == 1) ans++;
            bool ansBinary[curSegLen + 1];
            memset(ansBinary, 0, sizeof(ansBinary));
            int t = 0;
            while(ans > 0){
                ansBinary[t] = ans % 2;
                t++;
                ans >>= 1;
            }
            for(int i = curSegLen - 1; i >= 0; i--)
                cout << ansBinary[i];
            cout << endl;
            break;
        }

        // clear notepad
        for(int rank = 0; rank < maxCount; rank++){
            notepad.clear(rank);
        }
    }

    return 0;
}