#include <bits/stdc++.h>
#define ios cin.sync_with_stdio(false) //;cin.tie(0)
using namespace std;
const int maxn = 55;
const long long inf = 3e18;
const double eps = 1e-4;
inline void debug(initializer_list<long long> il)
{
    for (const auto &i : il)
        cout << i << " ";
    cout << "\n";
}
long long P;
int vis[55][20005], n, m;
long long dp[55][20005];
vector<pair<int, int>> G[maxn];
struct dat
{
    int x, g;
    long long d;
    bool operator<(const dat t) const
    {
        return d > t.d;
    }
};
priority_queue<dat> pq;
void dk(int st)
{
    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= P; ++j)
        {
            dp[i][j] = inf;
            vis[i][j] = 0;
        }
    }
    dp[st][0] = 0;
    pq.push({st, 0, 0});
    while (!pq.empty())
    {
        auto t = pq.top();
        pq.pop();
        if (vis[t.x][t.g])
            continue;
        vis[t.x][t.g] = 1;
        for (auto p : G[t.x])
        {
            int k = (t.g + p.second) % P, y = p.first;
            if (vis[y][k])
                continue;
            if (dp[y][k] > t.d + p.second)
            {
                dp[y][k] = t.d + p.second;
                pq.push({y, k, dp[y][k]});
            }
        }
    }
}
int main()
{
    ios;
    int T;
    cin >> T;
    while (T--)
    {
        long long k;
        cin >> n >> m >> k;
        for (int i = 0; i <= n; ++i)
        {
            G[i].clear();
        }
        for (int i = 1; i <= m; ++i)
        {
            int x, y, z;
            cin >> x >> y >> z;
            G[x].push_back({y, z});
            G[y].push_back({x, z});
        }
        int f = 0;
        if (!G[0].empty())
        {
            auto p = G[0][0]; // 从连接0或连接n-1的边中任挑一条边（这里选第一条边）即可
            P = 2 * p.second;
            dk(0);
            if (dp[n - 1][k % P] <= k)
                f = 1;
        }
        if (f)
            cout << "Possible\n";
        else
            cout << "Impossible\n";
    }
}