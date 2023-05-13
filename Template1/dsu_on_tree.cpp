#include <bits/stdc++.h> //dsu on tree经典题目：CF741D. Arpa’s letter-marked tree and Mehrdad’s Dokhtar-kosh paths
#define maxn 500005
#define N 1 << 22
#define inf 1000000009
using namespace std;
vector<int> G[maxn];
int ar[maxn], len[N], res[maxn]; // 以0/1表示22个字母分别出现奇数次还是偶数次，用len[s]表示从根到某一节点形成的01串s的最大深度
int sz[maxn], son[maxn];
int st[maxn];
int ans;
void debug(initializer_list<int> il)
{
	for (const auto &i : il)
		cout << i << " ";
	cout << endl;
}
void dfs0(int x, int f, int s)
{
	sz[x] = 1;
	st[x] = s; // 预先处理出每个节点到根的01串
	for (int i : G[x])
	{
		if (i == f)
			continue;
		dfs0(i, x, s ^ (1 << ar[i])); // 异或是不进位的加法，很适于独立的奇偶计数
		if (sz[i] > sz[son[x]])
			son[x] = i;
		sz[x] += sz[i];
	}
}
void cal(int x, int f, int tp, int d, int lcd)
{
	int s = st[x];
	if (tp == 1) // 计算模式
	{
		ans = max(ans, d + len[s] - 2 * lcd); // 这里由于d-2*lcd可能还是大于零，所以在重置len时不能只是置0，要赋为-inf（包括初始化时）
		for (int i = (1 << 21); i; i >>= 1)
			ans = max(ans, d + len[i ^ s] - 2 * lcd);
	}
	else if (tp == 2) // 添加模式
		len[s] = max(len[s], d);
	else // 删除模式
		len[s] = -inf;
	for (int i : G[x]) // 还可以通过提前生成dfs序，来代替遍历子树
		if (i != f)
			cal(i, x, tp, d + 1, lcd);
}
void dfs(int x, int f, int d)
{
	int s = st[x];
	for (int i : G[x])
		if (i != f & i != son[x])
		{
			dfs(i, x, d + 1);
			cal(i, x, 3, d + 1, d);
			ans = 0;
		}
	if (son[x])
	{
		dfs(son[x], x, d + 1);
	}
	len[s] = max(len[s], d); // 加入当前节点
	ans = len[s] - d;
	for (int i = (1 << 21); i; i >>= 1)
		ans = max(ans, len[s ^ i] - d);
	for (int i : G[x])
		if (i != f & i != son[x])
		{
			cal(i, x, 1, d + 1, d);
			cal(i, x, 2, d + 1, d);
		}
	res[x] = ans;
	for (int i : G[x])
		if (i != f)
			res[x] = max(res[x], res[i]);
}
int main()
{
	cin.sync_with_stdio(false);
	int n;
	cin >> n;
	for (int i = 2; i <= n; ++i)
	{
		int x;
		char ch;
		cin >> x >> ch;
		G[x].push_back(i);
		G[i].push_back(x);
		ar[i] = ch - 'a';
	}
	memset(len, -0x3f, sizeof(len)); // 初始化为负无穷
	dfs0(1, 0, 0);
	dfs(1, 0, 0);
	for (int i = 1; i <= n; ++i)
		cout << res[i] << " ";
}
/*
9
1 a
1 b
2 a
2 b
4 a
3 b
3 c
8 c

5
1 a
2 b
3 c
4 b

6
1 a
2 b
3 b
4 b
5 b
*/