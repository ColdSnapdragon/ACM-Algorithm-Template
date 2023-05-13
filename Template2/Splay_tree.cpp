#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> //使用<bits/extc++.h>，会有一个叫iconv.h的文件在编译时无法找到，所以还是记住这两个个头文件的名称
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
typedef tree<long long, null_type, less<long long>, rb_tree_tag, tree_order_statistics_node_update> Tr;
// 为了使用排名相关的函数，必须选用tree_order_statistics_node_update
int main()
{
    cin.sync_with_stdio(false);
    Tr T;
    int Q;
    cin >> Q;
    for (int i = 1; i <= Q; ++i)
    {
        long long p, x;
        cin >> p >> x;
        if (p == 1)
            T.insert((x << 32) + i); //+i是为了去重，“<<32”使得int的负数在映射为long long后还是负数
        else if (p == 2)
            T.erase(T.lower_bound(x << 32));
        else if (p == 3)
            cout << T.order_of_key(x << 32) + 1 << "\n";
        else
        {
            long long ans;
            if (p == 4)
                ans = *T.find_by_order(x - 1);
            if (p == 5)
                ans = *--T.lower_bound(x << 32);
            if (p == 6)
                ans = *T.lower_bound((x + 1) << 32);
            ans >>= 32;
            cout << ans << "\n";
        }
    }
}