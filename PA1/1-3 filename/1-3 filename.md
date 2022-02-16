### 1-3 filename

1. **数据结构与算法**：动态规划

   1. **构思**：

      该题输入 `2` 个串，原始串 `A` 和目标串 `B` ，并对其最小修改（插入、删除）次数进行统计。最小修改次数的反面即是公共部分的最长长度，于是马上想到最长公共子序列，即使用动态规划算法比对 `2` 个串的最长公共部分。

      通过举例分析与数学推导，果然发现最小修改次数 `k` 与最长公共子序列长度 `s` 之间存在等式 `k = m + n - 2 * s` ，其中 `m` ,  `n` 分别为 `2` 个串的长度。

   2. **原理**：动态规划（当前状态只与前一状态有关，列出状态转移方程即可）。

   3. **实现要点**：

      1. **输入**：以字符串形式输入 `2` 个串。
      2. **状态矩阵初始化**：全部置为 `0` 。
      3. **动态规划**：当原始串当前字符与目标串相同时，状态转移方程为 `status[current_i][current_j] = max(status[current_i][previous_j], status[previous_i][current_j], status[previous_i][previous_j] + 1)` ；当原始串当前字符与目标串不同时，状态转移方程为 `status[current_i][current_j] = max(status[current_i][previous_j], status[previous_i][current_j])` 。
      4. **计算修改次数**：最小修改次数 `k` 与最长公共子序列长度 `s` 之间存在等式 `k = m + n - 2 * s` ，其中 `m` ,  `n` 分别为 `2` 个串的长度， `s` 为状态转移矩阵 `status` 最后一次更新的值。

2. **问题与解决**：

   1. **问题1**：空间超限

      **解决**：将状态转移矩阵由 `m` 行变为 `2` 行

      ```c++
      for(int i = 1; i <= m; i++){
          swap(previous, current); // 记录当前行与上一行的参数需交换
          for(int j = 1; j <= n; j++){ // 此版本此处时间复杂度还未优化
              if(old[j - 1] == goal[i - 1]){
                  // st[i][j] = max_of_three(st[i][j - 1], st[i - 1][j], st[i - 1][j - 1] + 1);
                  st[current][j] = max_of_three(st[current][j - 1], st[previous][j], st[previous][j - 1] + 1);
              }
              else{
                  // st[i][j] = max_of_two(st[i][j - 1], st[i - 1][j]);
                  st[current][j] = max_of_two(st[current][j - 1], st[previous][j]);
              }
          }
      }
      ```

      由上述代码（注释版本为优化前）可看出该动态规划部分的空间复杂度由 `O(n * m)` 降为 `O(n)` 。

   2. **问题2**：时间超限

      **解决**：动态规划的状态更新由一整行变为对角线处偏移 `2k` 的部分元素

      ```c++
      for(int i = 1; i <= m; i++){
          swap(previous, current); 
          // for(int j = 1; j <= n; j++){
        	for(int j = max_of_two(1, i - k); j <= min_of_two(i + k, n); j++){ // 只需要更新对角线处 ± k，注意上下均不要越界
              if(old[j - 1] == goal[i - 1])
                  st[current][j] = max_of_three(st[current][j - 1], st[previous][j], st[previous][j - 1] + 1);
              else st[current][j] = max_of_two(st[current][j - 1], st[previous][j]);
          }
      }
      ```

      由上述代码（注释版本为优化前）可看出该动态规划部分的时间复杂度由 `O(n * m)` 降为 `O(k * m)` ；已知 `n, m ≤ 501000` , `k ≤ 100` ，即极端情况下 `n` 的大小远大于 `k` 。

3. **复杂度估算**：

   1. **时间复杂度**： `O(n + k * m)` 
   
      状态矩阵的初始化时间复杂度为 `n` ，动态规划的时间复杂度为 `k * m` ，因此总时间复杂度为 `O(m + k * m)` 。
   
   2. **空间复杂度**： `O(n)` 
   
      状态矩阵的空间复杂度为 `n` ，其他部分均为就地处理，因此总空间复杂度为 `O(n)` 。

