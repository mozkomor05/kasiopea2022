#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;

// use SegmentTree for fast interval merge queries
class SegmentTree {
    unordered_map<ull, ull> sum;
    unordered_map<ull, ull> tag;
    ull N;

public:
    explicit SegmentTree(ull n) {
        N = n;
    }

    void update(ull l, ull r, int val) {
        _update(1, 0, N, l, r, val);
    }

    ull query() {
        return sum[1];
    }

private:
    void _update(ull v, ull l, ull r, ull ql, ull qr, int val) {
        if (ql <= l && r <= qr) {
            tag[v] += val;
        } else {
            ull mid = (l + r) / 2;
            if (qr <= mid) {
                _update(v * 2, l, mid, ql, qr, val);
            } else if (mid <= ql) {
                _update(v * 2 + 1, mid, r, ql, qr, val);
            } else {
                _update(v * 2, l, mid, ql, mid, val);
                _update(v * 2 + 1, mid, r, mid, qr, val);
            }
        }

        if (tag[v]) {
            sum[v] = r - l;
        } else {
            sum[v] = (l + 1 == r ? 0 : sum[v * 2] + sum[v * 2 + 1]);
        }
    }
};

void solve() {
    int N, K;
    cin >> N >> K;

    // imagine a line sweeping from bottom to top, each event corresponds to the line passing through a point
    vector<tuple<ull, ull, ull, int>> events(2 * N);

    for (int i = 0; i < N; i++) {
        ll sx, sy;
        cin >> sx >> sy;

        // convert to unsigned numbers
        ull x = sx + 1e9;
        ull y = sy + 1e9;

        // square coordinates after K steps
        ull Lx = x - K, Ly = y - K;
        ull Rx = x + K + 1, Ry = y + K + 1;

        events[2 * i] = {Ly, Lx, Rx, 1}; // 1 = square starts
        events[2 * i + 1] = {Ry, Lx, Rx, -1}; // -1 = square ends
    }

    sort(events.begin(), events.end());

    SegmentTree tree(1e16 + 1);
    ull prev_y = 0;
    ull result = 0;

    // line sweep from bottom to top implementation
    for (const auto [y, l, r, val]: events) {
        // y - prev_y: height of the current square
        // tree.query(): TOTAL width of all squares that the line currently passes through
        result += (y - prev_y) * tree.query();
        tree.update(l, r, val);
        prev_y = y;
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