#include <bits/stdc++.h> //P2056 [ZJOI2007] 捉迷藏
using namespace std;
const int maxn = 1e5 + 5;
typedef long long ll;
vector<int> G[maxn];
int sz[maxn];                  // 表示当前树中以某节点为根的子树大小
int mxsz[maxn] = {1000000009}; // 表示当前树中某节点的最大支的大小
int vis[maxn];                 // 标记删除的点，这样其各支就相当于分离了
int rt, tot;                   // tot表示当前树的总大小。这两个量要在递归进入每个solve时重置
int fa[maxn];
int ar[maxn]; // 点权
int sum;

#define FLAG cout << "done" << endl;
inline void debug(initializer_list<int> il)
{
    for (int i : il)
        cout << i << " ";
    cout << endl;
}
inline int read()
{
    char c = getchar();
    int x = 0, f = 1;
    while (c < '0' || c > '9')
    {
        if (c == '-')
            f = -1;
        c = getchar();
    } // 符号
    while (c >= '0' && c <= '9')
    {
        x = x * 10 + c - '0';
        c = getchar();
    } // 数字
    return x * f;
}

int dt[maxn];
int dfn[maxn], sa[2 * maxn], st[18][maxn * 2], bin[32], cnt;
void dfs(int x, int f, int d)
{
    dt[x] = d;
    sa[++cnt] = x;
    dfn[x] = cnt;
    int n = G[x].size();
    for (int i : G[x])
    {
        if (i != f)
        {
            dfs(i, x, d + 1);
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
    for (int i = 1; i < 22; ++i)
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
inline int dis(const int &x, const int &y)
{
    return dt[x] + dt[y] - 2 * dt[lca(x, y)];
}

void get_root(int x, int f)
{
    mxsz[x] = 0, sz[x] = 1;
    for (auto i : G[x])
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

multiset<int, greater<int>> msd[maxn], msf[maxn], ms; // 维护每个分治重心从每个支的结果、对其父亲的贡献信息、全局答案
int res[maxn];                                        // 记录和维护每个分治重心的分治结果

void calc(int x, int f, int d, int tar)
{
    msf[tar].insert(d);
    for (int i : G[x])
    {
        if (i == f || vis[i])
            continue;
        calc(i, x, d + 1, tar);
    }
}

void build(int x) // 构建点分树
{
    vis[x] = 1;
    get_root(x, 0);
    for (int i : G[x])
    {
        if (vis[i])
            continue;
        tot = sz[i];
        rt = 0;
        get_root(i, 0);
        calc(i, x, 1, rt);
        msd[x].insert(*msf[rt].begin());
        fa[rt] = x;
        build(rt);
    }
    if (!msd[x].empty())
    {
        auto it = msd[x].begin();
        res[x] += *it;
        if (++it != msd[x].end())
            res[x] += *it;
    }
    ms.insert(res[x]);
}

void maintain(int x)
{
    int t = 0;
    if (msd[x].size() - msd[x].count(0) <= 1 && ar[x] == 1)
    {
        ;
    }
    else if (!msd[x].empty())
    {
        auto it = msd[x].begin();
        t += *it;
        if (++it != msd[x].end()) // 取代size()>1
            t += *it;
    }
    if (t != res[x])
    {
        ms.erase(ms.find(res[x])); // 切记要查找+删除
        ms.insert(t);
        res[x] = t;
    }
}

void change(int x)
{
    if (ar[x] == 1)
    {
        ar[x] = 0;
        --sum;
        maintain(x);
        for (int y = x; fa[y]; y = fa[y])
        {
            int t = *msf[y].begin();
            msf[y].insert(dis(x, fa[y]));
            if (t != *msf[y].begin())
            {
                msd[fa[y]].erase(msd[fa[y]].find(t));
                msd[fa[y]].insert(*msf[y].begin());
                maintain(fa[y]);
            }
        }
    }
    else
    {
        ar[x] = 1;
        ++sum;
        maintain(x);
        for (int y = x; fa[y]; y = fa[y])
        {
            int t = *msf[y].begin();
            msf[y].erase(msf[y].find(dis(x, fa[y])));
            if (t != *msf[y].begin())
            {
                msd[fa[y]].erase(msd[fa[y]].find(t));
                msd[fa[y]].insert(*msf[y].begin());
                maintain(fa[y]);
            }
        }
    }
}

int main()
{
    // cin.sync_with_stdio(false);
    int n, Q;
    cin >> n;
    for (int i = 1; i < n; ++i)
    {
        int x, y;
        // cin>>x>>y;
        x = read(), y = read();
        G[x].push_back(y);
        G[y].push_back(x);
    }
    dfs(1, 0, 0);
    init();
    tot = n;
    get_root(1, 0);
    int root = rt; // 记录点分树根节点
    build(rt);
    // cin>>Q;
    Q = read();
    while (Q--)
    {
        char ch;
        // cin>>ch;
        ch = getchar();
        getchar();
        if (ch == 'C')
        {
            int x;
            // cin>>x;
            x = read();
            change(x);
        }
        else if (sum == n)
            cout << "-1\n";
        else
            cout << *ms.begin() << "\n";
    }
}