#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <array>
#include <queue>
#include <deque>
#include <climits>
#include <functional>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef unordered_map<int, vector<int>> adjList;

int M, N, K;
adjList adj;
vector<int> costs;
vector<vector<int>> shortest;

// modified dijkstra to find ALL the shortest paths
void dijkstra(int source, int target) {
    std::priority_queue<pii, vector<pii>, greater<>> pq;
    vector<int> dist(N, INT_MAX);

    // store the parents of each visited node
    // only the nodes leading to the shortest path will be visited
    adjList prevs;

    pq.emplace(costs[source], source);
    dist[source] = costs[source];

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        for (const auto &v: adj[u]) {
            int cost = d + costs[v];

            if (dist[v] >= cost) {
                if (dist[v] > cost) {
                    dist[v] = cost;
                    // better path found, clear the previous parents
                    prevs[v].clear();
                }

                prevs[v].push_back(u);
                pq.emplace(dist[v], v);
            }
        }
    }

    // make undirected graph from the parents
    adjList shortest_adj;
    for (auto &p: prevs) {
        for (auto &v: p.second) {
            shortest_adj[p.first].push_back(v);
            shortest_adj[v].push_back(p.first);
        }
    }

    vector<bool> visited(N, false);
    vector<int> path;

    // find all possible paths from source to target. !!EXPONENTIAL TIME - NP HARD!!
    function<void(int, int)> dfs = [&](int s, int d) {
        visited[s] = true;
        path.push_back(s);
        if (s == d) {
            shortest.push_back(path);
        } else {
            for (const auto &v: shortest_adj[s]) {
                if (!visited[v]) {
                    dfs(v, d);
                }
            }
        }
        path.pop_back();
        visited[s] = false;
    };

    dfs(source, target);
}

// finding all shortest paths using bfs on unweighted graph is polynomial
void bfsShortest(int source, int target) {
    deque<int> q = {source};
    unordered_map<int, int> depth = {{source, 1}};

    while (!q.empty()) {
        int u = q.front();
        q.pop_front();

        if (u == target) {
            break;
        }

        for (const auto &v: adj[u]) {
            if (depth[v] == 0) {
                depth[v] = depth[u] + 1;
                q.push_back(v);
            }
        }
    }

    vector<int> path;

    function<void(int, int)> dfs = [&](int s, int d) {
        path.push_back(s);
        if (s == d) {
            shortest.push_back(path);
        } else {
            for (const auto &v: adj[s]) {
                // the critical line - we only visit the nodes that are one step closer to the target, making the time complexity polynomial
                if (depth[v] == depth[s] + 1) {
                    dfs(v, d);
                }
            }
        }
        path.pop_back();
    };

    dfs(source, target);
}

// NP hard but it doesn't matter because K is small and the shortest paths are short
bool comb(int offset, int k, vector<int> &unique, vector<bool> &kBelonging) {
    if (k == 0) {
        // check if the combination is valid
        for (const auto &path: shortest) {
            int kSlept = 0;
            int kNotSlept = 0;
            bool satisfied = false;

            for (const auto &v: path) {
                if (kBelonging[v]) {
                    kNotSlept = 0;
                    kSlept++;
                } else {
                    kNotSlept++;
                    kSlept = 0;
                }

                if (kSlept == 2 || kNotSlept == 2) {
                    satisfied = true;
                    break;
                }
            }

            if (!satisfied) {
                return false;
            }
        }

        return true;
    }
    // recursively find all possible combinations
    for (int i = offset; i <= unique.size(); ++i) {
        kBelonging[unique[i]] = true;
        if (comb(i + 1, k - 1, unique, kBelonging)) {
            return true;
        }
        kBelonging[unique[i]] = false;
    }

    return false;
}

void printTrivial(int n, unordered_set<int> &unique) {
    int k = 0;

    for (int i = 0; i < N; i++) {
        if (unique.find(i) == unique.end()) {
            cout << i + 1 << " ";
            k++;
        }

        if (k == n) {
            cout << endl;
            return;
        }
    }
}

void solve() {
    cin >> N >> M >> K;

    costs = vector<int>(M);
    adj = adjList(M);
    shortest = vector<vector<int>>();

    for (int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    bool easyMode = true;

    for (int i = 0; i < N; i++) {
        cin >> costs[i];

        if (easyMode && costs[i] != 1) {
            easyMode = false;
        }
    }

    // dijkstra with DFS won't work for easy mode as finding all the shortest paths in weighted graph is NP HARD (exponential time)
    if (easyMode) {
        bfsShortest(0, N - 1);
    } else {
        dijkstra(0, N - 1);
    }

    unordered_set<int> uniqueSet;

    for (const auto &path: shortest) {
        for (const auto &v: path) {
            if (uniqueSet.find(v) == uniqueSet.end()) {
                uniqueSet.insert(v);
            }
        }
    }

    int unvisited = N - uniqueSet.size();
    int minK = K - unvisited;

    // there are more unvisited hotels than K => give all hotels to Kasi and valid solution is guaranteed
    if (minK <= 0) {
        printTrivial(K, uniqueSet);
        return;
    }

    vector<bool> kBelonging(N, false);
    vector<int> uniqueVec(uniqueSet.begin(), uniqueSet.end());
    bool possible = false;

    // try to find a combination of hotels that is valid, first allocating as many unvisited hotels as possible to Kasi
    // to reduce the number of combinations to check. (if impossible, give one unvisited hotel back to Opea and try again
    while (minK <= K) {
        if (comb(0, minK, uniqueVec, kBelonging)) {
            possible = true;
            break;
        }
        minK++;
    }

    if (possible) {
        for (int i = 0; i < N; i++) {
            if (kBelonging[i]) {
                cout << i + 1 << " ";
            }
        }

        printTrivial(K - minK, uniqueSet);
    } else {
        cout << "-1" << endl;
    }
}

int main(int argc, char *argv[]) {
    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        solve();
    }

    return 0;
}