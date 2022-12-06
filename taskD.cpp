#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;
typedef long long ll;

struct pairHash {
    size_t operator()(const pair<int, int> &p) const {
        return p.first ^ p.second;
    }
};

int n, m;
unordered_set<pair<int, int>, pairHash> forest;
unordered_set<pair<int, int>, pairHash> visited;

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void dfs(int x, int y, int &min_x, int &max_x, int &min_y, int &max_y) {
    visited.insert({x, y});

    min_x = min(min_x, x);
    max_x = max(max_x, x);
    min_y = min(min_y, y);
    max_y = max(max_y, y);

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if (nx >= 0 && nx < m && ny >= 0 && ny < n && visited.find({nx, ny}) == visited.end() &&
            forest.find({nx, ny}) != forest.end()) {
            dfs(nx, ny, min_x, max_x, min_y, max_y);
        }
    }
}

void solve() {
    int L;
    cin >> m >> n >> L;

    // use of rectangular 2d grid would allocate over 16GiB of memory :)
    // so we use unordered_set instead to represent individual trees in forest
    forest = unordered_set<pair<int, int>, pairHash>();

    for (int i = 0; i < L; i++) {
        int x, y;
        cin >> x >> y;
        forest.insert({x, y});
    }

    visited = unordered_set<pair<int, int>, pairHash>();
    int x, y;
    int min_x, max_x, min_y, max_y;
    ll total = 0;

    // for each tree, find the bounding box of the forest
    for (const auto &p: forest) {
        x = p.first;
        y = p.second;

        if (visited.find({x, y}) == visited.end()) {
            min_x = x;
            max_x = x;
            min_y = y;
            max_y = y;
            dfs(x, y, min_x, max_x, min_y, max_y);

            // minimum and maximum x and y coordinates correspond to the vertices of a rectangle, containing the forest
            total += (max_x - min_x + 1) * (max_y - min_y + 1);
        }
    }

    cout << total << endl;
}

int main(int argc, char *argv[]) {
    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        solve();
    }

    return 0;
}