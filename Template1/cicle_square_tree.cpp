#include <bits/stdc++.h> //仙人掌上的两点最短路
using namespace std;
const int maxn = 1e4 + 5, inf = 1e9, N = 2 * maxn, mod = 998244353;
vector<pair<int, int>> G[maxn], v[N];
int gd[N][15], lg[N], dis[maxn], dep[N], dt[N];
int dfn[maxn], low[maxn], tim, cnt;
int to[N], fa[N];
int rs[N];
void dfs0(int x, int f, int d, int e)
{
    dis[x] = d;
    fa[x] = f;
    to[x] = e;
    dfn[x] = low[x] = ++tim;
    for (auto p : G[x])
    {
        int t = p.first;
        if (!dfn[t])
        {
            dfs0(t, x, d + p.second, p.second);
            low[x] = min(low[x], low[t]);
        }
        else if (t != f)
        {
            low[x] = min(low[x], dfn[t]);
            if (dfn[t] > dfn[x])
            {
                int y = t;
                int sq = ++cnt;
                rs[sq] = dis[t] - dis[x] + p.second; // 环长
                while (y != x)
                {
                    int re = dis[y] - dis[x];
                    re = min(re, rs[sq] - re);
                    int tem = fa[y];
                    to[y] = re;
                    fa[y] = sq;
                    y = tem;
                }
                to[sq] = 0;
                fa[sq] = x;
            }
        }
    }
}
void dfs(int x, int f, int d, int td)
{
    dep[x] = d;
    dt[x] = td;
    gd[x][0] = f;
    for (int i = 1; i <= lg[d]; ++i)
        gd[x][i] = gd[gd[x][i - 1]][i - 1];
    for (auto p : v[x])
    {
        int t = p.first;
        if (t != f)
        {
            dfs(t, x, d + 1, td + p.second);
        }
    }
}
int main()
{
    int n, m, Q;
    cin >> n >> m >> Q;
    cnt = n;
    for (int i = 1; i <= 2 * n; ++i)
        lg[i] = log2(i);
    for (int i = 1; i <= m; ++i)
    {
        int x, y, z;
        cin >> x >> y >> z;
        G[x].push_back({y, z});
        G[y].push_back({x, z});
    }
    dfs0(1, 0, 0, 0);
    for (int i = 2; i <= cnt; ++i) // 建树
    {
        v[i].push_back({fa[i], to[i]});
        v[fa[i]].push_back({i, to[i]});
    }
    dfs(1, 0, 1, 0);
    while (Q--)
    {
        int a, b, x, y;
        cin >> x >> y;
        if (dep[x] < dep[y])
            swap(x, y);
        a = x, b = y;
        while (dep[x] > dep[y])
            x = gd[x][lg[dep[x] - dep[y]]];
        if (x == y)
            cout << dt[a] - dt[b] << "\n";
        else
        {
            for (int i = lg[dep[x]]; i >= 0; --i)
            {
                if (gd[x][i] != gd[y][i])
                {
                    x = gd[x][i];
                    y = gd[y][i];
                }
            }
            int lc = gd[x][0];
            if (lc <= n) // 如果是圆点
            {
                cout << dt[a] + dt[b] - 2 * dt[lc] << "\n";
            }
            else // 如果是方点
            {
                int s = abs(dis[x] - dis[y]);
                s = min(s, rs[lc] - s);
                cout << dt[a] - dt[x] + dt[b] - dt[y] + s << "\n";
            }
        }
    }
}