#include <bits/stdc++.h> //P3345 [ZJOI2015]幻想乡战略游戏
using namespace std;
const int maxn = 1e5 + 5;
typedef long long ll;
vector<pair<int, int>> G[maxn];
vector<int> Gx[maxn];          // 此题需对点分树建立真正的树结构
int sz[maxn];                  // 表示当前树中以某节点为根的子树大小
int mxsz[maxn] = {1000000009}; // 表示当前树中某节点的最大支的大小
int vis[maxn];                 // 标记删除的点，这样其各支就相当于分离了
int rt, tot;                   // tot表示当前树的总大小。这两个量要在递归进入每个solve时重置
int fa[maxn];
ll ar[maxn]; // 点权

#define FLAG cout << "done" << endl;
inline void debug(initializer_list<int> il)
{
    for (int i : il)
        cout << i << " ";
    cout << endl;
}

ll de[maxn];
int dfn[maxn], sa[2 * maxn], st[18][maxn * 2], bin[32], cnt;
void dfs(int x, int f, int d)
{
    de[x] = d;
    sa[++cnt] = x;
    dfn[x] = cnt;
    int n = G[x].size();
    for (auto p : G[x])
    {
        int i = p.first;
        if (i != f)
        {
            dfs(i, x, d + p.second);
            sa[++cnt] = x;
        }
    }
}
void init()
{
    bin[0] = 1;
    for (int i = 1; i < 30; ++i)
        bin[i] = bin[i - 1] * 2;
    for (int i = 1; i <= cnt; ++i)
        st[0][i] = dfn[sa[i]];
    for (int i = 1; i <= 17; ++i)
    {
        for (int j = 0; j + bin[i] <= cnt; ++j)
        {
            st[i][j] = min(st[i - 1][j], st[i - 1][j + bin[i - 1]]);
        }
    }
}
inline int lca(int x, int y)
{
    int a = dfn[x], b = dfn[y];
    if (a > b)
        swap(a, b);
    int t = log2(b - a + 1);
    return sa[min(st[t][a], st[t][b - bin[t] + 1])];
}
inline ll dis(const int &x, const int &y)
{
    return de[x] + de[y] - 2 * de[lca(x, y)];
}

void get_root(int x, int f)
{
    mxsz[x] = 0, sz[x] = 1;
    for (auto p : G[x])
    {
        int i = p.first;
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

ll sumv[maxn], sumd[maxn], sumf[maxn]; // 分别表示点权和（同时也是对父亲点权和的贡献），总链值，对父亲总链值的贡献

void build(int x) // 构建点分树
{
    // calc(x,0,0,x,0);
    vis[x] = 1;
    get_root(x, 0);
    for (auto p : G[x])
    {
        int i = p.first;
        if (vis[i])
        {
            Gx[x].push_back(i);
            continue;
        }
        tot = sz[i];
        rt = 0;
        get_root(i, 0);
        // calc(i,x,p.second,rt,1);
        Gx[x].push_back(rt);
        fa[rt] = x;
        build(rt);
    }
}

void updata(int x, ll val)
{
    for (int y = x; y; y = fa[y])
    {
        sumv[y] += val;
        sumd[y] += dis(x, y) * val;
        if (fa[y])
            sumf[y] += dis(x, fa[y]) * val; // dis()中带0是会出问题的，有必要加此特判
    }
}

ll query(int x)
{
    ll res = sumd[x];
    for (int y = x; fa[y]; y = fa[y])
    {
        res += sumd[fa[y]] - sumf[y] + dis(x, fa[y]) * (sumv[fa[y]] - sumv[y]);
    }
    return res;
}

ll sea(int x) // 传至此节点的点权和以及总贡献
{
    ll me = query(x);
    int vn = G[x].size();
    for (int i = 0; i < vn; ++i)
    {
        if (query(G[x][i].first) < me) // 本题（求点带权重心）的性质：非重心的节点，至少有一个邻节点的值小于自己
            return sea(Gx[x][i]);      // 我们在建树时，x在原树的每个邻节点恰好对应x在点分树的子节点，从而可以快速找到该邻节点所在的连通块的重心
        // 由于单调性，一定不会回到x在点分树的父节点
    }
    return me;
}

int main()
{
    cin.sync_with_stdio(false);
    int n, Q;
    cin >> n >> Q;
    for (int i = 1; i < n; ++i)
    {
        int x, y, z;
        cin >> x >> y >> z;
        G[x].push_back({y, z});
        G[y].push_back({x, z});
    }
    dfs(1, 0, 0);
    init();
    tot = n;
    get_root(1, 0);
    int root = rt; // 记录点分树根节点
    build(rt);
    while (Q--)
    {
        int x, k;
        cin >> x >> k;
        updata(x, k);
        cout << sea(root) << "\n";
    }
}