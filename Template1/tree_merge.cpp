#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e5 + 5, inf = 0x3f3f3f3f;
vector<int> G[maxn];
int gd[maxn][22], lg[maxn], dt[maxn];
int n, m, cnt;
struct node
{
    int l, r, mx;
} nd[maxn * 50];
int res[maxn];
void dfs1(int x, int f, int d)
{
    gd[x][0] = f;
    dt[x] = d;
    for (int i = 1; i <= lg[d]; ++i)
        gd[x][i] = gd[gd[x][i - 1]][i - 1];
    for (int i : G[x])
    {
        if (i != f)
        {
            dfs1(i, x, d + 1);
        }
    }
}
int lca(int x, int y)
{
    if (dt[x] < dt[y])
        swap(x, y);
    while (dt[x] != dt[y])
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
void add(int &id, int l, int r, int x, int v)
{
    if (id == 0)
        id = ++cnt;
    if (l == r)
    {
        nd[id].mx += v;
        return;
    }
    int mid = (l + r) / 2;
    if (x <= mid)
        add(nd[id].l, l, mid, x, v);
    else
        add(nd[id].r, mid + 1, r, x, v);
    nd[id].mx = max(nd[nd[id].l].mx, nd[nd[id].r].mx);
}
int merge(int a, int b) // 将b合并到a中
{
    if (!a)
        return b;
    if (!b)
        return a;
    nd[a].l = merge(nd[a].l, nd[b].l);
    nd[a].r = merge(nd[a].r, nd[b].r);
    if (nd[a].l == 0 && nd[a].r == 0) // 到达叶子
        nd[a].mx += nd[b].mx;
    else
        nd[a].mx = max(nd[nd[a].l].mx, nd[nd[a].r].mx); // 重新合并更新
    return a;
}
int find(int id, int l, int r)
{
    if (l == r)
        return l;
    int mid = (l + r) / 2;
    if (nd[nd[id].l].mx >= nd[nd[id].r].mx)
        return find(nd[id].l, l, mid);
    else
        return find(nd[id].r, mid + 1, r);
}
void dfs2(int x, int f)
{
    for (int i : G[x])
    {
        if (i != f)
        {
            dfs2(i, x);
            merge(x, i);
        }
    }
    if (nd[x].mx == 0)
        res[x] = 0;
    else
        res[x] = find(x, 1, maxn);
}
int main()
{
    cin >> n >> m;
    cnt = n;
    for (int i = 1; i < n; ++i)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        G[x].push_back(y);
        G[y].push_back(x);
    }
    for (int i = 1; i <= n; ++i)
        lg[i] = log2(i);
    dfs1(1, 0, 1);
    for (int i = 1; i <= m; ++i)
    {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        add(x, 1, maxn, z, 1);
        add(y, 1, maxn, z, 1);
        int lc = lca(x, y);
        add(lc, 1, maxn, z, -1);
        add(gd[lc][0], 1, maxn, z, -1);
    }
    dfs2(1, 0);
    for (int i = 1; i <= n; ++i)
        printf("%d\n", res[i]);
}
/*

*/