#include <bits/stdc++.h> //洛谷-点分树模板
using namespace std;
const int maxn = 1e5 + 5;
vector<int> G[maxn];
int sz[maxn];   // 表示当前树中以某节点为根的子树大小
int mxsz[maxn]; // 表示当前树中某节点的最大支的大小
int vis[maxn];  // 标记删除的点，这样其各支就相当于分离了
int rt, tot;    // tot表示当前树的总大小。这两个量要在递归进入每个solve时重置
int fa[maxn];   // fa[x]表示x在点分树上的父节点
int ar[maxn];   // 点权
int n, m;

#define FLAG cout << "done" << endl;
inline void debug(initializer_list<int> il)
{
    for (int i : il)
        cout << i << " ";
    cout << endl;
}
inline int lb(int x) { return x & -x; }

struct BIT
{ // 树状数组（支持查询0下标）
    vector<int> v;
    size_t up;
    void init(size_t Size)
    {
        up = Size;
        v.resize(up);
    }
    void add(int x, int val)
    {
        ++x;
        for (; x < up; x += lb(x))
            v[x] += val;
    }
    int sum(int x)
    {
        if (x < 0)
            return 0; // 1.
        ++x;
        if (x >= up)
            x = up - 1; // 2.
        int res = 0;
        for (; x; x -= lb(x))
            res += v[x];
        return res;
    }
} tr1[maxn], tr2[maxn];

int dt[maxn], lg[maxn], gd[maxn][20];
void dfs0(int x, int f, int d)
{
    dt[x] = d;
    gd[x][0] = f;
    for (int i = 1; i <= lg[d]; ++i)
        gd[x][i] = gd[gd[x][i - 1]][i - 1];
    for (int i : G[x])
    {
        if (i == f)
            continue;
        dfs0(i, x, d + 1);
    }
}

int lca(int x, int y)
{
    if (dt[x] < dt[y])
        swap(x, y);
    while (dt[x] > dt[y])
        x = gd[x][lg[dt[x] - dt[y]]];
    if (x == y)
        return x;
    for (int i = lg[dt[x]]; i >= 0; --i)
    {
        if (gd[x][i] != gd[y][i])
        {
            x = gd[x][i];
            y = gd[y][i];
        }
    }
    return gd[x][0];
}

inline int dis(const int &x, const int &y)
{
    return dt[x] + dt[y] - 2 * dt[lca(x, y)];
}

void get_root(int x, int f) // 同点分治
{
    mxsz[x] = 0, sz[x] = 1;
    for (int i : G[x])
    {
        if (vis[i] || i == f)
            continue;
        get_root(i, x);
        mxsz[x] = max(sz[i], mxsz[x]);
        sz[x] += sz[i];
    }
    mxsz[x] = max(mxsz[x], tot - sz[x]);
    if (mxsz[x] < mxsz[rt])
        rt = x;
}

void calc(int x, int f, int d, int tar, int mode) // 遍历连通块，把贡献加入指定容器
{
    if (mode == 0)
        tr1[tar].add(d, ar[x]);
    else
        tr2[tar].add(d, ar[x]);
    for (int i : G[x])
    {
        if (i == f || vis[i])
            continue;
        calc(i, x, d + 1, tar, mode);
    }
} // 也可以

void build(int x) // 构建点分树
{
    tr1[x].init(tot + 1);
    calc(x, 0, 0, x, 0); // 在构建树的时候顺便初始化。更简便地，由于此题带修，我们可以在构建新树后再执行updata函数来初始化。
    vis[x] = 1;
    get_root(x, 0);
    for (int i : G[x])
    { // 以下代码必须严格确保次序
        if (vis[i])
            continue;
        tot = sz[i];
        rt = 0;
        get_root(i, 0);
        tr2[rt].init(tot + 1);
        calc(i, x, 1, rt, 1);
        fa[rt] = x;
        build(rt);
    }
}

void updata(int x, int k)
{
    int y = x;
    while (y)
    {
        tr1[y].add(dis(x, y), k - ar[x]);
        if (fa[y])
            tr2[y].add(dis(x, fa[y]), k - ar[x]);
        y = fa[y];
    }
    ar[x] = k;
}

int query(int x, int r)
{
    int res = tr1[x].sum(r);
    int y = x;
    while (fa[y])
    {
        res += tr1[fa[y]].sum(r - dis(x, fa[y]));
        res -= tr2[y].sum(r - dis(x, fa[y]));
        y = fa[y];
    }
    return res;
}

int main()
{
    cin.sync_with_stdio(false);
    for (int i = 1; i < maxn; ++i)
        lg[i] = log2(i);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
        cin >> ar[i];
    for (int i = 1; i < n; ++i)
    {
        int x, y;
        cin >> x >> y;
        G[x].push_back(y);
        G[y].push_back(x);
    }
    dfs0(1, 0, 1);
    mxsz[0] = 1e9;
    tot = n;
    rt = 0;
    get_root(1, 0);
    build(rt);
    int lst = 0;
    for (int i = 1; i <= m; ++i)
    {
        int p, x, y;
        cin >> p >> x >> y;
        x ^= lst, y ^= lst;
        if (p)
            updata(x, y);
        else
        {
            lst = query(x, y);
            cout << lst << "\n";
        }
    }
}