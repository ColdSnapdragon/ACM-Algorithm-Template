#include<bits/stdc++.h>
using namespace std;
const int maxn=1e4+5,inf=0x3f3f3f3f;
int vis[maxn],dis[maxn];
struct dat{
    int a,b,c;
    inline bool operator < (dat other)
    {
        return c<other.c;
    }
}e[maxn];
int n;
int sz[maxn];//表示以i为根的并查集中元素个数
int nd[maxn];
long long ans=0;
int find(int x)
{
    if(x==nd[x])return x;
    nd[x]=find(nd[x]);
    return nd[x];
}
int main()
{
    int T;
    cin>>T;
    while(T--)
    {
        scanf("%d",&n);
        for(int i=1;i<=n;++i)
        {
            nd[i]=i;
            sz[i]=1;
        }
        ans=0;
        for(int i=1;i<n;++i)
            scanf("%d%d%d",&e[i].a,&e[i].b,&e[i].c);
        sort(e+1,e+n);
        for(int i=1;i<n;++i)
        {
            int x=find(e[i].a),y=find(e[i].b);
            ans+=(sz[x]*sz[y]-1)*(e[i].c+1);
            sz[x]+=sz[y];
            sz[y]=0;
            nd[y]=x;
        }
        cout<<ans<<endl;
    }
}