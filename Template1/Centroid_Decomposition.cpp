#include<bits/stdc++.h>
using namespace std;
const int maxn=1e4+5;
vector<pair<int,int> > G[maxn];
int q[105];
int sz[maxn];//表示当前树中以某节点为根的子树大小
int mxsz[maxn];//表示当前树中某节点的最大支的大小
int vis[maxn];//标记删除的点，这样其各支就相当于分离了
int judge[10000007];//我们要维护的表。同往常一样，每次不能暴力清空
int rt,tot; //tot表示当前树的总大小。这两个量要在递归进入每个solve时重置
int n,m;

void get_root(int x,int f)
{
    mxsz[x]=0,sz[x]=1;
    for(auto p:G[x])
    {
        int i=p.first;
        if(vis[i] || i==f) continue;
        get_root(i,x);
        mxsz[x]=max(sz[i],mxsz[x]);
        sz[x]+=sz[i];
    }
    mxsz[x]=max(mxsz[x],tot-sz[x]);
    if(mxsz[x]<mxsz[rt]) rt=x;
}

void calc(int x,int f,int d,int mode)
{
    if(mode==0) //计算模式
    {
        for(int i=1;i<=m;++i)
        {
            if(q[i]<d || q[i]-d>10000000) continue;
            if(judge[q[i]-d]) q[i]=0;//标记为Yes
        }
    }
    else if(mode==1) //录入模式
    {
        if(d<=10000000) judge[d]=1;
    }
    else //删除模式
    {
        if(d<=10000000) judge[d]=0;
    }
    for(auto p:G[x])
    {
        int i=p.first;
        if(vis[i] || i==f) continue;
        calc(i,x,d+p.second,mode);
    }
}

void solve(int x) //与寻常分治不同，点分治是自上而下的，即先处理以本点为根的树，在递归处理其子树
{//一定要确保x是所在子树的重心
    for(auto p:G[x])
    {
        int i=p.first;
        if(vis[i]) continue;
        calc(i,x,p.second,0);
        calc(i,x,p.second,1);
    }
    for(auto p:G[x])
    {
        int i=p.first;
        if(vis[i]) continue;
        calc(i,x,p.second,2);
    }
    vis[x]=1;
    get_root(x,0); //以x为根dfs重置sz数组（很经典的漏洞，会导致重心找错从而影响算法效率（然而却还是能过洛谷模板题））
    for(auto p:G[x])
    {
        int i=p.first;
        if(vis[i]) continue;
        tot=sz[i]; //这里的sz[i]不会被下面的get_root影响，因为x的各个子节点在不同的连通块中
        rt=0; //这里不可取rt=i
        get_root(i,x);
        solve(rt);
    }
}

int main()
{
    cin.sync_with_stdio(false);
    cin>>n>>m;
    for(int i=1;i<n;++i){
        int x,y,z;
        cin>>x>>y>>z;
        G[x].push_back({y,z});
        G[y].push_back({x,z});
    }
    for(int i=1;i<=m;++i)
        cin>>q[i];
    judge[0]=1;
    mxsz[0]=1e9;//使得rt=0初始为无穷
    tot=n;
    rt=0;
    get_root(1,0);
    solve(rt);
    for(int i=1;i<=m;++i)
        if(q[i]) cout<<"NAY\n";
        else cout<<"AYE\n";
}
/*
5 8
1 2 6
1 3 2
3 4 3
3 5 4
1 2 3 4 10 11 12 1 3
*/