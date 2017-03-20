/* Implementation of a solution to
   the Generalized Palindromic Decomposition with Gaps problem
   in O(n log n * g) time and O(n * g) space complexity
   */

#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<tuple>
#include "PalindromicDecompositionWithGaps.h"

using triple = std::tuple<int, int, int>;
using std::min;
using std::max;
using std::tie;
using std::make_tuple;
using std::string;
using std::vector;
using std::queue;
using std::function;

class PalindromicDecompositionWithGapsFastImpl : public PalindromicDecompositionWithGaps {
    vector<vector<int>> MG, MG1;

    // GPL2 is used similarly as GPL in Fici et al., J. Discr. Alg., 2014
    vector<vector<int>> GPL2;

    public:
    PalindromicDecompositionWithGapsFastImpl(const string &t, function<char(char)> f, int minLength, int maxGapsNum) 
        : PalindromicDecompositionWithGaps(t, f, minLength, maxGapsNum) {}

    int run() {
        init();
        vector<triple> G, G1;
        // G1 is G' from Fici et al., J. Discr. Alg., 2014

        G.clear();
        int i, d, k;
        for (int j = 1; j <= n; ++j) {
            G1 = extendPalindromes(G, j);
            G = fixGapsAndMergeTriples(G1, j);
            G1 = filterPalindromesOnLegth(G, j);
            computeMG(j, G1);
        }

        return MG[n][maxGapsNum] > n ? -1 : MG[n][maxGapsNum];
    }

    void printDecomposition() {
        // TODO
    }

    private:
    void init() {
        MG.resize(n+1);
        MG1.resize(n+1);
        GPL2.resize(maxGapsNum+1);

        for (int j = 0; j <= maxGapsNum; ++j)
            GPL2[j].resize(n+1);

        for (int j = 0; j <= n; ++j) {
            MG[j].resize(maxGapsNum+1);
            MG1[j].resize(maxGapsNum+1);
            for (int q = 0; q <= maxGapsNum; ++q) {
                MG[j][q] = (j==0) ? 0 : INFTY;
                MG1[j][q] = INFTY;
            }
        }
    }

    vector<triple> extendPalindromes(const vector<triple> &G, int j) {
        vector<triple> G1;
        int i, d, k;
        for (const triple &g : G) {
            std::tie(i, d, k) = g;
            if (i > 1 && t[i-1] == f(t[j]))
                G1.push_back(make_tuple(i-1, d, k));
        }
        return G1;
    }


    vector<triple> fixGapsAndMergeTriples(const vector<triple> &G1, int j) {
        int i, d, k, i1, d1, k1;
        vector<triple> G;
        queue<triple> G2;

        int r = -j; // i-r big enough to act as inf
        for (const triple &g : G1) {
            std::tie(i, d, k) = g;
            if (i - r != d) {
                G2.push(make_tuple(i, i-r, 1));
                if (k > 1)
                    G2.push(make_tuple(i+d, d, k-1));
            } else {
                G2.push(make_tuple(i, d, k));
            }
            r = i + (k-1)*d;
        }
        if (j > 1 && t[j-1] == f(t[j])) {
            G2.push(make_tuple(j-1, j-1-r, 1));
            r = j-1;
        }
        if (t[j] == f(t[j]))
            G2.push(make_tuple(j, j-r, 1));

        if (!G2.empty()) {
            std::tie(i1, d1, k1) = G2.front();
            G2.pop();
            while (!G2.empty()) {
                std::tie(i,d,k) = G2.front();
                G2.pop();
                if (d1 == d)
                    k1 += k;
                else {
                    G.push_back(make_tuple(i1,d1,k1));
                    i1 = i, d1 = d, k1 = k;
                }
            }
            G.push_back(make_tuple(i1, d1, k1));
        }
        return G;
    }


    vector<triple> filterPalindromesOnLegth(const vector<triple> &G, int j) {
        vector<triple> G1;
        int maxPos = max(j - minLength + 1, 0);
        int i, d, k;
        for (const triple &g : G) {
            std::tie(i, d, k) = g;

            int nk = (maxPos - i) / d + (i <= maxPos);
            if (k > 1 && nk > 0)
                G1.push_back(make_tuple(i, d, nk));
            if (k == 1 && i <= maxPos)
                G1.push_back(make_tuple(i, d, k));
        }
        return G1;
    }

    void computeMG(int j, vector<triple> G) {
        int i, d, k, r;

        for (int q = 0; q <= maxGapsNum; ++q) {
            if (q > 0)
                MG1[j][q] = min(MG1[j-1][q], MG[j-1][q-1]) + 1;
            MG[j][q] = MG1[j][q];
            for (const triple &g : G) {
                std::tie(i, d, k) = g;

                r = i + (k-1) * d;
                int mn = MG[r-1][q];
                if (k > 1 && d <= i)
                    mn = min(mn, GPL2[q][i - d]);
                if (d <= i)
                    GPL2[q][i-d] = mn;
                MG[j][q] = min(MG[j][q], mn);
            }
        }
    }
};

