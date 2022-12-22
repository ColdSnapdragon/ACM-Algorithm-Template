#include<bits/stdc++.h> //求仙人掌的直径
using namespace std;
const int maxn=5e4+5,inf=0x3f3f3f3f;
int dfn[maxn],low[maxn],tim;
int fa[maxn],dep[maxn];
int dt[maxn];//除了环根外，不经过本环的最远叶深度
struct dat{
    int dis,val;
}q[2*maxn]; //注意由于断环为链，长度应翻倍
int a,b;
int ans;
vector<int> G[maxn];
void dfs(int x,int f,int d)
{
    dfn[x]=low[x]=++tim;
    dep[x]=d;
    fa[x]=f;
    for(int i:G[x])
    {
        if(!dfn[i])
        {
            dfs(i,x,d+1);
            if(low[i]>dfn[x])
            {
                ans=max(ans,dt[x]+dt[i]+1);
                dt[x]=max(dt[x],dt[i]+1);
            }
            low[x]=min(low[x],low[i]);
        }
        else if(i!=f)
        {
            low[x]=min(low[x],dfn[i]);
            if(dfn[i]>dfn[x]) //如果是一条向下边，那么说明当前x是一个环根
            {
                int s=dep[i]-dep[x]+1;
                int bs=s/2;
                int y=i;
                vector<int> v;
                while(y!=x)
                {
                    v.push_back(y);
                    y=fa[y];
                }
                v.push_back(x);
                int m=v.size();
                for(int i=0;i<m;++i)
                    v.push_back(v[i]);
                m=v.size();
                a=1,b=0;
                q[a]=q[b]={0,0};
                int tem=dt[x]; dt[x]=0;//暂时令自己为0
                for(int j=0;j<m;++j) //单调栈
                {
                    int t=v[j];
                    while(j-q[a].dis>bs && a<=b)
                        ++a;
                    if(a<=b)
                        ans=max(ans,dt[t]+j+q[a].val);
                    while(q[b].val<=dt[t]-j && a<=b)
                        --b;
                    q[++b]={j,dt[t]-j};
                }
                dt[x]=tem;
                y=i;
                int td=0;
                while(y!=x)
                {
                    int c=dep[y]-dep[x];
                    if(2*c>s)
                        c=s-c;
                    td=max(td,c+dt[y]);
                    y=fa[y];
                }
                ans=max(ans,dt[x]+td);
                dt[x]=max(dt[x],td);
            }
        }
    }
}
int main()
{
    cin.sync_with_stdio(false);
    cout.tie(0);
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=m;++i)
    {
        int k,x,y;
        cin>>k>>x;
        for(int j=1;j<k;++j)
        {
            cin>>y;
            G[x].push_back(y);
            G[y].push_back(x);
            x=y;
        }
    }
    dfs(1,0,1);
    cout<<ans;
}