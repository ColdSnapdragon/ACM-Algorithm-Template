#include<bits/stdc++.h> //P4381 [IOI2008] Island（基环树的直径）
using namespace std;
const int maxn=1e6+5;
typedef long long ll;
vector<int> rd;
int dfn[maxn],fa[maxn],cnt,tim,to[maxn];
struct edge{
	int to,w;
}e[2*maxn];
int hd[maxn],nt[2*maxn],ct=2; //无向边为两条边，记得maxn*2，否则可引发MLE、RE或SF
inline void add(int x,int y,int z)
{
	e[ct].to=y; e[ct].w=z; nt[ct]=hd[x]; hd[x]=ct++;
}
long long dt[maxn];
int a,b;
struct dat{
	long long val,d;
}q[maxn*2];
inline int read()
{
    char c = getchar();int x = 0,f = 1;
    while(c < '0' || c > '9') {if(c == '-') f = -1;c = getchar();}
    while(c >= '0' && c <= '9') {x = x*10 + c -'0';c = getchar();}
    return x*f;
}
void dfs1(int x,int f,int d,int ind)
{
	dfn[x]=++tim;
	to[x]=d;
	fa[x]=f;
	for(int p=hd[x];p;p=nt[p])
	{
		if(p==(ind^1)) continue;
        int i=e[p].to;
        if(dfn[i] && dfn[i]<dfn[x])
        {
            int y=x;
            while(y!=i)
            {
                rd.push_back(y);
                y=fa[y];
            }
            rd.push_back(i);
            to[i]=e[p].w;
        }
        else if(!dfn[i])
            dfs1(i,x,e[p].w,p);
	}
}
void dfs(int x,int f,long long &d)
{
	for(int p=hd[x];p;p=nt[p])
	{
		int i=e[p].to;
		if(i!=f)
		{
			dfs(i,x,d);
			if(dt[x]+dt[i]+e[p].w>d)
				d=dt[x]+dt[i]+e[p].w;
			dt[x]=max(dt[x],dt[i]+e[p].w);
		}
	}
}
signed main()
{
	int n;
	n=read();
	for(int i=1;i<=n;++i)
	{
		int x,y;
		x=read(),y=read();
		add(x,i,y);
		add(i,x,y);
	}
	long long ans=0;
	for(int i=1;i<=n;++i)
	{
		if(!dfn[i])
		{
			dfs1(i,0,0,0);
			int m=rd.size();
			for(int j=0;j<m;++j)
				rd.push_back(rd[j]);
			long long res=0;
			for(int j=1;j<=m;++j)
			{
				int t=rd[j];
				for(int p=hd[t];p;p=nt[p])
				{
					int k=e[p].to;
					if(k!=rd[j-1]&&k!=rd[j+1])
					{
						dfs(k,t,res);
						res=max(res,dt[t]+dt[k]+e[p].w);
						dt[t]=max(dt[t],dt[k]+e[p].w);
					}
				}
			}
			a=1,b=0;
			long long s=0;
			for(int j=0;j<m;++j)
				s+=to[rd[j]];
			long long bs=s/2;
			long long dis=0;
			for(int j=0;j<2*m;++j)
			{
				int t=rd[j];
				while(dis-q[a].d>bs && a<=b)
					++a;
				if(a<=b)
					res=max(res,dt[t]-dis+q[a].val);
				long long tem=dt[t]+dis+s;
				while(q[b].val<=tem && a<=b)
					--b;
				q[++b]={tem,dis};
				dis+=to[t];
			}
			ans+=res;
			rd.clear();
		}	
	}
	cout<<ans;
}