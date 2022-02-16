### 3-7 Sort

1. **数据结构与算法**：归并排序

   1. **构思**：由题意，若使用一般的排序（如二路归并），比较次数约在$n\log_2n$，对于最差的数据限制`n = 10^6`，`limit = 1.03 * 10^7`而言，需要比较`1.99 * 10^7`次，显然超出了`limit`的范畴。而题目所给比较器为三元比较器，首先会想到使用三路归并，比较次数约在$n\log_3n$，需要比较`1.26 * 10^7`次，也超出了限制。但上述三路归并只利用了比较器返回的`min`，而没有有效利用`max`，因此会想到扩展到四路归并，其比较次数约为$n\log_4n$，即比较`9.97 * 10^6`，满足要求，并且可以有效利用`max`：当前第`i`轮比较中为`max`的，在下一轮`i+1`轮中必定不参与三元比较（因为其注定不可能是最小值，至少还有第`i`轮比较中第二大的比它小）。

   2. **原理**：归并排序（四路归并）

   3. **实现要点**：

      1. **分而治之**：与二路归并一样，将区间`[lo, hi)`进行等分后再递归调用，递归基为单元素区间。不同的是二路归并做二等分，四路归并做四等分。

         ```c++
         void mergeSort(int * elem, int lo, int hi){
             if(hi - lo < 2) return; // 单元素区间自然有序
             int mi = (lo + hi) / 2, leftmi = (lo + mi) / 2, rightmi = (mi + hi) / 2; 
             // 对四等分后的4个部分分别排序
             mergeSort(elem, lo, leftmi); 
             mergeSort(elem, leftmi, mi);
             mergeSort(elem, mi, rightmi); 
             mergeSort(elem, rightmi, hi);
             merge(elem, lo, leftmi, mi, rightmi, hi); // 归并
         }
         ```

      2. **归并**：

         同二路归并一样，再各分区间都有序后，做归并。只不过对于四路归并，归并的是四个各自有序的区间（`[lo,leftmi)`， `[leftmi,mi)`，` [mi,rightmi)`，`[rightmi, hi)`）而非两个。四路归并时，除了第一个最小元素的选取，需要调用三元比较器2次，剩余元素的选取均只需要调用三元比较器1次（如构思中提到的同时利用`min`、`max`信息来做到）。

         在某一路元素耗尽后，四路归并退化为三路归并，以此类推（后退化为二路归并、乃至一路）。三路、二路每个元素的选取同样只需要调用三元比较器1次，具体分类讨论具体哪些路耗尽的情况较多，可详见代码，此处只给出大致的分类讨论框架：

         ```c++
         void merge(int * elem, int lo, int leftmi, int mi, int rightmi, int hi){ 
             // 分配内存提供给4路（B、C、D同理，此处只展示A）
             int la = leftmi - lo;
             int *A = new int[la];
             for(int i = lo; i < leftmi; i++) A[i - lo] = elem[i];
             int ei = lo, ai = 0; // 初始位置
             bool a = false; // 用于标记是否为“非最小元”
           	...
             // 四路归并
             if(ai < la && bi < lb && ci < lc && di < ld){
                 // 初始情况要把4个元素全部比较才能得到最小的元素
                 compare(A[ai], B[bi], C[ci], &max, &min);
                 compare(min, max, D[di], &max2, &min2);
                 // 4路中最小的放入elem，分类讨论较繁琐，此处省略
                 // 4路中最大的标记为“非最小元”，分类讨论较繁琐，此处省略
                 ...
               	// 其他情况只需在排除“非最小元”后的三个元素中选出最小的
                 while(ai < la && bi < lb && ci < lc && di < ld){ 
                     if(a){ // a是上次三元素比较中最大的, 即在此次选择中其一定为“非最小元”, 因此排除
                         a = false; // 复位
                         three_way_merge(elem, B, C, D, bi, ci, di, ei, b, c, d);
                     }
                     // 其他b、c、d同理
                   	...
                 }
               	// a路元素耗尽，退化为三路归并
                 if(ai >= la) mergeThree(elem, B, C, D, bi, lb, ci, lc, di, ld, ei, b, c, d); 
                 // 其他b、c、d同理
               	...
             }
           	// d路中无元素，退化为三路归并
             else if(ai < la && bi < lb && ci < lc) mergeThree(elem, A, B, C, ai, la, bi, lb, ci, lc, ei, a, b, c); 
             // 其他a、b、c同理 
           	...
             else{
               	// c,d路中无元素，退化为二路归并
                 if(ai < la && bi < lb) mergeTwo(elem, A, B, ai, la, bi, lb, ei); 
               	// 其他组合同理
               	...
             }
             // 释放内存，其他B、C、D同理
             delete [] A; 
           	...
         }
         ```

2. **问题**：用三路归并无法解决比较次数超出`limit`的问题

   **解决**：如前述所讲，最后拓展到四路归并才解决问题，有效利用了三元比较器返回的`max`值。

3. **复杂度估算**：

   1. **时间复杂度**：`O(nlogn)` 

      使用了归并排序（四路归并），时间复杂度为`O(nlogn)`。

   2. **空间复杂度**： `O(n)` 

      归并排序不是就地的，每次归并时都会为几个各自有序的区间总共分配约`O(n)`复杂度的内存，因此总空间复杂度为`O(n)`。

