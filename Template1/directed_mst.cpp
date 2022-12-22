#include<bits/stdc++.h>
using namespace std;
const int maxn=1e3+5,inf=1e9;
int vis[maxn],tag[maxn],pre[maxn];
int n,m;
struct dat{
    long long x,y;
}pos[maxn];
double G[maxn][maxn],w[maxn],ans=0;
void mst(int u)
{
    ans=0;
    for(int i=1;i<=n;++i)
        tag[i]=0;
    while(1)//能不能把根u也打tag? 答：不能
    {
        for(int i=1;i<=n;++i)
        {
            if(tag[i] || i==u) continue;
            pre[i]=i; w[i]=inf;
            for(int j=1;j<=n;++j)
            {
                if(!tag[j])
                    if(G[j][i]<w[i])
                    {
                        w[i]=G[j][i];
                        pre[i]=j;
                    }
            }
            if(pre[i]==i)
            {
                ans=-1;
                return;
            }
        }
        int t;
        for(t=1;t<=n;++t)
        {
            if(tag[t]) continue;
            int j=t,cnt=0;
            while(j!=u && pre[j]!=t && cnt<=n)
            {
                j=pre[j];
                ++cnt;
            }
            if(j==u || cnt>n) continue;
            else break;
        }
        if(t>n)
        {
            for(int i=1;i<=n;++i)
            {
                if(!tag[i] && i!=u)
                    ans+=w[i];
            }
            return;
        }
        for(int i=1;i<=n;++i)
            vis[i]=0;
        int j=t;
        do{
            ans+=w[j];
            j=pre[j];
            vis[j]=tag[j]=1; //vis标记当前环，tag标记所有环
        }while(j!=t);
        tag[t]=0;
        for(int k=1;k<=n;++k)//环外点k
        {
            if(tag[k] || vis[k]) continue;
            for(int j=1;j<=n;++j)//环内点j
            {
                if(vis[j])
                {
                    if(G[j][k]<G[t][k])
                        G[t][k]=G[j][k];
                    if(G[k][j]<inf && G[k][t]>G[k][j]-G[pre[j]][j]) //需先保证G[k][j]存在再作减法
                        G[k][t]=G[k][j]-G[pre[j]][j];
                }
            }
        }
    }
}
int main()
{
    while(cin>>n>>m)
    {
        for(int i=1;i<=n;++i)
            scanf("%lld%lld",&pos[i].x,&pos[i].y);
        for(int i=1;i<=n;++i)
            for(int j=1;j<=n;++j)
                G[i][j]=inf;
        for(int i=1;i<=m;++i)
        {
            int a,b;
            scanf("%d%d",&a,&b);
            G[a][b]=sqrt((pos[a].x-pos[b].x)*(pos[a].x-pos[b].x)+(pos[a].y-pos[b].y)*(pos[a].y-pos[b].y));
        }
        mst(1);
        if(ans==-1)
            printf("poor snoopy\n");
        else
            printf("%.2lf\n",ans);
    }
}
/*
9 25
0 6
3 4
4 1
10 10
10 6
6 0
3 5
6 7
3 10
3 9
8 1
2 3
9 4
2 8
2 9
2 1
2 4
5 3
6 7
5 4
5 9
9 2
3 2
1 6
5 6
7 3
1 5
6 3
7 6
4 7
1 2
3 8
1 9
2 5

39.41
*/