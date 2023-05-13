#include <bits/stdc++.h> //P2495 [SDOI2011] 消耗战
using namespace std;
const int maxn = 2.5e5 + 5;
vector<pair<int, int>> G[maxn], Gx[maxn];

#define FLAG cout << "done" << endl;
inline void debug(initializer_list<int> il)
{
    for (int i : il)
        cout << i << " ";
    cout << endl;
}

int dfn[maxn], tim;
int gd[maxn][22], dt[maxn], lg[maxn], ge[maxn][22];
void dfs0(int x, int f, int d, int fe)
{
    dfn[x] = ++tim;
    dt[x] = d;
    gd[x][0] = f;
    ge[x][0] = fe;
    for (int i = 1; i <= lg[d]; ++i)
    {
        gd[x][i] = gd[gd[x][i - 1]][i - 1];
        ge[x][i] = min(ge[x][i - 1], ge[gd[x][i - 1]][i - 1]);
    }
    for (auto p : G[x])
    {
        int i = p.first;
        if (i == f)
            continue;
        dfs0(i, x, d + 1, p.second);
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

void adde(int x, int y) // 建虚树时，所连两点总是祖孙关系
{
    if (dt[x] < dt[y])
        swap(x, y);
    int to = x;
    int w = 1e9;
    for (int i = lg[dt[x] - dt[y]]; i >= 0; --i)
    {
        int t = gd[x][i];
        if (dt[t] >= dt[y])
        {
            w = min(w, ge[x][i]); // debug({x,t,ge[x][i]});
            x = t;
        }
    }
    Gx[y].emplace_back(to, w); // debug({y,to,w});
}

int st[maxn], top;
void insert(int u)
{
    if (top <= 1)
    {
        st[++top] = u;
        return;
    }
    int lc = lca(u, st[top]);
    if (lc == st[top])
    {
        st[++top] = u;
        return;
    }
    while (top > 1 && dfn[st[top - 1]] >= dfn[lc])
    {
        adde(st[top], st[top - 1]);
        --top;
    }
    if (st[top] != lc)
    {
        adde(st[top], lc);
        st[top] = lc;
    }
    st[++top] = u;
}

int ar[maxn], tag[maxn];

long long dfs(int x, long long fe) // 正常的树形dp
{
    long long sum = 0;
    for (auto p : Gx[x])
    {
        sum += dfs(p.first, p.second);
    }
    Gx[x].clear(); //**即将回溯时，顺便清空边集**
    if (tag[x])
        return fe; // 如果是关键点，必选其父边（题目保证根节点不是关键点）
    return min(fe, sum);
}

int main()
{
    cin.sync_with_stdio(false);
    for (int i = 1; i < maxn; ++i)
        lg[i] = log2(i);
    int n, m;
    cin >> n;
    for (int i = 1; i < n; ++i)
    {
        int x, y, z;
        cin >> x >> y >> z;
        G[x].emplace_back(y, z);
        G[y].emplace_back(x, z);
    }
    dfs0(1, 0, 1, 1e9);
    ar[0] = 1; // ar为关键点序列，为了方便dp故设置st[0]为根节点
    top = 1;   // 栈位置从1开始
    st[1] = 1;
    cin >> m;
    while (m--)
    {
        int k, x;
        cin >> k;
        for (int i = 1; i <= k; ++i)
            cin >> ar[i];
        sort(ar + 1, ar + k + 1, [](const int &a, const int &b)
             { return dfn[a] < dfn[b]; });
        for (int i = 1; i <= k; ++i)
        {
            tag[ar[i]] = 1; // 关键点
            insert(ar[i]);
        }
        while (top > 1)
        {
            adde(st[top], st[top - 1]);
            --top;
        } // 最后top=1
        cout << dfs(ar[0], 1e18) << "\n";
        for (int i = 0; i <= k; ++i)
            tag[ar[i]] = 0; // 清除关键点标记
    }
}
/*
5
1 2 1
1 3 2
3 4 3
3 5 4
5
2 2 3
1 2
1 3
1 4
2 4 5
*/