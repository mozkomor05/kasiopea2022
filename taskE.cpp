#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;
typedef unsigned long long ll;

void solve() {
    int N, K;
    cin >> N >> K;

    unordered_map<int, ll> sums{};
    vector<int> pictograms(20);

    for (int i = 0; i < N; i++) {
        int pic_num;
        cin >> pic_num;

        for (int j = 0; j < pic_num; j++) {
            int pictogram;
            cin >> pictogram;

            // compute Hamming weight of pictogram (number of 1s)
            int population = 0;
            while (pictogram) {
                population++;
                pictogram &= pictogram - 1;
            }

            pictograms[j] = population;
        }

        // pre-compute all possible sums as we introduce new pictograms
        if (i == 0) {
            for (int j = 0; j < pic_num; j++) {
                sums[pictograms[j]] += 1;
            }
        } else {
            unordered_map<int, ll> new_sums{};

            for (const auto &sum: sums) {
                for (int j = 0; j < pic_num; j++) {
                    int new_sum = sum.first + pictograms[j];
                    if (new_sum <= K) {
                        new_sums[new_sum] += sum.second;
                    }
                }
            }

            // update sums with the new combinations
            sums = new_sums;
        }
    }


    ll result = 0;

    // after all pictograms are loaded, sums[K] contains the number of ways to add up to K
    if (sums.find(K) != sums.end()) {
        result += sums[K];
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