
#include <bits/stdc++.h>
using namespace std;
#define endl "\n"
#define all(v) v.begin(), v.end()
#define gen(arr, n, nxt) generate(arr, arr + n, nxt)
#define Blobo2                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0);
#pragma GCC optimize("Ofast")
#pragma GCC target("popcnt")
int nxt()
{
    int x;
    cin >> x;
    return x;
}
const int mo = 998244353, N = 5e5 + 5;
int par[N], sparse[2 * N][20], IDX[N], query[N][2], root[N], color[N], q, c_0;
long long dis[N];
int n = 1, INDEX = 0;
vector<pair<int, int>> adj[N];

void dfs(int u, int p = -1)
{
    par[u] = p;
    IDX[u] = INDEX;
    sparse[INDEX++][0] = u;
    for (auto &[v, c] : adj[u])
    {
        if (v == p)
            continue;
        dis[v] = dis[u] + c;
        sparse[INDEX++][0] = u;
        dfs(v, u);
    }
    return;
}

long long distance(int x, int y)
{
    if (x == -1 || y == -1)
        return -1e9;
    if (IDX[x] > IDX[y])
        swap(x, y);
    int lg = __lg(IDX[y] - IDX[x] + 1);
    long long X = dis[sparse[IDX[x]][lg]];
    long long Y = dis[sparse[IDX[y] - (1 << lg) + 1][lg]];
    long long lca;
    if (X < Y)
        lca = X;
    else
        lca = Y;
    return dis[y] + dis[x] - (lca << 1ll);
}

struct SegTree1
{

    struct Node
    {
        int L, R, X, Y;
        long long diameter;
    };
    Node tree[10000005];
    int segNode = 0;

    void init()
    {
        segNode = 0;
        return;
    }

    int newNode()
    {
        segNode++;
        tree[segNode].L = tree[segNode].R = tree[segNode].X = tree[segNode].Y = -1;
        tree[segNode].diameter = -1e9;
        return segNode;
    }

    void Merge(int p)
    {
        if (tree[p].L == -1 && tree[p].R == -1)
            return;
        if (tree[p].L == -1)
        {
            tree[p].X = tree[tree[p].R].X;
            tree[p].Y = tree[tree[p].R].Y;
            tree[p].diameter = tree[tree[p].R].diameter;
        }
        else if (tree[p].R == -1)
        {
            tree[p].X = tree[tree[p].L].X;
            tree[p].Y = tree[tree[p].L].Y;
            tree[p].diameter = tree[tree[p].L].diameter;
        }
        else
        {
            tree[p].diameter = -1e9;
            tree[p].X = -1;
            tree[p].Y = -1;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    int x, y;
                    if (i == 0)
                        x = tree[tree[p].L].X;
                    if (i == 1)
                        x = tree[tree[p].L].Y;

                    if (j == 0)
                        y = tree[tree[p].R].X;
                    if (j == 1)
                        y = tree[tree[p].R].Y;
                    if (x == -1 || y == -1)
                        continue;
                    long long DIS = distance(x, y);
                    if (DIS > tree[p].diameter)
                    {
                        tree[p].diameter = DIS;
                        tree[p].X = x;
                        tree[p].Y = y;
                    }
                }
            }
            if (tree[tree[p].L].diameter > tree[p].diameter)
            {
                tree[p].diameter = tree[tree[p].L].diameter;
                tree[p].X = tree[tree[p].L].X;
                tree[p].Y = tree[tree[p].L].Y;
            }
            if (tree[tree[p].R].diameter > tree[p].diameter)
            {
                tree[p].diameter = tree[tree[p].R].diameter;
                tree[p].X = tree[tree[p].R].X;
                tree[p].Y = tree[tree[p].R].Y;
            }
        }
        return;
    }

    void add(int p, int l, int r, int idx)
    {
        if (l == r)
        {
            tree[p].X = tree[p].Y = idx;
            tree[p].diameter = 0;
            return;
        }
        int mid = (l + r) >> 1;
        if (l <= idx && idx <= mid)
        {
            if (tree[p].L == -1)
                tree[p].L = newNode();
            add(tree[p].L, l, mid, idx);
        }
        else if (mid + 1 <= idx && idx <= r)
        {
            if (tree[p].R == -1)
                tree[p].R = newNode();
            add(tree[p].R, mid + 1, r, idx);
        }
        Merge(p);
        return;
    }
    void add(int c, int idx)
    {
        add(c, 0, n - 1, idx);
        return;
    }

    void remove(int p, int l, int r, int idx)
    {
        if (l == r)
        {
            tree[p].X = tree[p].Y = -1;
            tree[p].diameter = -1e9;
            return;
        }
        int mid = (l + r) >> 1;
        if (l <= idx && idx <= mid)
            remove(tree[p].L, l, mid, idx);
        else if (mid + 1 <= idx && idx <= r)
            remove(tree[p].R, mid + 1, r, idx);
        Merge(p);
        return;
    }
    void remove(int c, int idx)
    {
        remove(c, 0, n - 1, idx);
        return;
    }
} small;

struct SegTree2
{

    struct Node
    {
        int L, R, Xsame, Ysame, Xdiff, Ydiff;
        long long diameterSame, diameterDiff;
    };

    Node tree[1000005];
    int segNode = 0;

    void init()
    {
        segNode = 0;
        return;
    }

    int newNode()
    {
        segNode++;
        tree[segNode].L = tree[segNode].R = tree[segNode].Xsame = tree[segNode].Ysame =
            tree[segNode].Xdiff = tree[segNode].Ydiff = -1;
        tree[segNode].diameterSame = tree[segNode].diameterDiff = -1e18;
        return segNode;
    }

    void Merge(int p)
    {
        if (tree[p].L == -1 && tree[p].R == -1)
            return;
        if (tree[p].L == -1)
        {
            tree[p].Xsame = tree[tree[p].R].Xsame;
            tree[p].Ysame = tree[tree[p].R].Ysame;
            tree[p].diameterSame = tree[tree[p].R].diameterSame;

            tree[p].Xdiff = tree[tree[p].R].Xdiff;
            tree[p].Ydiff = tree[tree[p].R].Ydiff;
            tree[p].diameterDiff = tree[tree[p].R].diameterDiff;
        }
        else if (tree[p].R == -1)
        {
            tree[p].Xsame = tree[tree[p].L].Xsame;
            tree[p].Ysame = tree[tree[p].L].Ysame;
            tree[p].diameterSame = tree[tree[p].L].diameterSame;

            tree[p].Xdiff = tree[tree[p].L].Xdiff;
            tree[p].Ydiff = tree[tree[p].L].Ydiff;
            tree[p].diameterDiff = tree[tree[p].L].diameterDiff;
        }
        else
        {
            tree[p].diameterSame = tree[tree[p].L].diameterSame;
            tree[p].Xsame = tree[tree[p].L].Xsame;
            tree[p].Ysame = tree[tree[p].L].Ysame;
            if (tree[tree[p].R].diameterSame > tree[p].diameterSame)
            {
                tree[p].diameterSame = tree[tree[p].R].diameterSame;
                tree[p].Xsame = tree[tree[p].R].Xsame;
                tree[p].Ysame = tree[tree[p].R].Ysame;
            }
            tree[p].diameterDiff = -1e9;
            tree[p].Xdiff = tree[p].Ydiff = -1;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    int x, y;
                    if (i == 0)
                        x = tree[tree[p].L].Xsame;
                    if (i == 1)
                        x = tree[tree[p].L].Ysame;
                    if (i == 2)
                        x = tree[tree[p].L].Xdiff;
                    if (i == 3)
                        x = tree[tree[p].L].Ydiff;

                    if (j == 0)
                        y = tree[tree[p].R].Xsame;
                    if (j == 1)
                        y = tree[tree[p].R].Ysame;
                    if (j == 2)
                        y = tree[tree[p].R].Xdiff;
                    if (j == 3)
                        y = tree[tree[p].R].Ydiff;

                    if (x == -1 || y == -1)
                        continue;

                    long long DIS = distance(x, y);
                    if (DIS > tree[p].diameterDiff)
                    {
                        tree[p].diameterDiff = DIS;
                        tree[p].Xdiff = x;
                        tree[p].Ydiff = y;
                    }
                }
            }
            if (tree[tree[p].L].diameterDiff > tree[p].diameterDiff)
            {
                tree[p].diameterDiff = tree[tree[p].L].diameterDiff;
                tree[p].Xdiff = tree[tree[p].L].Xdiff;
                tree[p].Ydiff = tree[tree[p].L].Ydiff;
            }
            if (tree[tree[p].R].diameterDiff > tree[p].diameterDiff)
            {
                tree[p].diameterDiff = tree[tree[p].R].diameterDiff;
                tree[p].Xdiff = tree[tree[p].R].Xdiff;
                tree[p].Ydiff = tree[tree[p].R].Ydiff;
            }
        }
        return;
    }

    void update(int p, int l, int r, int idx)
    {
        if (l == r)
        {
            tree[p].Xsame = small.tree[root[idx]].X;
            tree[p].Ysame = small.tree[root[idx]].Y;
            tree[p].Xdiff = tree[p].Ydiff = -1;
            tree[p].diameterSame = small.tree[root[idx]].diameter;
            tree[p].diameterDiff = -1e9;
            return;
        }
        int mid = (l + r) >> 1;
        if (l <= idx && idx <= mid)
        {
            if (tree[p].L == -1)
                tree[p].L = newNode();
            update(tree[p].L, l, mid, idx);
        }
        else if (mid + 1 <= idx && idx <= r)
        {
            if (tree[p].R == -1)
                tree[p].R = newNode();
            update(tree[p].R, mid + 1, r, idx);
        }
        Merge(p);
        return;
    }
    void update(int c, int idx)
    {
        update(c, 0, q - 1, idx);
        return;
    }
} big;

inline void testCase()
{
    q = nxt(), c_0 = nxt() - 1;
    small.init();
    big.init();
    for (int i = 0; i < q; i++)
    {
        int type = nxt();
        if (type)
        {
            int node = nxt() - 1, c = nxt() - 1;
            query[i][0] = node + (int)(1e6);
            query[i][1] = c;
        }
        else
        {
            int node = nxt() - 1, c = nxt() - 1, d = nxt();
            adj[node].push_back({n, d});
            adj[n++].push_back({node, d});
            query[i][0] = node;
            query[i][1] = c;
        }
    }
    dfs(0, -1);
    for (int j = 1; j <= 19; j++)
    {
        for (int i = 0; i + (1 << (j - 1)) < INDEX; i++)
        {
            int x = sparse[i][j - 1];
            int y = sparse[i + (1 << (j - 1))][j - 1];
            if (dis[x] < dis[y])
                sparse[i][j] = x;
            else
                sparse[i][j] = y;
        }
    }
    for (int i = 0; i < q; i++)
        root[i] = small.newNode();
    color[0] = c_0;
    small.add(root[c_0], 0);
    int ROOT = big.newNode();
    big.update(ROOT, c_0);
    int node = 0;
    for (int i = 0; i < q; i++)
    {
        if (query[i][0] >= (int)1e6)
        {
            int x = query[i][0] - (int)1e6, c = query[i][1];
            small.remove(root[color[x]], x);
            big.update(ROOT, color[x]);
            color[x] = c;
            small.add(root[c], x);
            big.update(ROOT, c);
        }
        else
        {
            int c = query[i][1];
            node++;
            small.add(root[c], node);
            big.update(ROOT, c);
            color[node] = c;
        }
        long long ans = big.tree[ROOT].diameterDiff;
        if (ans == (int)-1e9)
            ans = 0;
        cout << ans << endl;
    }

    for (int i = 0; i < n; i++)
        adj[i].clear();
    n = 1;
    INDEX = 0;
    return;
}

signed main()
{
    Blobo2 int qu = nxt();
    while (qu--)
    {
        testCase();
    }
    return 0;
}
