#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll base[64];

int main()
{
	cin.tie(0)->sync_with_stdio(false);
	int n;
	cin >> n;
	for (int i = 1; i <= n; ++i)
	{
		ll x;
		cin >> x;
		for (int i = 63; i >= 0; --i)
		{
			if (x >> i) // 判断当前x是否大于等于2^i
			{
				if (base[i])
					x ^= base[i];
				else
				{
					base[i] = x;
					break;
				}
			}
		}
	}
	ll ans = 0;
	for (int i = 63; i >= 0; --i)
	{
		if ((ans ^ base[i]) > ans) // 注意优先级
			ans ^= base[i];
	}
	cout << ans;
}