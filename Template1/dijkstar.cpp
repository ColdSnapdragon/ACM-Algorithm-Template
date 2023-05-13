#include <bits/stdc++.h>
using namespace std;
const int maxn = 1.5e5 + 5, inf = 0x3f3f3f3f;
int vis[maxn];
struct dat
{
    int x, d;
    bool operator<(const dat t) const
    {
        return d > t.d;
    }
};
priority_queue<dat> pq;
vector<pair<int, int>> G[maxn];
int dis[maxn];
void dk(int st)
{
    memset(dis, 0x3f, sizeof(dis));
    dis[st] = 0;
    pq.push({st, 0});
    while (!pq.empty())
    {
        auto t = pq.top();
        pq.pop();
        if (vis[t.x])
            continue;
        vis[t.x] = 1;
        for (auto p : G[t.x])
        {
            if (vis[p.first])
                continue;
            if (dis[p.first] > t.d + p.second)
            {
                dis[p.first] = t.d + p.second;
                pq.push({p.first, dis[p.first]});
            }
        }
    }
}
int main()
{
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= m; ++i)
    {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        G[x].push_back({y, z});
        // G[y].push_back({x,z});//用于无向图
    }
    dk(1);
    if (dis[n] == inf)
        cout << -1;
    else
        cout << dis[n];
}