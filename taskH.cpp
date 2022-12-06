#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct assignment {
    int S;
    vector<int> path;
};

void solve(int _) {
    int n, k;
    cin >> n >> k;

    vector<int> S(k);
    int size = 1 << k;
    unordered_map<int, assignment> dp;
    dp[0] = {0, {}};
    unordered_map<int, assignment> newDp;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            cin >> S[j];
        }

        newDp.clear();

        for (const auto &p: dp) {
            for (int j = 0; j < k; j++) {
                int newMask = p.first | (1 << j);
                int newS = p.second.S + S[j];

                if (newS > newDp[newMask].S) {
                    vector<int> newPath = p.second.path;
                    newPath.push_back(j);
                    newDp[newMask] = {newS, newPath};
                }
            }
        }

        dp = newDp;
    }

    assignment res = dp[size - 1];
    cout << res.S << endl;

    for (int i: res.path) {
        cout << i + 1 << " ";
    }
    cout << endl;
}

int main() {
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        solve(i);
    }
    return 0;
}