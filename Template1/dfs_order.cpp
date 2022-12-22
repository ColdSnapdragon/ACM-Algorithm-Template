#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <math.h>
#include <cstdio>
using namespace std;
const int maxn=1e6+5;
vector<vector<int> > G(maxn);
int ar[maxn];
long long c[2][maxn];

inline int lb(int x){
	return x&-x;
}
void add(int p,int x,long long val)
{
	if(!x)return;
	for(;x<maxn;x+=lb(x))
		c[p][x]+=val;
}
long long sum(int p,int x)
{
	long long res=0;
	for(;x;x-=lb(x))
		res+=c[p][x];
	return res;
}

long long dt[maxn];
int l[maxn],r[maxn],tim,fa[maxn];
int dfn[maxn],sa[2*maxn],st[18][maxn*2],bin[32],cnt;
void dfs(int x,int f,int d)
{
	fa[x]=f;
	dt[x]=d;
	l[x]=++tim;
	sa[++cnt]=x;
	dfn[x]=cnt;
	int n=G[x].size();
	for(auto i:G[x])
	{
		if(i!=f)
		{
			dfs(i,x,d+1);
			sa[++cnt]=x;
		}
	}
	r[x]=tim;
}
void init()
{
	bin[0]=1;
	for(int i=1;i<30;++i)
		bin[i]=bin[i-1]*2;
	for(int i=1;i<=cnt;++i)
		st[0][i]=dfn[sa[i]];
	for(int i=1;i<=17;++i)
	{
		for(int j=0;j+bin[i]<=cnt;++j)
		{
			st[i][j]=min(st[i-1][j],st[i-1][j+bin[i-1]]);
		}
	}
}
int lca(int x,int y)
{
	int a=dfn[x],b=dfn[y];
	if(a>b)
		swap(a,b);
	int t=log2(b-a+1);
	return sa[min(st[t][a],st[t][b-bin[t]+1])];
}
int main()
{
	int n,m,rt;
	cin>>n>>m>>rt;
	for(int i=1;i<=n;++i)
	{
		scanf("%d",&ar[i]);
	}
	for(int i=1;i<n;++i)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		G[x].push_back(y);
		G[y].push_back(x);
	}
	dfs(rt,0,1);
	init();
	for(int i=1;i<=n;++i)
	{
		add(0,l[i],ar[i]);
		add(0,l[fa[i]],-ar[i]);
		add(1,l[i],ar[i]*dt[i]);
		//add(2,l[fa[i]],-ar[i]);
		add(1,l[fa[i]],-ar[i]*dt[fa[i]]);
	}
	while(m--)
	{
		int x,y,p;
		long long z;
		scanf("%d",&p);
		if(p==1)
		{
			scanf("%d%d%lld",&x,&y,&z);
			int lc=lca(x,y);
			//cout<<"*"<<lc<<endl;
			add(0,l[x],z);
			add(0,l[y],z);
			add(0,l[lc],-z);
			add(0,l[fa[lc]],-z);
			add(1,l[x],z*dt[x]);
			//add(2,l[x],z);
			add(1,l[y],z*dt[y]);
			//add(2,l[y],z);
			add(1,l[lc],-z*dt[lc]);
			//add(2,l[lc],z);
			add(1,l[fa[lc]],-z*dt[fa[lc]]);
			//add(2,l[fa[lc]],z);
		}
		else if(p==2)
		{
			scanf("%d",&x);
			printf("%lld\n",sum(0,r[x])-sum(0,l[x]-1));
		}
		else if(p==3)
		{
			scanf("%d",&x);
			printf("%lld\n",sum(1,r[x])-sum(1,l[x]-1)-(dt[x]-1)*(sum(0,r[x])-sum(0,l[x]-1)));
		}
	}
}
