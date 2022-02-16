### 1-2 Graphics

1. **数据结构与算法**：二分查找

   1. **构思**：

      已知 `x` 轴正半轴和 `y` 轴正半轴由 `n` 个不同点，一一连接后形成 `n` 条线段（两两不相交），对于给定点 `P(Px, Py)` ，要求 `OP` 与 `n` 条线段的交点数。因为这 `n` 条线段两两不相交，因此它们可以划分出 `n` 个区域（第 `n + 1` 个区域由哨兵划出）。

      可以立刻联想到这道题是要在 `n` 个线段框定的 `n` 个区域位置中，找到 `P` 点所在的位置（即第 `k` 条线段与第 `k + 1` 条线段之间），即可知 `OP` 与线段的交点数为 `k` 个。“找到所在位置”即“查找”，结合本章内容，二分查找最为合适。

   2. **原理**：有序向量的二分查找。

   3. **实现要点**：

      1. **输入**： `x` 轴正半轴和 `y` 轴正半轴各 `n` 个点，均以 `long long` 型数组格式输入。
      2. **排序**：因为二分查找是基于有序向量，所以先用 `cstdlib` 库的 `qsort` 排序，其中的 `compare` 参数返回待比较两数前者减去后者的值（正序排序）。
      3. **二分查找**：利用二分查找算法找到目标点 `P(Px, Py)` 在线段中的位置，即返回大于等于目标 `P(Px, Py)` 的坐标 `(x[i], y[i])` 的 `i` 值 (对同一个 `x` ，返回满足 `y[i] <= Py < y[i + 1]` 的 `i` 值)。

2. **问题与解决**：

   1. **问题1**：二分查找模板写法

      **解决**：对照数据结构讲义中的二分查找模板进行修改

      参考资料：邓俊辉 dsa-2021fall 有序向量 二分查找（版本C）

      二分查找（版本C）：

      ```c++
      template <typename T> static Rank binSearch (T* A, T const& e, Rank lo, Rank hi){
      		while(lo < hi){
      				Rank mi = (lo + hi) >> 1;
      				(e < A[mi]) ? hi = mi : lo = mi + 1;
      		}
      		return --lo;
      }
      ```

      根据实际题目进行修改：

      ```c++
      int binSearch(long long * x, long long * y, long long const & px, long long const & py, int lo, int hi){ // 数据存储类型微调
      		while(lo < hi){
        			int mi = (lo + hi) >> 1;
        			py * x[mi] < y[mi] * (x[mi] - px) ? hi = mi : lo = mi + 1; // 此处比较方法改为坐标是否在直线下方的比较
      		}
      		return lo; // 返回不高于P(Px, Py)的最大秩再加1（因为秩从0开始记录，因此计算个数时需要再加1）
      }
      ```

   2. **问题2**：排序算法模板写法

      **解决**：直接调用 ` cstdlib` 库的 `qsort` 排序

      参考资料：http://www.cplusplus.com/reference/cstdlib/qsort/

      ```c++
      qsort(x, n, sizeof(x[0]), compare);
      qsort(y, n, sizeof(y[0]), compare);
      ```

      ```c++
      int compare(const void * a, const void * b){
          return (*(long long*)a - *(long long*)b);
      }
      ```


3. **复杂度估算**：
   1. **时间复杂度**： `O((n + m) * logn)`
   
      对输入数据（线段的数量为 `n` ）进行排序，排序算法为 `cstdlib` 库中的 `qsort` 排序，其中以归并排序为主，时间复杂度为 `nlogn` ；再对 `m` 个询问进行二分查找，一次二分查找时间复杂度为 `logn` ，对于 `m` 个询问则为 `mlogn` ；因此程序总时间复杂度为 `O((n + m) * logn)` 。
   
   2. **空间复杂度**： `O(n)`
   
      归并排序的空间复杂度为 `n` ，程序中剩余部分均为就地处理，因此总的空间复杂度为 `O(n)` 。

