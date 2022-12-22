#include<bits/stdc++.h>
using namespace std;
const int maxn=1e3+5,inf=0x3f3f3f3f;
int vis[maxn];
double dis[maxn];
int n,m;
struct dat{
    int x,y;
}pos[maxn];
double G[maxn][maxn], ans=0;
void prim()
{
    ans=0;
    for(int i=1;i<=n;++i)
    {
        vis[i]=0;
        dis[i]=1e9;
    }
    dis[1]=0;
    for(int i=1;i<=n;++i)
    {
        int t=-1;
        double d=1e9;
        for(int j=1;j<=n;++j)
        {
            if(!vis[j] && d>dis[j])
            {
                t=j;
                d=dis[j];
            }
        }
        if(t==-1)
        {
            ans=-1;
            break;
        }
        ans+=dis[t];
        vis[t]=1;
        for(int j=1;j<=n;++j)
        {
            if(!vis[j] && G[t][j]!=0 && dis[j]>G[t][j])
                dis[j]=G[t][j];
        }
    }
}
int main()
{
    cin.sync_with_stdio(false);
    while(cin>>n>>m)
    {
        for(int i=1;i<=n;++i)
            cin>>pos[i].x>>pos[i].y;
        for(int i=1;i<=n;++i)
            for(int j=1;j<=n;++j)
                G[i][j]=0.0;
        for(int i=1;i<=m;++i)
        {
            int a,b;
            cin>>a>>b;
            G[a][b]=sqrt((pos[a].x-pos[b].x)*(pos[a].x-pos[b].x)+(pos[a].y-pos[b].y)*(pos[a].y-pos[b].y));
        }
        prim();
        if(ans==-1)
            cout<<"poor snoopy\n";
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