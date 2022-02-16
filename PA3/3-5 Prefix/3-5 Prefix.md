### 3-5 Prefix

1. **数据结构与算法**：KMP

   1. **构思**：若暴力匹配，则`n`个前缀每个都需比较`n-i`次（`i`为第`i`个前缀，即`prefix[i]`中的`i`），总时间复杂度为`O(n^2)`，对`1 <= n <= 20000000`的限制显然不现实。而考虑到KMP算法中Next数组的含义：可以使得`prefix[l]`作为`prefix[i]`的后缀的最长前缀；因此可以直接构造Next表来辅助求解答案。

   2. **原理**：KMP中Next数组

   3. **实现要点**：

      1. **构造Next表**：按照邓老师上课所讲述的KMP算法中，改进之前的Next表来构造，具体代码如下：

         ```c++
         // 邓俊辉《数据结构》KMP改进前Next表 (有所改动)
         void buildNext(){
             int j = 0, t = Next[0] = -1;
             while(j < n){ // 这里改成j<n (原来为j<n-1)
                 if(0 > t || S[j] == S[t]){
                     j++; t++; Next[j] = t;
                 }
                 else t = Next[t];
             }
         }
         ```

      2. **动态规划**：用动态规划计算所有前缀出现的次数之和，主要思路为计算每个以`S[i-1]`为结尾字符的`prefix` (即`prefix=S[:i]`) 有多少个。其中`status[i] = j`的定义为，存在`j`个`prefix`可以以`S[i-1]`为结尾字符, 其中`j`满足`Next^j[i] = 0`，`Next^2[i] = Next[Next[i]])`。递推公式为`status[i] = status[Next[i]] + 1`。具体详见代码：

         ```c++
         long long dp(){ // 动态规划计算以S[i-1]为结尾字符的prefix有多少个
             int status[n + 1]; // status[i]=j, 即存在j个prefix可以以S[i-1]为结尾字符, 其中j满足Next^j[i]=0 (Next^2[i]=Next[Next[i]])
             status[0] = 0; // 初始化
             long long sum = 0; // 累加所有前缀出现的次数之和, 即对status[i]求和
             for(int i = 1; i <= n; i++){
                 status[i] = status[Next[i]] + 1; // 递推公式
                 sum += status[i];
             }
             return sum;
         }
         ```

2. **问题**：以`S[i-1]`（即`S`中最后一位有效字符）为结尾字符的前缀个数在Next表中没有体现

   **解决**：Next表构造时，需要把`while`循环中的终止条件改为`j < n`（原来为`j < n - 1`），才能够计算以`S[i-1]`（即`S`中最后一位有效字符）为结尾字符的前缀个数（即此题中Next表的大小比原先KMP算法中的Next表大小要大一位（最后一位）。

3. **复杂度估算**：

   1. **时间复杂度**：`O(n)` 

      Next表的构造时间复杂度为`O(n)`，动态规划的时间复杂度为`O(n)`，因此总的时间复杂度为`O(2*n) = O(n)`。

   2. **空间复杂度**： `O(n)` 

      Next表以及动态规划中的状态数组均需`O(n)`的空间复杂度，因此总的空间复杂度为`O(2 * n) = O(n)`。

