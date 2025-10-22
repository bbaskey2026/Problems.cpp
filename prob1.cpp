// Compile with: g++ -O2 -std=c++17 centroid_pairs.cpp -o centroid_pairs
#include <bits/stdc++.h>
using namespace std;

int N;
long long K;
vector<vector<int>> adj;
vector<int> subsize;
vector<bool> removed;

void dfs_size(int u, int p) {
    subsize[u] = 1;
    for (int v: adj[u]) if (v != p && !removed[v]) {
        dfs_size(v, u);
        subsize[u] += subsize[v];
    }
}

int find_centroid(int u, int p, int total) {
    for (int v: adj[u]) if (v != p && !removed[v]) {
        if (subsize[v] > total/2) return find_centroid(v, u, total);
    }
    return u;
}

void collect_distances(int u, int p, int depth, vector<int> &dst) {
    dst.push_back(depth);
    for (int v: adj[u]) if (v != p && !removed[v]) {
        collect_distances(v, u, depth + 1, dst);
    }
}

long long count_pairs_with_limit(vector<int> &arr, long long limit) {
    sort(arr.begin(), arr.end());
    long long cnt = 0;
    int i = 0, j = (int)arr.size() - 1;
    while (i < j) {
        if ((long long)arr[i] + arr[j] <= limit) {
            cnt += (j - i); // pairs (i, i+1..j)
            i++;
        } else {
            j--;
        }
    }
    return cnt;
}

long long solve_subtree(int start) {
    // compute sizes and centroid
    dfs_size(start, -1);
    int c = find_centroid(start, -1, subsize[start]);
    // For centroid c, gather distances from c to nodes in each child-subtree
    vector<int> allDistances;
    long long totalPairs = 0;
    // include centroid itself at distance 0? Usually pairs (u,v) u != v; distances from centroid include 0,
    // but pairs counting logic should treat only pairs of nodes; centroid pairs with nodes counted via distances.
    allDistances.push_back(0); // distance from centroid to itself, helps counting pairs where one end is centroid

    vector<vector<int>> lists;
    for (int v: adj[c]) if (!removed[v]) {
        vector<int> dlist;
        collect_distances(v, c, 1, dlist); // distances starting at 1
        if (!dlist.empty()) {
            lists.push_back(dlist);
            for (int x: dlist) allDistances.push_back(x);
        }
    }

    // Count total pairs through centroid using combined list
    totalPairs += count_pairs_with_limit(allDistances, K);

    // Subtract pairs that lie entirely inside single child-subtrees (they were counted in totalPairs but
    // should not be counted here; they will be counted when processing that subtree's centroid)
    for (auto &lst : lists) {
        totalPairs -= count_pairs_with_limit(lst, K);
    }

    // mark centroid removed and recurse on components
    removed[c] = true;
    for (int v: adj[c]) if (!removed[v]) {
        totalPairs += solve_subtree(v);
    }
    // restore removed? no, keep it removed for this decomposition
    return totalPairs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> N >> K;
    adj.assign(N+1, {});
    for (int i = 0; i < N-1; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    subsize.assign(N+1, 0);
    removed.assign(N+1, false);
    long long ans = solve_subtree(1);
    cout << ans << "\n";
    return 0;
}
