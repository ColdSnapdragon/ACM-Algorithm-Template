#include<bits/stdc++.h>  //[NOI2018] 归程
using namespace std;
#define FLAG cout<<"done"<<endl;
inline void debug(initializer_list<int>il){for(int i:il)cout<<i<<" ";cout<<endl;}
const int maxn=4e5+5;
vector<int> G[maxn];
int nd[maxn],cnt;
int find(int x)
{
    if(x==nd[x]) return x;
    nd[x]=find(nd[x]);
    return nd[x];
}
struct edge
{
    int x,y,w,z;
}e[maxn];
inline bool cmp(const edge& a,const edge& b){return a.w>b.w;}//采用最小生成树还是最大生成树，只取决于这里的不等号方向

int ar[maxn];
int dt[maxn],gd[maxn][22],lg[maxn];
int vis[maxn];
long long dis[maxn];
vector<pair<int,int> > Gx[maxn];
struct dat{
    int x;
    long long d;
    bool operator < (const dat t) const
    {
        return d>t.d;
    }
};
priority_queue<dat> pq;
void dk(int st)
{
    memset(dis,127,sizeof(dis));
    memset(vis,0,sizeof(vis));
    dis[st]=0;
    pq.push({st,0});
    while(!pq.empty())
    {
        auto t=pq.top();
        pq.pop();
        if(vis[t.x])continue;
        vis[t.x]=1;
        for(auto p:Gx[t.x])
        {
            if(vis[p.first])continue;
            if(dis[p.first]>t.d+p.second)
            {
                dis[p.first]=t.d+p.second;
                pq.push({p.first,dis[p.first]});
            }
        }
    }
}
void dfs(int x,int f,int d) //建树
{//debug({x,f});
    gd[x][0]=f;
    dt[x]=d;
    for(int i=1;i<=lg[d];++i)
        gd[x][i]=gd[gd[x][i-1]][i-1];
    for(int i:G[x])
    {
        dfs(i,x,d+1);
        dis[x]=min(dis[x],dis[i]);
    }
}
int seach(int x,int val) //搜索叶子x的最后一个权值大于val的祖先（小顶堆）
{
    for(int i=lg[dt[x]];i>=0;--i)
    {
        if(gd[x][i]==0) continue;
        int t=gd[x][i];
        if(ar[t]>val)
            x=t;
    }
    return x;
}

int main()
{
    cin.sync_with_stdio(false);//cin.tie(0);
    for(int i=1;i<maxn;++i)
        lg[i]=log2(i);
    int n,m,T;
    cin>>T;
    while(T--)
    {
        cin>>n>>m;
        cnt=n;
        for(int i=1;i<=2*n;++i)
        {
            nd[i]=i;
            G[i].clear();
            Gx[i].clear();
            for(int j=0;j<22;++j) gd[i][j]=0; //多组样例中，gd数组也需要初始化
        }
        for(int i=1;i<=m;++i)
        {
            cin>>e[i].x>>e[i].y>>e[i].z>>e[i].w;
            Gx[e[i].x].push_back({e[i].y,e[i].z});
            Gx[e[i].y].push_back({e[i].x,e[i].z});
        }
        dk(1);
        sort(e+1,e+m+1,cmp);
        for(int i=1;i<=m;++i)
        {
            int a=find(e[i].x),b=find(e[i].y);
            if(a!=b)
            {
                ++cnt;
                G[cnt].push_back(a);
                G[cnt].push_back(b);
                nd[a]=nd[b]=cnt;//合并集合，并将其根设为新点
                ar[cnt]=e[i].w;
            }
        }
        dfs(cnt,0,1); //cnt为Kruscal树的根（准确地说，森林里的每棵树总是以最大编号的节点为根）
        long long Q,K,S,lst=0;
        cin>>Q>>K>>S;
        while(Q--){
            long long x,y;
            cin>>x>>y;
            int v=(x+K*lst-1)%n+1;
            int p=(y+K*lst)%(S+1);
            lst=dis[seach(v,p)];
            cout<<lst<<"\n";
        }
    }
}