#include <bits/stdc++.h> //洛谷——缩点
#define FLAG cout << "done" << endl;
using namespace std;
const int maxn = 1e4 + 5, M = 1e5 + 5;
const double inf = 1061109567;
const double eps = 1e-4;
vector<int> G[maxn];
int to[M], hd[maxn], nt[M], e = 1;
int dfn[maxn], low[maxn], tim;
int id[maxn], in[maxn];
int ar[maxn], tag[maxn], st[maxn], up;
inline void debug(initializer_list<long long> il)
{
    for (const auto &i : il)
        cout << i << " ";
    cout << "\n";
}
inline void add(int x, int y)
{
    to[e] = y;
    nt[e] = hd[x];
    hd[x] = e++;
}
void tarjan(int x) // SCC-tarjan的参数只有当前节点x
{
    dfn[x] = low[x] = ++tim;
    tag[x] = 1;
    st[++up] = x;
    for (int k = hd[x]; k; k = nt[k])
    {
        int i = to[k];
        if (!dfn[i])
        {
            tarjan(i);
            low[x] = min(low[x], low[i]);
        }
        else if (tag[i]) // 若目标点已访问而不在栈中，说明早已被弹出作为其他SCC，就不能再由它更新了
            low[x] = min(low[x], dfn[i]);
    }
    if (low[x] == dfn[x])
    {
        int res = 0, t;
        do
        {
            t = st[up--];
            res += ar[t];
            tag[t] = 0;
            id[t] = x; // 缩为本点
        } while (t != x);
        ar[x] = res;
    }
}
int mem[maxn];
int dfs(int x)
{
    if (mem[x])
        return mem[x];
    for (int i : G[x])
    {
        mem[x] = max(mem[x], dfs(i));
    }
    mem[x] += ar[x];
    return mem[x];
}
int main()
{
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
        cin >> ar[i];
    for (int i = 1; i <= m; ++i)
    {
        int x, y;
        cin >> x >> y;
        add(x, y);
    }
    for (int i = 1; i <= n; ++i) // 由于不一定是流图，要多次tarjan
    {
        if (!dfn[i])
            tarjan(i);
    }
    for (int i = 1; i <= n; ++i)
    {
        for (int j = hd[i]; j; j = nt[j])
        {
            int k = to[j];
            if (id[i] != id[k]) // 说明这条边一定在缩点图上
            {
                G[id[i]].push_back(id[k]);
                ++in[id[k]];
            }
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        if (in[i] == 0)
            ans = max(ans, dfs(i));
    cout << ans;
}
// 对于一个原图点，如果id[x]!=x，说明它不在缩点图上