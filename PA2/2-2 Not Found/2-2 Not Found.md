### 2-2 Not Found

1. **数据结构与算法**：位图

   1. **构思**：

      由空间限制 `6 MB` 可知，最多开辟 `6 MB = 3 * 2^21 Byte` 内存空间，若用 `char *` 来读入数据规模为 `1 ≤ N (A 的长度) ≤ 2^24` 的字符串，则最多需要 `8 * 2^24 Byte` ，显然远超过空间限制。考虑到字符串只包含字符 `0` 和 `1` ，会想到用 `bool *` 来处理，但最多需要 `1 * 2^24 Byte` ，同样超过空间限制。

      既然想到可以用 `bool *` 来处理 `01` 字符串，自然会想到利用具有相同功能的 `Bitmap ` 来处理。其存储读入数据最多需要 `1 * 2^24 bit = 2^21 Byte` ，在空间限制范围内，满足题意。

   2. **原理**：位图存储，位图检验、设置、复位操作

   3. **实现要点**：

      1. **输入 `01` 串并构建Bitmap**：创建一个大小为 `2^24 + 1` 位的 `Bitmap input` ，通过 `getchar()` 来获取单个字符，若第 `length` 个字符为 `1` 则将 `input` 对应第 `length` 位设置为 `1` （ `input` 初始化全为 `0` ），直到读入 `\n` 。
      2. **计算答案最长长度**：若 `01` 串的长度为 `length` ，则答案的长度 `m` 必定满足不等式 `length < 2^(m + 1) + m ` ，（反正法）否则若 `length >= 2^(m + 1) + m` ，则 `01` 串中最多找到 `2^(m + 1)` 个不同位置的长度为 `m + 1` 的子串，而长度为 `m + 1` 的不同的 `01` 子串一共有仅有 `2^(m + 1)` 个，根据抽屉原理，必定有至少一个长度为 `m + 1` 的 `01` 子串没有出现过，因此答案的长度应至少为 `m + 1` 而不是 `m` ，与假设矛盾。由此计算出答案的最长长度 `maxLen` 。
      3. **暴力枚举候选答案**：把最短答案从 `1` 枚举到 `maxLen` ，当候选答案长度为 `curSegLen` 时，最多枚举 `2^curSegLen` 种，因此必定不会超过 `length` 个，同样开一个大小同样为 `2^24 + 1` 的 `Bitmap notepad` ，用来记录候选答案 `01` 串的十进制数值（将原 `01` 串看作二进制的数值）为 `rank` 的候选答案是否出现过。其中候选答案长度为 `i` 时，因为不同的长度为 `i` 的 `01` 串最多有 `2^i` 个，因此可以设置一个计数器 `count` ，当累计查询到的长度为 `i` 的候选答案个数 `count` 超过 `maxCount = 2^i` 时，直接查看下一长度的候选答案，并重置 `notepad` 标记情况；若枚举完 `count < maxCount` 则说明答案长度为 `i` ，停止枚举并在 `notepad` 中找到最小未被标记的下标并转二进制输出。

2. **问题**：空间超限

   **解决**：`Bitmap` 构造函数中，初始化 `char * M` 时，将 `memset(M, 0, N)` 修改为 `memset(M, 0, sizeof(M))` ，大量节省空间。

3. **复杂度估算**：

   1. **时间复杂度**：`O(nlogn)` ，其中 `n` 为输入字串 `A` 的长度

      枚举答案时外层循环答案长度为 `logn` ，内层循环遍历输入 `01` 串最多为 `n` 次，因此总时间复杂度为 `O(nlogn)` 。

   2. **空间复杂度**： `O(n)` ，其中 `n` 为输入字串 `A` 的长度

      输入字串 `A` 的长度为 `n` ，`Bitmap input` 与 `Bitmap notepad` 大小均 `O(n)` ，总空间复杂度为 `O(n)` 。
