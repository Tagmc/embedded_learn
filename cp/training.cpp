#include <vector>
#include <iostream>
#include <algorithm>
std::vector<std::vector<int>> adj;
std::vector<int> color;
std::vector<int> parent;
int cycleStart = -1, cycleEnd = -1;
int n, m;

bool dfs(int u)
{
    color[u] = 1;
    for (int v : adj[u])
    {
        if (color[v] == 0)
        {
            parent[v] = u;
            if (dfs(v))
                return true;
        }
        else if (color[v] == 1)
        {
            cycleStart = v;
            cycleEnd = u;
            return true;
        }
    }
    color[u] = 2;
    return false;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cin >> n >> m;
    color.assign(n + 1, 0);
    parent.assign(n + 1, -1);
    adj.resize(n + 1);
    for (int i = 0; i < m; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
    }
    for (int i = 1; i <= n; ++i)
    {
        if (color[i] == 0)
        {
            if (dfs(i))
            {
                break;
            }
        }
    }
    if (cycleStart == -1)
    {
        std ::cout << "IMPOSSIBLE";
        return 0;
    }
    std::vector<int> ans;
    ans.push_back(cycleStart);
    int cur = cycleEnd;
    while (cur != cycleStart)
    {
        ans.push_back(cur);
        cur = parent[cur];
    }
    ans.push_back(cycleStart);
    reverse(ans.begin(), ans.end());
    std::cout << ans.size() << '\n';
    for (int &x : ans)
    {
        std::cout << x << ' ';
    }
    return 0;
}