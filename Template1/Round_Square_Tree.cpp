#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+5,N=2*maxn,inf=1e9;
inline void debug(initializer_list<int>il){for(int i:il)cout<<i<<" ";cout<<endl;}
vector<int> G[N];
int ar[N];
int dfn[maxn],low[maxn],od,st[maxn],up;
int hd[maxn],to[N],nt[N],e_idx;
int cnt;
inline void add(int x,int y){to[++e_idx]=y;nt[e_idx]=hd[x];hd[x]=e_idx;}
void tarjan(int x)
{
    dfn[x]=low[x]=++od;
    st[++up]=x;
    for(int p=hd[x];p;p=nt[p])
    {
        int t=to[p];
        if(!dfn[t])
        {
            tarjan(t);
            low[x]=min(low[x],low[t]);
            if(low[t]>=dfn[x])
            {
                ++cnt;
                do{
                    int y=st[up];
                    G[cnt].push_back(y);
                    G[y].push_back(cnt);
                }while(st[up--]!=t); //弹出至当前儿子
                G[cnt].push_back(x);
                G[x].push_back(cnt);
            }
        }
        else low[x]=min(low[x],dfn[t]); //求vDCC的过程允许从父亲更新low值；无向图没有横边，也无需判断是否在栈中
    }
}
int sz[N],son[N],fa[N],dep[N],top[N],l[N],r[N],tim,sa[N];
struct node{
    int l,r,val;
}nd[4*N];
int n,m,Q;
multiset<int> ms[N];
void dfs1(int x,int f,int d) //建成圆方树
{
    fa[x]=f;
    dep[x]=d;
    sz[x]=1;
    int t=0;
    for(int i:G[x])
    {
        if(i!=f)
        {
            dfs1(i,x,d+1);
            if(sz[i]>t){t=sz[i];son[x]=i;}
            sz[x]+=sz[i];
            if(x>n) //如果是方点，则统计其子节点权值
                ms[x].insert(ar[i]);
        }
    }
    if(x>n)
        ar[x]=*(ms[x].begin());
}
void dfs2(int x,int tp)
{
    l[x]=++tim;
    sa[tim]=x;
    top[x]=tp;
    if(son[x]) dfs2(son[x],tp);
    for(int i:G[x])
    {
        if(i!=fa[x] && i!=son[x])
            dfs2(i,i);
    }
    r[x]=tim;
}
inline void pushup(int x)
{
    nd[x].val=min(nd[x<<1].val,nd[x<<1|1].val);
}
void make(int x,int l,int r)
{
    nd[x].l=l,nd[x].r=r;
    if(l==r)
    {
        nd[x].val=ar[sa[l]]; //注意这里的初始化要从位置映射回原节点
        return;
    }
    int mid=(l+r)/2;
    make(x<<1,l,mid);
    make(x<<1|1,mid+1,r);
    pushup(x);
}
void updata(int x,int pos,int v)
{
    if(nd[x].l==nd[x].r)
    {
        nd[x].val=v;
        return;
    }
    int mid=(nd[x].l+nd[x].r)>>1;
    if(pos<=mid)
        updata(x<<1,pos,v);
    else
        updata(x<<1|1,pos,v);
    pushup(x);
}
int query(int x,int l,int r)
{
    if(l<=nd[x].l&&nd[x].r<=r)
    {
        return nd[x].val;
    }
    int mid=(nd[x].l+nd[x].r)>>1;
    int a=inf,b=inf;
    if(l<=mid)
        a=query(x<<1,l,r);
    if(r>mid)
        b=query(x<<1|1,l,r);
    return min(a,b);
}

int main()
{
    cin.sync_with_stdio(false);
    cin>>n>>m>>Q;
    cnt=n;
    for(int i=1;i<=n;++i)
    {
        cin>>ar[i];
    }
    ar[0]=inf;
    for(int i=1;i<=m;++i)
    {
        int x,y;
        cin>>x>>y;
        add(x,y);add(y,x);
    }
    tarjan(1);
    dfs1(1,0,1);
    dfs2(1,1);
    make(1,1,cnt);
    while(Q--)
    {
        char ch;
        int x,y;
        cin>>ch>>x>>y;
        if(ch=='C')
        {
            updata(1,l[x],y);
            if(fa[x]) //用multiset来维护方点的子节点最小权值
            {
                auto &s=ms[fa[x]];
                s.erase(s.find(ar[x]));
                s.insert(y);
                updata(1,l[fa[x]],*(s.begin()));
            }
            ar[x]=y;
        }
        else
        {
            int res=inf;
            while(top[x]!=top[y])
            {
                if(dep[top[x]]<dep[top[y]])
                    swap(x,y);
                res=min(res,query(1,l[top[x]],l[x]));
                x=fa[top[x]];
            }
            if(dep[x]<dep[y])
                swap(x,y);
            if(y>n) //如果是方点，额外判断其父节点（在以圆点为根的圆方树中，方点父亲一定是圆点）
            {
                res=min(res,ar[fa[y]]);
            }
            res=min(res,query(1,l[y],l[x]));
            cout<<res<<"\n";
        }
    }
}
