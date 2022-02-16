#include <iostream>

using namespace std;

// 邓俊辉《数据结构》reference: https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define stature(p) ((p) ? (p)->height : -1) // BST中节点的高度（NUll视作空树，对应于-1）

// 邓俊辉《数据结构》BinNode：
struct Node{    
    int data;
    Node *parent, *lc, *rc; 
    int height; 
    Node(): parent(NULL), lc(NULL), rc(NULL), height(0){}
    Node(int e, Node* p = NULL, Node* lc = NULL, Node* rc = NULL, int h = 0): data(e), parent(p), lc(lc), rc(rc), height(h){}
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

inline void attachAsLC(Node* lc, Node* p){ // 在节点*p与*lc（可能为空）之间建立父（左）子关系
    p->lc = lc; 
    if(lc) lc->parent = p; 
}
inline void attachAsRC(Node* p, Node* rc){ // 在节点*p与*rc（可能为空）之间建立父（右）子关系
    p->rc = rc; 
    if(rc) rc->parent = p; 
}
     
class Splay{
public:
    Node *_root, *_hot; // 根节点、“命中”节点的父亲
    int _size; // 规模
    Splay(): _size(0), _root(NULL){}
    int updateHeight(Node* x){ // 更新节点x的高度
        return x->height = 1 + max(stature(x->lc), stature(x->rc)); 
    }
    void updateHeightAbove(Node* x){ // 更新高度
        while(x){ // 从x出发，覆盖历代祖先
            updateHeight(x);
             x = x->parent; 
        }
    }
    Node *splay(Node *v){ // 将节点v伸展至根
        if(!v) return NULL;
        Node *p;
        Node *g; 
        while((p = v->parent) && (g = p->parent)){ // 自下而上，反复对*v做双层伸展
            Node *gg = g->parent; // 每轮之后*v都原great-grand parent为父
            if(IsLChild(*v)){
                if(IsLChild(*p)){ // zig-zig
                    attachAsLC(p->rc, g);
                    attachAsLC(v->rc, p);
                    attachAsRC(p, g);
                    attachAsRC(v, p);
                }
                else{ // zig-zag
                    attachAsLC(v->rc, p);
                    attachAsRC(g, v->lc);
                    attachAsLC(g, v);
                    attachAsRC(v, p);
                }
            }
            else if(IsRChild(*p)){ // zag-zag
                attachAsRC(g, p->lc);
                attachAsRC(p, v->lc);
                attachAsLC(g, p);
                attachAsLC(p, v);
            }
            else{ // zag-zig
                attachAsRC(p, v->lc);
                attachAsLC(v->rc, g);
                attachAsRC(v, g);
                attachAsLC(p, v);
            }
            if(!gg) v->parent = NULL; // *v原先的曾祖父*gg不存在：*v现在应为树根
            else ((g == gg->lc) ? attachAsLC(v, gg) : attachAsRC(gg, v)); // *v原先的曾祖父*gg存在：*gg此后应该以*v作为左或右孩子
            updateHeight(g);
            updateHeight(p);
            updateHeight(v);
        }
        if((p = v->parent)){ // p非空：再做一次单旋
            if(IsLChild(*v)){
                attachAsLC(v->rc, p);
                attachAsRC(v, p);
            }
            else{
                attachAsRC(p, v->lc);
                attachAsLC(p, v);
            }
            updateHeight(p);
            updateHeight(v);
        }
        v->parent = NULL;
        return v;
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
    Node* &splaySearch(const int& e){ //查找
        Node* p = search(e);
        _root = splay(p ? p : _hot); // 将最后一个被访问的节点伸展至根
        return _root;
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
    Node* splayLabSearch(const int& e){
        Node* p = labSearch(e);
        _root = splay(p ? p : _hot);
        return p; // 要splay，但切记返回labSearch到的结果
    }
    Node* insert(const int& e){ //插入
        if(!_root){ // 原树为空
            _size++;
            return _root = new Node(e);
        } 
        if(e == splaySearch(e)->data) return _root; // 确认目标节点不存在
        _size++;
        Node *t = _root; // 创建新节点。以下调整<=7个指针以完成局部重构
        if(_root->data < e){ // 插入新根，以t和t->rc为左、右孩子
            t->parent = _root = new Node(e, NULL, t, t->rc); // 2 + 3个
            if(HasRChild(*t)){ // <= 2个
                t->rc->parent = _root;
                t->rc = NULL;
            } 
        }
        else{ // 插入新根，以t->lc和t为左、右孩子
            t->parent = _root = new Node(e, NULL, t->lc, t); // 2 + 3个
            if(HasLChild(*t)){ // <= 2个
                t->lc->parent = _root;
                t->lc = NULL;
            } 
        }
        updateHeightAbove(t); // 更新t及其祖先（只有_root一个祖先）的高度
        return _root; // 新节点必然置于树根，返回之
    } 
    bool remove(const int &e){ //删除
        if(!_root || (e != splaySearch(e)->data)) return false; // 树空或目标不存在（递归基：search()后节点e已被伸展至树根）
        Node *w = _root; 
        if(!HasLChild(*_root)){ // 无左子树：直接删除
            _root = _root->rc;
            if(_root) _root->parent = NULL;
        }
        else if(!HasRChild(*_root)){ // 无右子树：直接删除
            _root = _root->lc;
            if(_root) _root->parent = NULL;
        }
        else{ // 左右子树同时存在
            Node *lTree = _root->lc;
            lTree->parent = NULL;
            _root->lc = NULL; // 暂时将左子树切除
            _root = _root->rc;
            _root->parent = NULL; // 只保留右子树
            splaySearch(w->data); // 以原树根为目标，做一次必定失败的查找（右子树中最小节点必伸展至根，且（因无雷同节点）其左子树必空）
            _root->lc = lTree;
            lTree->parent = _root; // 将原左子树接回原位
        }
        _size--;
        if(_root) updateHeight(_root); // 树非空：树根的高度需要更新
        return true;
    }
}*splayTree;

void input(int n){ // 读取n条指令
    char c;
    int x;
    for(int i = 0; i < n; i++){
        cin >> c >> x;
        switch (c){
            case 'A': // 插入
                splayTree->insert(x);
                break;
            case 'B': // 删除
                splayTree->remove(x);
                break;
            case 'C': // 查询
                Node* temp = splayTree->splayLabSearch(x);
                cout << (temp ? temp->data : -1) << endl;
                break;
        }
    }
}

int main(){
    int n;
    splayTree = new Splay; // 建Splay树
    cin >> n;
    input(n); // 读取n条指令
    return 0;
}