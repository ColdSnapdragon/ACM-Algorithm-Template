#include <bits/stdc++.h> //https://www.cnblogs.com/BCOI/p/9010229.html
using namespace std;
const int maxn = 2e6 + 5;

struct node
{
    int key;
    int val;
    int num;
    int sz;
} nd[maxn];

int fa[maxn], ch[maxn][2];
int root, cnt;

bool get(int x)
{
    return x == ch[fa[x]][1];
}

inline void pushup(int x)
{
    nd[x].sz = nd[ch[x][0]].sz + nd[ch[x][1]].sz + nd[x].num;
}

void rotate(int x) // 单旋一次
{                  // 三次认父，三次作子
    int f = fa[x], g = fa[f], s = ch[x][!get(x)];
    int d1 = get(f), d2 = get(x);
    fa[f] = x;
    fa[x] = g;
    fa[s] = f;
    ch[g][d1] = x;
    ch[f][d2] = s;
    ch[x][!d2] = f;
    pushup(x);
    pushup(f);
}

void splay(int x, int goal) // goal=0，则新根为x；goal!=0，则把x旋转为goal的儿子
{
    if (goal == 0)
        root = x;
    while (fa[x] != goal)
    {
        if (fa[fa[x]] != goal)
        { // 进行双旋
            if (get(x) == get(fa[x]))
                rotate(fa[x]); // 一字选
            else
                rotate(x); // 之字旋
        }
        rotate(x);
    }
}

void insert(int key, int val)
{
    int cur = root, f = 0;
    while (cur && nd[cur].key != key)
    {
        f = cur;
        cur = ch[cur][key > nd[cur].key];
    }
    if (cur)
        ++nd[cur].num;
    else
    {
        cur = ++cnt;
        nd[cur].key = key, nd[cur].val = val, nd[cur].num = 1;
        fa[cur] = f;
        ch[f][key > nd[f].key] = cur;
    }
    splay(cur, 0); // 旋转时会顺便更新合并值
}

int kth(int k)
{
    int cur = root;
    while (1)
    {
        if (k <= nd[ch[cur][0]].sz)
            cur = ch[cur][0];
        else if (k > nd[ch[cur][0]].sz + nd[cur].num)
        {
            k -= nd[ch[cur][0]].sz + nd[cur].num;
            cur = ch[cur][1];
        }
        else
            return cur; // 这里返回节点，按实际需求修改
    }
}

int find(int key) // 找到键与key最接近的节点
{
    int cur = root;
    while (nd[cur].key != key && ch[cur][nd[cur].key < key]) // 含判断叶子
        cur = ch[cur][nd[cur].key < key];
    return cur;
}

int get_pre(int key)
{
    splay(find(key), 0);
    if (nd[root].key < key)
        return nd[root].key;
    int cur = ch[root][0];
    while (ch[cur][1])
        cur = ch[cur][1];
    return cur; // 根的左子树中的最大值结点
}

int get_suc(int key)
{
    splay(find(key), 0);
    if (nd[root].key > key)
        return nd[root].key;
    int cur = ch[root][1];
    while (ch[cur][0])
        cur = ch[cur][0];
    return cur; // 根的右子树中的最小值结点
}

int del(int key)
{
    int lst = get_pre(key);
    int nxt = get_suc(key);
    splay(lst, 0);
    splay(nxt, lst);
    if (nd[ch[nxt][0]].num > 1)
    {
        --nd[ch[nxt][0]].num;
        splay(ch[nxt][0], 0); // 重新统计树的大小
    }
    else
        ch[nxt][0] = 0; // 直接断边
}

int main()
{
    cin.tie(0)->sync_with_stdio(false);
    return 0;
}