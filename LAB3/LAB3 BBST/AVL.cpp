#include <iostream>

using namespace std;

// 邓俊辉《数据结构》reference: https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define stature(p) ((p) ? (p)->height : -1) // BST中节点的高度（NUll视作空树，对应于-1）
#define BalFac(x) (stature((x).lc) - stature((x).rc)) // 平衡因子
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2)) // AVL平衡条件
#define tallerChild(x) (stature((x)->lc) > stature((x)->rc) ? (x)->lc : ( /*左高*/ stature((x)->lc) < stature((x)->rc) ? (x)->rc : ( /*右高*/ IsLChild(* (x))? (x)->lc : (x)->rc /*等高：与父亲x同侧者（zig-zig或zag-zag）优先*/ )))

// 邓俊辉《数据结构》BinNode：
struct Node{    
    int data;
    Node *parent, *lc, *rc; 
    int height; 
    Node(): parent(NULL), lc(NULL), rc(NULL), height(0){}
    Node(int e, Node* p = NULL, Node* lc = NULL, Node* rc = NULL, int h = 0): data(e), parent(p), lc(lc), rc(rc), height(h){}
    Node* succ(){ // 取当前节点的直接后继
        Node* s = this; 
        if(rc){ // 若有右孩子：直接后继必在右子树中
           s = rc; // 右子树中
           while(HasLChild(*s)) s = s->lc; // 最靠左（最小）的节点
        }
        else{ // 当前节点所在左子树中的最低祖先
           while(IsRChild(*s)) s = s->parent; // 断朝左上方移动
           s = s->parent; // 朝右上方移动一步
        }
        return s;
    }
    Node* pred(){ // 取当前节点的直接前驱
        Node* s = this;
        if(lc){
            s = lc;
            while(HasRChild(*s)) s = s->rc;
        }
        else{
            while(IsLChild(*s)) s = s->parent;
            s = s->parent;
        }
        return s;
    }
};

// 邓俊辉《数据结构》BST：
static Node* removeAt(Node* & x, Node* & hot){
    Node* w = x; // 实际被摘除的节点
    Node* succ = NULL; // 实际被删除节点的接替者
    if(!HasLChild(*x)) succ = x = x->rc;  // *x的左子树为空：将*x替换为其右子树
    else if(!HasRChild(*x)) succ = x = x->lc; //右子树为空，对称处理（succ != NULL）   
    else{ // 左右子树均存在：选择x的直接后继删除
        w = w->succ(); // *x的直接后继*w
        swap(x->data, w->data); // 交换*x和*w的数据
        Node* u = w->parent;
        ((u == x) ? u->rc : u->lc) = succ = w->rc; //隔离节点*w
    }
    hot = w->parent; // 实际被删除节点的父亲
    if(succ) succ->parent = hot; // 将被删除节点的接替者与hot相联 
    delete w;
    return succ; // 返回接替者
} 

// 邓俊辉《数据结构》BinTree, BST, AVL：
class AVL{
public:
    Node *_root, *_hot; // 根节点、“命中”节点的父亲
    int _size; // 规模
    AVL(): _size(0), _root(NULL){}
    int updateHeight(Node* x){ // 更新节点x的高度
        return x->height = 1 + max(stature(x->lc), stature(x->rc)); 
    }
    Node* connect34(Node* a, Node* b, Node* c, Node* T0, Node* T1, Node* T2, Node* T3){ // 按照“3 + 4”结构，联接3个节点及四棵子树
        a->lc = T0; 
        if(T0) T0->parent = a;
        a->rc = T1; 
        if(T1) T1->parent = a; 
        updateHeight(a);
        c->lc = T2; 
        if(T2) T2->parent = c;
        c->rc = T3; 
        if(T3) T3->parent = c; 
        updateHeight(c);
        b->lc = a; 
        a->parent = b;
        b->rc = c; 
        c->parent = b; 
        updateHeight(b);
        return b; // 该子树新的根节点
    }
    Node* rotateAt(Node* v){ // 对x及其父亲p、祖父g做统一旋转调整
        Node* p = v->parent; 
        Node* g = p->parent; 
        // 视v、p和g相对位置分四种情况
        if(IsLChild(*p)){/* zig */
            if(IsLChild(*v)){ /* zig-zig */
                p->parent = g->parent; // 向上联接
                return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
            } 
            else{ /* zig-zag */
                v->parent = g->parent; // 向上联接
                return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
            }
        }
        else{  /* zag */
            if(IsRChild(*v)){ /* zag-zag */
                p->parent = g->parent; // 向上联接
                return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
            } 
            else{ /* zag-zig */
                v->parent = g->parent; // 向上联接
                return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
            }
        }
    }
    Node* &FromParentTo(Node &x){ // 来自父亲的引用
        if(IsRoot(x)) return _root;
        else{
            if(IsLChild(x)) return x.parent->lc;
            else return x.parent->rc;
        }
    }
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
    Node* insert(const int & e){
        Node* &x = search(e); 
        if(x) return x; // 确认目标节点不存在
        Node* xx = x = new Node(e, _hot); // 创建新节点x
        _size++; 
        // x的父亲_hot若增高，则其祖父有可能失衡
        for(Node* g = _hot; g; g = g->parent){ // 从x之父出发向上，逐层检查各代祖先g
            if(!AvlBalanced(*g)){ // g失衡：（采用“3 + 4”算法）使之复衡
                Node* &temp = FromParentTo(*g);
                temp = rotateAt(tallerChild(tallerChild(g))); // 将子树重新接入原树
                break; 
            } 
            else updateHeight(g);// g平衡：只需更新其高度  
        }
        return xx; // 返回新节点位置
    } 
    bool remove(const int & e){
        Node* &x = search(e); 
        if(!x) return false; // 确认目标存在
        removeAt(x, _hot); 
        _size--; 
        // 原节点之父_hot及其祖先均可能失衡
        for(Node* g = _hot; g; g = g->parent){ // 从_hot出发向上，逐层检查各代祖先g
            if(!AvlBalanced(*g)){ // g失衡：（采用“3 + 4”算法）使之复衡
                Node* &temp = FromParentTo(*g);
                g = rotateAt(tallerChild(tallerChild(g))); // 将该子树联至原父亲
                temp = g;
            }
            updateHeight(g); // 更新高度
        } 
        return true; 
    }
    Node* labSearch(const int& e){ // 找到满足data<=e的data最大节点
        if(!_root || e == _root->data){ // 空树，或恰在树根命中
            _hot = NULL; 
            return _root; 
        } 
        for(_hot = _root; ;){ // 自顶而下
            Node* & v =(e < _hot->data)? _hot->lc : _hot->rc; // 确定方向，深入一层
            if(!v || e == v->data){ // 命中或抵达叶子
                if(!v){ // 若抵达叶子
                    if(_hot->data < e) return _hot; // 该叶子是父节点_hot的右孩子：最大节点为_hot
                    else return _hot->pred(); // 该叶子是父节点_hot的左孩子：最大节点为_hot的直接前驱
                }
                return v; 
            }
            _hot = v; 
        }
    }
}*avlTree;

void input(int n){ // 读取n条指令
    char c;
    int x;
    for(int i = 0; i < n; i++){
        cin >> c >> x;
        switch (c){
            case 'A': // 插入
                avlTree->insert(x);
                break;
            case 'B': // 删除
                avlTree->remove(x);
                break;
            case 'C': // 查询
                Node* temp = avlTree->labSearch(x);
                cout << (temp ? temp->data : -1) << endl;
                break;
        }
    }
}

int main(){
    int n;
    avlTree = new AVL; // 建AVL树
    cin >> n;
    input(n); // 读取n条指令
    return 0;
}