#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>

using namespace std;
typedef unsigned long long ll;

struct pairHash {
    size_t operator()(const pair<int, int> &p) const {
        return p.first ^ p.second;
    }
};

class DSU {
    vector<int> parent;
    vector<int> rank;

public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n);

        for (int i = 0; i < n; i++) {
            rank[i] = 0;
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }

        return parent[x];
    }

    void unite(int x, int y) {
        x = find(x);
        y = find(y);

        if (rank[x] < rank[y]) {
            parent[x] = y;
        } else {
            parent[y] = x;

            if (rank[x] == rank[y]) {
                rank[x]++;
            }
        }
    }
};

vector<vector<int>> adj_list;
unordered_set<int> problems;

int N, M, K;

void dfs(int i, vector<bool> &visited) {
    visited[i] = true;

    for (int j: adj_list[i]) {
        if (!visited[j] && problems.find(j) == problems.end()) {
            dfs(j, visited);
        }
    }
}

bool is_fully_connected() {
    vector<bool> visited(N, false);
    int vertex = 0;

    while (problems.find(vertex) != problems.end()) {
        vertex++;
    }

    dfs(vertex, visited);

    for (int i = 0; i < N; i++) {
        if (!visited[i] && problems.find(i) == problems.end()) {
            return false;
        }
    }

    return true;
}

void solve() {
    cin >> N >> M >> K;

    unordered_map<pair<int, int>, int, pairHash> edges{};
    adj_list = vector<vector<int>>(N);
    problems = unordered_set<int>();

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        u--;
        v--;

        if (u > v) {
            swap(u, v);
        }

        edges[{u, v}] = c;
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }

    ll result = 0;

    for (int i = 0; i < K; i++) {
        int p;
        cin >> p;

        problems.insert(--p);
    }

    // remove problems from graph, then calculate minimum spanning tree
    for (const auto &p: problems) {
        int min_cost = 1e9;
        bool impossible = true;

        for (const auto &v: adj_list[p]) {
            if (problems.find(v) == problems.end()) {
                min_cost = min(min_cost, edges[{min(p, v), max(p, v)}]);

                impossible = false;
            }
        }

        // problems are only connected between each other
        if (impossible) {
            cout << -1 << endl;
            return;
        }

        // add the cost of problem to the result
        result += min_cost;
    }

    // removing problems created disconnected graph => problems need at least two edges (repairs)
    if (!is_fully_connected()) {
        cout << -1 << endl;
        return;
    }

    // perform Kruskal's algorithm to find minimum spanning tree
    DSU mst(N);

    vector<pair<pair<int, int>, int>> sorted_edges(edges.begin(), edges.end());
    sort(sorted_edges.begin(), sorted_edges.end(), [](const auto &a, const auto &b) {
        return a.second < b.second;
    });

    for (const auto &edge: sorted_edges) {
        int u = edge.first.first;
        int v = edge.first.second;
        int c = edge.second;

        if (problems.find(u) != problems.end() || problems.find(v) != problems.end()) {
            continue;
        }

        if (mst.find(u) != mst.find(v)) {
            mst.unite(u, v);
            result += c;
        }
    }

    cout << result << endl;
}

int main(int argc, char *argv[]) {
    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        solve();
    }

    return 0;
}