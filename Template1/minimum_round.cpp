#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e3 + 5, inf = 0x1f1f1f1f1f1f1f;
int d[maxn][maxn], pt[maxn][maxn];
int G[1000][1000];
int ans = inf, n, m;
vector<int> res;
void fun(int a, int b)
{
    while (pt[a][b] != b)
    {
        res.push_back(pt[a][b]);
        a = pt[a][b];
    }
}
void solve()
{
    for (int k = 1; k <= n; ++k)
    {
        for (int i = 1; i < k; ++i)
            for (int j = i + 1; j < k; ++j)
                if (ans > d[i][j] + G[i][k] + G[k][j])
                {
                    ans = d[i][j] + G[i][k] + G[k][j];
                    res.clear();
                    fun(i, j);
                    res.push_back(i);
                    res.push_back(k);
                    res.push_back(j);
                }
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                if (d[i][j] > d[i][k] + d[k][j])
                {
                    d[i][j] = d[i][k] + d[k][j];
                    pt[i][j] = pt[i][k];
                }
    }
}
int main()
{
    cin >> n >> m;
    memset(G, 0x1f, sizeof(G)); // 0x1f1f1f1f1f1f*3不会爆int
    memset(d, 0x1f, sizeof(d));
    for (int i = 1; i <= n; ++i)
        d[i][i] = G[i][i] = 0;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
        {
            pt[i][j] = j;
            pt[j][i] = i;
        }
    for (int i = 1; i <= m; ++i)
    {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        if (G[x][y] > z)
        {
            d[x][y] = G[x][y] = z;
            d[y][x] = G[y][x] = z;
        }
    }
    solve();
    if (ans == inf)
        cout << "No solution.";
    else
    {
        int a = inf, k = 0;
        int vn = res.size();
        for (int i = 0; i < vn; ++i)
            if (res[i] < a)
            {
                a = res[i];
                k = i;
            }
        for (int i = 0; i < vn; ++i)
        {
            printf("%d ", res[(k - i + vn) % vn]);
        }
    }
}
/*
10 15
8 2 30
1 2 6
5 1 10
2 3 7
9 3 30
7 9 1
10 7 5
6 8 3
8 10 4
6 5 30
4 5 9
7 1 30
3 4 8
9 6 2
10 4 30

9 6 7 10 8
*/