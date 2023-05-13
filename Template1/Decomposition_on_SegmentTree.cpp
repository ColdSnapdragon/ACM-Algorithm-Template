#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/rope>
#define maxn 100005
#define FLAG cout << "done" << endl;
using namespace std;
using namespace __gnu_cxx;
void debug(initializer_list<int> il)
{
    for (int i : il)
        cout << i << " ";
    cout << endl;
}
const int N = 200005;
int n, m, k;

struct DSU
{                     // 可撤销并查集+种类并查集+按秩合并 （也可以由点带权并查集代替种类并查集）
    int fa[N], rk[N]; // rk[x]表示x的秩
    struct edge
    {
        int x, y, w; // w记录秩是否改变
    } st[N];         // 回退栈
    int up;
    void init()
    {
        up = 0;
        for (int i = 1; i < N; ++i)
            fa[i] = i, rk[i] = 1;
    }
    int find(int x)
    {
        if (x == fa[x])
            return x;
        return find(fa[x]);
    }
    void join(int x, int y)
    {
        int a = find(x), b = find(y);
        if (a != b)
        {
            if (rk[a] > rk[b])
                swap(a, b);
            if (rk[a] == rk[b])
                ++rk[b];
            fa[a] = b;
            st[++up] = {a, b, rk[a] == rk[b]};
        }
        else if (x == 0) // 此判断用于处理无效边（代表一次无效连接）
            st[++up] = {0, 1, 0};
        else
            st[++up] = {0, 0, 0}; // 即便无连接，也别忘了这一步
    }
    int withdraw() // 如果撤回的是无效边，返回1
    {
        auto p = st[up--];
        if (p.x)
        {
            fa[p.x] = p.x;
            rk[p.y] -= p.w;
            return 0;
        }
        else
            return p.y;
    }
} P;

struct node
{
    int l, r;
    vector<pair<int, int>> v;
} nd[maxn << 2];

void make(int x, int l, int r)
{
    nd[x].l = l, nd[x].r = r;
    if (l == r)
        return;
    int mid = (l + r) >> 1;
    make(x << 1, l, mid);
    make(x << 1 | 1, mid + 1, r);
}

void add(int x, int l, int r, const pair<int, int> &p)
{
    if (l <= nd[x].l && nd[x].r <= r)
    {
        nd[x].v.push_back(p);
        return;
    }
    int mid = (nd[x].l + nd[x].r) >> 1;
    if (l <= mid)
        add(x << 1, l, r, p);
    if (r > mid)
        add(x << 1 | 1, l, r, p);
}

int now;

void dfs(int x)
{
    for (auto p : nd[x].v)
    {
        if (P.find(p.first) == P.find(p.second))
        {
            P.join(0, 0);
            P.join(0, 0);
            now += 2;
        }
        else
        {
            P.join(p.first, p.second + n);
            P.join(p.first + n, p.second);
        }
    }
    if (nd[x].l == nd[x].r) // 如果是叶子
    {
        if (now)
            cout << "No\n";
        else
            cout << "Yes\n";
    }
    else
    {
        dfs(x << 1);
        dfs(x << 1 | 1);
    }
    for (auto p : nd[x].v)
    {
        now -= P.withdraw();
        now -= P.withdraw();
    }
}

int main()
{
    cin.sync_with_stdio(false);
    cin >> n >> m >> k;
    make(1, 1, k);
    for (int i = 1; i <= m; ++i)
    {
        int x, y, st, ed;
        cin >> x >> y >> st >> ed;
        if (st == ed)
            continue;
        add(1, st + 1, ed, {x, y});
    }
    P.init();
    dfs(1); // dfs线段树
}
