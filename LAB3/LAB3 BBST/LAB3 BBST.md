### LAB3 BBST

1. **数据结构**

   1. **BST（共用）**

      1. **二叉树节点结构体**

         实现方式如下，详见`.cpp`中的代码：

         ```c++
         struct Node{    
             int data; // 存储的数据
             Node *parent, *lc, *rc; // 父亲、左孩子、右孩子
             int height; // 当前节点高度
             Node(): parent(NULL), lc(NULL), rc(NULL), height(0){} // 构造函数
             Node(int e, Node* p = NULL, Node* lc = NULL, Node* rc = NULL, int h = 0): data(e), parent(p), lc(lc), rc(rc), height(h){} // 构造函数
             Node* succ(); // 取当前节点的直接后继
             Node* pred(); // 取当前节点的直接前驱
         };
         ```

      2. **查找函数**

         ```c++
         Node* &search(const int & e){ // 查找关键码e
             if(!_root || (e == _root->data)){ // 空树，或恰在树根命中
               	_hot = NULL; 
               	return _root; 
             } 
             for(_hot = _root; ;){ // 自顶而下
               	Node* & v = (e < _hot->data) ? _hot->lc : _hot->rc; // 确定方向，深入一层
               	if(!v || (e == v->data)) return v; // 一旦命中或抵达叶子，随即返回
               	_hot = v; // _hot均指向v之父亲（v是根时，hot为NULL）
             } 
         }
         ```

         给定一个数据`e`，查找树中对应节点。若数据为e的节点存在，则返回该节点的指针引用 `Node* &`（便于后续直接对返回的节点做赋值等操作）；若数据为`e`的不存在，则返回其若存在所对应的节点位置的指针引用（为空）。该函数同时会更新其父节点`_hot`。

         实现过程：自顶`_root`而下，根据`e` 的大小不断确定其所在是左子树/右子树，逐层深入，直到命中或抵达叶子则查找停止。

         复杂度分析：时间复杂度为树的高度即`O(logn)`，空间复杂度为`O(1)`

      3. **节点高度更新函数**

         ```c++
         #define stature(p) ((p) ? (p)->height : -1) // BST中节点的高度（NUll视作空树，对应于-1）
         int updateHeight(Node* x){ // 更新节点x的高度
           	return x->height = 1 + max(stature(x->lc), stature(x->rc)); 
         }
         ```

         节点与其他节点的连接关系发生改变后（如插入、删除、旋转、伸展等）需要更新高度信息。

         实现过程：由节点高度定义可得。

         复杂度分析：时间、空间均为`O(1)`

   2. **AVL**

      1. **实现过程**

         基本操作接口如下，详见`.cpp`代码：

         ```c++
         static Node* removeAt(Node* & x, Node* & hot);
         class AVL{
         public:
             Node *_root, *_hot; // 根节点、“命中”节点的父亲
             int _size; // 规模
             AVL(): _size(0), _root(NULL){}
             int updateHeight(Node* x); // 更新节点x的高度
             Node* connect34(Node* a, Node* b, Node* c, Node* T0, Node* T1, Node* T2, Node* T3); // 按照“3 + 4”结构，联接3个节点及四棵子树
             Node* rotateAt(Node* v); // 对x及其父亲p、祖父g做统一旋转调整
             Node* &FromParentTo(Node &x); // 来自父亲的引用
             Node* &search(const int & e); // 查找关键码e
             Node* insert(const int & e);
             bool remove(const int & e);
             Node* labSearch(const int& e); // 找到满足data<=e的data最大节点
         };
         ```

         对外的接口为：`insert()`, `remove()`, `labSearch()`这三个函数，分别对应插入、删除、查找这三个操作。

      2. **复杂度分析**

         单次插入本身时间为`O(1)`，但首先需要通过查询来定位到插入的位置，而查询的时间复杂度为`O(logn)` ，因此单次时间复杂度为`O(logn)`；对`n`次插入操作而言，整体时间复杂度为`O(nlogn)` ；

         删除操作同样需要查询待删除节点位置，因此单次时间复杂度为`O(logn)`，整体时间复杂度为`O(nlogn)` ；

         以上两个操作（插入、删除）均有可能引起树的失衡，通过均摊时间复杂度`O(1)` 来进行旋转调整，因此不影响其单次`O(logn)`、总体`O(nlogn)`的时间复杂度。

         对外查询接口，与内部查询接口所需时间一致（多一步如果节点不存在，分类讨论返回其父节点或父节点的直接前驱），单次时间复杂度为`O(logn)`，整体时间复杂度为`O(nlogn)` 。

         以上三个操作空间复杂度均为 `O(1)` （就地）。

   3. **Splay**

      1. **实现过程**

         基本操作接口如下，详见`.cpp`代码：

         ```c++
         inline void attachAsLC(Node* lc, Node* p); // 在节点*p与*lc（可能为空）之间建立父（左）子关系
         inline void attachAsRC(Node* p, Node* rc); // 在节点*p与*rc（可能为空）之间建立父（右）子关系
         class Splay{
         public:
             Node *_root, *_hot; // 根节点、“命中”节点的父亲
             int _size; // 规模
             Splay(): _size(0), _root(NULL){}
             int updateHeight(Node* x); // 更新节点x的高度
             void updateHeightAbove(Node* x); // 更新高度
             Node *splay(Node *v); // 将节点v伸展至根
             Node* &search(const int & e); // 查找关键码e
             Node* &splaySearch(const int& e); //查找
             Node* labSearch(const int& e); // 找到满足data<=e的data最大节点
             Node* insert(const int& e); //插入
             bool remove(const int &e); //删除
         };
         ```

         对外的接口为：`insert()`, `remove()`, `labSearch()`这三个函数，分别对应插入、删除、查找这三个操作。

      2. **复杂度分析**

         对外查询接口，与内部查询接口`splaySearch()`所需时间一致（多一步如果节点不存在，分类讨论返回其父节点或父节点的直接前驱），而内部查询接口`splaySearch()`是公用BST的查询函数`search()`叠加伸展`splay()`函数，而`splay()`函数均摊时间复杂度为`O(logn)`，因此整体时间复杂度为`O(nlogn)` 。

         插入操作需要调用`splaySearch()`定位待插入位置，因此单次时间复杂度为`O(logn)`，整体时间复杂度为`O(nlogn)` ；

         删除操作需要调用`splaySearch()`定位待删除节点位置，因此单次时间复杂度为`O(logn)`，整体时间复杂度为`O(nlogn)` ；

         以上三个操作空间复杂度均为 `O(1)` （就地）。

2. **测例**

   1. **设计思路**

      因为两棵树的插入与删除操作较相似，此处数据只考虑插入与查询二者的不同；且均规定为先插入、后查询。

      主要比较的是AVL与Splay在数据特征不同的数据中的性能差别，因此这里插入与查询数量均一样多，只改变特征：

      | 文件名 | 插入A : 查询C           | 特征         |
      | ------ | ----------------------- | ------------ |
      | 0.in   | A : C = 500000 : 500000 | 随机数据     |
      | 1.in   | A : C = 500000 : 500000 | 顺序数据     |
      | 2.in   | A : C = 500000 : 500000 | 随机局部数据 |

   2. **生成方法**

      实现方式如下，详见`.cpp`中的代码：

      ```c++
      inline void randomData(){ // 随机数据
          srand((unsigned int)time(NULL));
          int n = 1000000; // 总操作数
          int A = 500000, C = 500000; // 插入、查询数量
          cout << n << endl;
          for(int i = 0; i < A; i++)
              cout << "A " << rand() % n << endl; // 用rand()随机生成插入数据
          for(int i = 0; i < C; i++)
              cout << "C " << rand() % n << endl; // 用rand()随机生成查询数据
      }
      
      inline void orderData(){ // 顺序数据
          int n = 1000000; // 总操作数
          int A = 500000, C = 500000; // 插入、查询数量
          cout << n << endl;
          for(int i = 1; i <= A; i++)
              cout << "A " << i << endl; // 从1～A顺序生存插入数据
          for(int i = 1; i <= C; i++)
              cout << "C " << i << endl; // 同样从1～A顺序生存插入数据
      }
      
      inline void localData(){ // 局部数据
          int n = 1000000; // 总操作数
          int A = 500000, C = 500000; // 插入、查询数量
          cout << n << endl;
          int k = 10; // 局部扰动区间大小
          for(int i = 0; i < A; i++)
              cout << "A " << rand() % n << endl; // 用rand()随机生成插入数据
          for(int i = 0; i < C; i++)
              cout << "C " << rand() % k << endl; // 用rand()随机生成查询数据，其范围为(0, k)区间内的局部数据
      }
      ```

3. **性能比较**

   1. **性能描述**

      （运行时注释掉输出数据的部分，以防止输出数据的时间对测评造成影响）运行时间的单位均为秒，具体数据如下（下表为一次数据运行后的结果，多次运行误差均不超过±0.1秒）：

      | 输入数据           | AVL时间（总体 : 插入 + 查询） | Splay时间（总体 : 插入 + 查询） |
      | ------------------ | ----------------------------- | ------------------------------- |
      | 0.in，随机数据     | 2.09 = 1.05 + 1.04            | 2.91 = 1.48 + 1.42              |
      | 1.in，顺序数据     | 1.55 = 0.81 + 0.74            | 1.47 = 0.73 + 0.75              |
      | 2.in，随机局部数据 | 1.54 = 1.07 + 0.48            | 1.97 = 1.54 + 0.44              |

      可以看出对于随机数据，AVL不论是插入还是查询的性能均显著优于Splay；

      对于顺序数据，Splay的插入性能比AVL好，查询性能基本一致，总体Splay性能更优；

      对于局部数据，AVL的插入性能比Splay好，查询性能Splay优于AVL；

   2. **原因分析**

      随机数据的局部性较差，AVL的平衡性使得树高始终较低，复杂度显著比缓存无法生效的Splay树要好；

      顺序数据局部性强，因此Splay的缓存策略可以起到作用，其总是在根节点附近插入、查询数据，相比需要每次稳定进行一定树高的单次操作复杂度的AVL反而更优；

      随机局部数据局部性较强，因此Splay的缓存策略可以起到一定的作用，其在根节点附近的一定范围内进行插入、查询，但AVL的平衡性使得AVL的单次操作复杂度稳定，因此总体性能还是AVL更优。