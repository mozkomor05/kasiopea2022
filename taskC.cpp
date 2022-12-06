#include <iostream>
#include <unordered_map>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

void solve() {
    int n;
    cin >> n;

    unordered_map<string, ull> freq;
    for (int i = 0; i < n; i++) {
        string s;
        ull k;
        cin >> s >> k;
        freq[s] += k;
    }

    cout << freq.size() << endl;
    for (const auto &p: freq) {
        cout << p.first << " " << p.second << endl;
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
