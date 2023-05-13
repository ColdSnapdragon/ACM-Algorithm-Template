#include <bits/stdc++.h> //luogu P3628
using namespace std;
using ll = long long;
const int maxn = 1e6 + 5;

struct Pos
{
    ll x, y;
} q[maxn];
ll sum[maxn];
ll dp[maxn];

int main()
{
    cin.tie(0)->sync_with_stdio(false);
    int n;
    ll a, b, c;
    cin >> n >> a >> b >> c;
    int l = 1, r = 1; // 闭区间，至少有两个元素在内
    q[1] = {0, 0};    // 朴素dp是可以从0位置转移的，在斜率优化中也要按X,Y的定义如实加入起点
    for (int i = 1; i <= n; ++i)
    {
        ll t;
        cin >> t;
        sum[i] = sum[i - 1] + t;
        ll k = 2 * a * sum[i];
        while (r > l && k * (q[l + 1].x - q[l].x) < q[l + 1].y - q[l].y) // 用乘法比较斜率(这里是最尾点与其下一个点)(不用拆括号，保证无除法即可)
            ++l;
        dp[i] = q[l].y - k * q[l].x + a * sum[i] * sum[i] + b * sum[i] + c; // B=Y-KX，再从B中得到dp[i]
        // dp[i]=q[l].f+a*(sum[i]-q[l].x)*(sum[i]-q[l].x)+b*(sum[i]-q[l].x)+c; //另一种思想，表达式等价
        Pos p;
        p.x = sum[i], p.y = dp[i] + a * sum[i] * sum[i] - b * sum[i];
        while (r > l && (p.y - q[r].y) * (q[r].x - q[r - 1].x) > (q[r].y - q[r - 1].y) * (p.x - q[r].x))
            --r;
        q[++r] = p;
    }
    cout << dp[n];
}