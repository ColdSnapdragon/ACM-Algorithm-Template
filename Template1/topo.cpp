#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e2 + 5;
vector<int> G[maxn];
vector<int> v;
int in[maxn], ar[maxn];
int n, m;
int topo()
{
    v.clear();
    queue<int> q;
    for (int i = 1; i <= n; ++i)
    {
        in[i] = ar[i];
        if (in[i] == 0)
            q.push(i);
    }
    int f = 1;
    while (!q.empty())
    {
        if (q.size() > 1)
            f = -1;
        int t = q.front();
        q.pop();
        v.push_back(t);
        for (int i : G[t])
        {
            --in[i];
            if (in[i] == 0)
                q.push(i);
        }
    }
    return f * v.size();
}
int main()
{
    while (scanf("%d%d", &n, &m) && (n || m))
    {
        for (int i = 1; i <= n; ++i)
        {
            ar[i] = 0;
            G[i].clear();
        }
        int f = 0;
        for (int i = 1; i <= m; ++i)
        {
            getchar();
            char x, y;
            int a, b;
            scanf("%c<%c", &x, &y);
            a = x - 'A' + 1, b = y - 'A' + 1;
            G[a].push_back(b);
            ++ar[b];
            int r = topo();
            if (f == 1)
                continue;
            else if (r == n)
            {
                string s;
                for (int k : v)
                    s += ('A' + k - 1);
                printf("Sorted sequence determined after %d relations: %s.\n", i, s.c_str());
                f = 1;
            }
            else if (abs(r) != n)
            {
                printf("Inconsistency found after %d relations.\n", i);
                f = 1;
            }
            else if (r < 0 && i == m)
            {
                printf("Sorted sequence cannot be determined.\n");
                f = 1;
            }
        }
    }
}