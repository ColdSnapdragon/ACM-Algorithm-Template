#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e5 + 5, inf = 0x3f3f3f3f;
int vis[maxn], dis[maxn];
vector<pair<int, int>> G[maxn];
void spfa(int st)
{
    deque<int> q;
    q.push_back(st);
    dis[st] = 0;
    long long sum = 0, num = 1;
    while (!q.empty())
    {
        int t = q.front();
        while (dis[t] * num > sum)
        {
            q.pop_front();
            q.push_back(t);
            t = q.front();
        }
        vis[t] = 0;
        q.pop_front();
        --num;
        sum -= dis[t];
        for (auto i : G[t])
        {
            if (dis[i.first] > dis[t] + i.second)
            {
                dis[i.first] = dis[t] + i.second;
                if (!vis[i.first])
                {
                    if (!q.empty() && dis[i.first] > dis[q.front()])
                        q.push_back(i.first);
                    else
                        q.push_front(i.first);
                    ++num;
                    sum += dis[i.first];
                    vis[i.first] = 1;
                }
            }
        }
    }
}
int main()
{
    int n, m, a, b, st;
    cin >> n >> a >> b >> st;
    for (int i = 1; i <= a; ++i)
    {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        G[x].push_back({y, z});
        G[y].push_back({x, z});
    }
    for (int i = 1; i <= b; ++i)
    {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        G[x].push_back({y, z});
    }
    for (int i = 1; i <= n; ++i)
        dis[i] = inf;
    spfa(st);
    for (int i = 1; i <= n; ++i)
        if (dis[i] == inf)
            printf("NO PATH\n");
        else
            printf("%d\n", dis[i]);
}