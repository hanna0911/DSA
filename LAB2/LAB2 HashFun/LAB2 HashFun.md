### LAB2 HashFun

1. **哈希策略**

   1. **哈希函数**

      1. **针对`ascii`字符串**：$hash(str) = \sum\limits_{i = 0}^{len - 1}{str[i] \cdot 128^{len - 1 - i}} \pmod N$，$str$为读入字符串，$len = strlen(str)$，$N$为哈希表大小
      3. **针对`utf-8`字符串**：
         $$
         hash(str) = 
         \sum\limits_{(i, j) = (0, 0)}^{(len - 1, size - 1)}
         \begin{cases}
         {toInt(str[i])} \\
         {toInt(str[i], str[i + 1])} \\
         {toInt(str[i], str[i + 1], str[i + 2])} 
         \end{cases}
         \cdot 65536^{size - 1 - j} \pmod N
         $$
         $size$为$str$中`utf-8`字符个数，`toInt`实现如下
         ```c++
         if((str[i] & 128) == 0) // 1 字节
           	toInt(str[i]) = str[i];
         else if((str[i]) & 224) == 192) // 2 字节
         		toInt(str[i], str[i + 1]) = ((str[i] & 31) << 6) + (str[i + 1] & 63);
         else // 3 字节
           	toInt(str[i], str[i + 1], str[i + 2]) = ((str[i] & 15) << 12) + ((str[i + 1] & 63) << 6) + (str[i + 2] & 63);
         ```
      
   2. **冲突处理**
   
      1. **双向平方试探**：
      
         增加成员变量并初始化
   
         ```c++
         void init(){
             square = 1; // 记录试探距离square^2
             negative = false; // 记录试探方向（正、负）
         }
         ```
         
         试探分正、反向
         
         ```c++
         int operator()(hash_entry* Table, int table_size, int last_choice){
             int ans;
             if(negative){ // 反向
                 ans = (last_choice - square * square) % table_size; // 减去square^2
                 if(ans < 0) ans += table_size;
                 square++; // 当前square两个方向都已完成
             }
             else // 正向
                 ans = (last_choice + square * square) % table_size; // 加上square^2
             negative = !negative; // 换向
             return ans;
         }
         ```
         
      2. **公共溢出区**：
      
         增加成员变量并初始化
      
         ```c++
         void init(){
             counter = 0; // 记录试探位置偏移量（试探位置为table_size + counter）
         }
         ```
         
         试探从公共溢出区开始，`table_size`取哈希表大小的$3/4$
         
         ```c++
         int operator()(hash_entry* Table, int table_size, int last_choice){
             int ans = table_size + counter; // 试探位置（公共溢出区起点 + 偏移量）
             counter++; // 下一试探位置
             return ans;
         }
         ```

2. **测试数据**

   | 数据 | 来源 | 数据规模与I/Q比例         | 分布              | 构造方法                                        |
   | ---- | ---- | ------------------------- | ----------------- | ----------------------------------------------- |
   | 1.in | POJ  | (I, Q) = (1000, 1000)     | 全部为ascii编码   | 借助`rand()` 随机选取数据并打乱插入、查询的顺序 |
   | 2.in | POJ  | (I, Q) = (300000, 300000) | 全部为ascii编码   | 借助`rand()` 随机选取数据并打乱插入、查询的顺序 |
   | 3.in | POJ  | (I, Q) = (500000, 100000) | 全部为ascii编码   | 借助`rand()` 随机选取数据并打乱插入、查询的顺序 |
   | 4.in | HDU  | (I, Q) = (1000, 1000)     | 尽可能多utf-8编码 | 借助`rand()` 随机选取数据并打乱插入、查询的顺序 |
   | 5.in | HDU  | (I, Q) = (300000, 300000) | 尽可能多utf-8编码 | 借助`rand()` 随机选取数据并打乱插入、查询的顺序 |
   | 6.in | HDU  | (I, Q) = (500000, 100000) | 尽可能多utf-8编码 | 借助`rand()` 随机选取数据并打乱插入、查询的顺序 |

3. **问题回答**

   1. 效果差。因为用针对ascii字符串的hashing function处理utf-8字符串，会将其映射到不均匀连续的空间上，产生局部数据聚集的情况，使得冲突增加。

   2. 双向平方试探性能好。因为产生冲突时，线性试探会让冲突数据聚集，而双向平方试探让冲突数据之间的分布距离相对均匀，避免数据聚集。

   3. 封闭散列更优。开放散列处理冲突较少的实际数据更适用。

   4. 造成局部数据堆积，冲突增多，插入与查询性能均变差。

   5. 数据例子为输入法的输入拼音（字符串）来查询对应的汉字（数字），多次查询会更新该拼音对应的汉字（词频改变）。

      优先级队列，根据数据访问频率维护偏序关系，查询次数更多的优先级更高；或伸展树，维护数据局部性，刚被访问过的节点移动到根，最后常被查询的数据集中在树的根节点附近。

      

