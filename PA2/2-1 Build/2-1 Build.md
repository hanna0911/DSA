### 2-1 Build

1. **数据结构与算法**：二叉树

   1. **构思**：

      由题目可知每次子树移动的 `cost` 为待查询位置表示中的路径的路径长度及其中所有 `rank` 之和，即每次移动子树后更新高度、子树规模都只能向该节点的 `leftSibling` （后续均简化为 `left` ，同理 `rightSibling` 简化为 `right` ）和 `parent` 方向遍历节点，这里对于多叉树是 `left` 和 `parent` ，很自然的会想到对于二叉树而言都是 `parent` 节点，因此用二叉树的链表结构来建图（但要保留多叉树中是否为 `left` 或 `parent` 的信息）。

      且因为 `cost` 只允许向 `left` 和 `parent` 方向更新，即下一节点的更新只能依赖于已经更新过的节点 `right` 和 `child` ，因此不维护树每一节点的真实高度或真实子树规模，而是维护每层当前后缀中的最大高度、二叉树的子树规模。

   2. **原理**：二叉树遍历、高度与子树规模维护

   3. **实现要点**：

      1. **构建多叉树**：根据输入数据，将每一节点的首个孩子与其连接为 `connectPC(parent, child)` 关系，将剩余的孩子们两两连接为 `connectLR(left, right)` 关系；并创建哨兵节点 `invalid` ，让所有没有连接关系的节点指向哨兵节点。

      2. **初始化高度、子树规模**：用栈来模拟递归调用过程，从根节点开始，利用递推式 `max_height = max(child.max_height + 1, right.max_height)` 与 `binary_size = child.binary_size + right.binary_size + 1` 实现每个节点的初始化。

         值得强调的是，为了确保后续维护高度、子树规模最多消耗 `cost` 的复杂度，这里不维护树每一节点的真实高度或真实子树规模，而是维护每层当前后缀中的最大高度、二叉树的子树规模。如此一来，每一节点更新时二者均只依赖于节点的 `child` 和 `right` 这两个节点（详见前述递推式，更新时也以该式为准），获取二者时均只依赖于 `child` 这一个节点（均为 `child` 所存值 `+1` ，即 `height = child.max_height + 1` 与 `size = child.binary_size + 1` ）。

      3. **操作实现**：包含三个操作，即子树移动 `move()` 、获取节点高度 `getHeight()` 、获取子树规模 `getSize()` ，三个操作均通过 `getNode()` 定位到节点的 `rank` ，后分别执行各自的操作。其中子树移动操作需要在删除子树 `deleteTree()` 、添加子树 `moveTree()` 之后分别都更新 `update()` 当前树每一节点存储的高度、子树规模（具体 `update()` 思路即为前述初始化中所提思路）。

2. **问题与解决**：

   1. **问题1**：节点路径包含大量无效信息，不读入会覆盖后续操作指令，一个一个读入会时间超限

      **解决**：使用 `fgets` 快速读入该行剩余的无效信息

      ```c++
      // 清空该行
      void clear(int length){
          char *s = (char *)malloc((length * 8) * sizeof(char));
          fgets(s, length * 8, stdin);
          free(s);
      }
      ```

   2. **问题2**：极端情况可能 `2000000` 个节点间都是 `(parent, child)` 关系，树退化为链表结构，若用递归来初始化会导致爆栈

      **解决**：使用栈来模拟递归调用过程

      原本递归调用版本的旧代码：

      ```c++
      void initTree(int rank){
          if(tree[rank].child != invalid && tree[tree[rank].child].max_height == -1) initTree(tree[rank].child); // A: 若 child 节点存在且没更新，则递归调用更新
          if(tree[rank].right != invalid && tree[tree[rank].right].max_height == -1) initTree(tree[rank].right); // B: 若 right 节点存在且没更新，则递归调用更新
          tree[rank].max_height = max(tree[tree[rank].child].max_height + 1, tree[tree[rank].right].max_height); // C: 更新 max_height
          tree[rank].binary_size = 1 + tree[tree[rank].child].binary_size + tree[tree[rank].right].binary_size; // D: 更新 binary_size
          return; // E: 该节点的递归调用结束，返回
      }
      ```

      改为用栈来模拟递归的新代码：

      ```c++
      // 用栈来模仿递归调用过程
      void initTree(int n){
          Stack stack(n + 2); // 待更新节点所在栈
          stack.push(1);
          while(!stack.empty()){
              int node = stack.top();
            	// A: 若 child 节点存在且没更新，则入栈等待下一次出栈时更新
              if(tree[node].child != invalid && tree[tree[node].child].max_height == -1){
                  stack.push(tree[node].child);
                  continue;
              }
            	// B: 若 right 节点存在且没更新，则入栈等待下一次出栈时更新
              if(tree[node].right != invalid && tree[tree[node].right].max_height == -1){
                  stack.push(tree[node].right);
                  continue;
              }
              tree[node].max_height = max(tree[tree[node].child].max_height + 1, tree[tree[node].right].max_height); // C: 更新 max_height
              tree[node].binary_size = tree[tree[node].child].binary_size + tree[tree[node].right].binary_size + 1; // D: 更新 binary_size
              stack.pop(); // E: 已更新的节点出栈
          }
      }
      ```

      两代码中字母一样的代码块表示同样的功能，确保二者的等价性。

3. **复杂度估算**：

   1. **时间复杂度**：`O((N + M) * cost)` ，其中 `cost` 为节点表示中的路径的路径长度及其中所有`rank` 之和

      建树以及初始化复杂度为 `N * cost` ，执行 `M` 个操作的复杂度为 `M * cost ` ，其中 `cost` 为节点表示中的路径的路径长度及其中所有`rank` 之和；因此总时间复杂度为 `O((N + M) * cost)` 。

   2. **空间复杂度**： `O(N)` 

      多叉树节点个数为 `N` ，空间复杂度为 `O(N)` 。

