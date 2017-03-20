/* Implementation of a slow solution - O(n^2 * g) - to
   the Generalized Palindromic Decomposition with Gaps problem
*/

#include<iostream>
#include<functional>
#include<vector>
#include<string>
#include "PalindromicDecompositionWithGaps.h"

using std::pair;
using std::make_pair;
using std::string;
using std::vector;
using std::function;

class PalindromicDecompositionWithGapsBruteImpl : public PalindromicDecompositionWithGaps {

    vector<vector<int>> MG, MG1;
    vector<vector<pair<int, int>>> D, D1;

    public:
    PalindromicDecompositionWithGapsBruteImpl(const string &t, function<char(char)> f, int minLength, int maxGapsNum)
        : PalindromicDecompositionWithGaps(t, f, minLength, maxGapsNum) {}

    void init() {
        MG.resize(n+1); MG1.resize(n+1); D.resize(n+1); D1.resize(n+1);

        for (int j = 0; j <= n; ++j) {
            MG[j].resize(maxGapsNum+1);
            MG1[j].resize(maxGapsNum+1);
            D[j].resize(maxGapsNum+1);
            D1[j].resize(maxGapsNum+1);
            for (int q = 0; q <= maxGapsNum; ++q) {
                if (j == 0) {
                    MG[j][q] = 0;
                    D[j][q] = make_pair(0,0);
                } else {
                    MG[j][q] = INFTY;
                    D[j][q] = make_pair(j, q);
                }
                MG1[j][q] = INFTY;
                D1[j][q] = make_pair(j, q);
            }
        }
    }

    int run() {
        init();

        vector<int> P[2];
        P[0].clear();
        for (int j = 1; j <= n; ++j) {
            P[j&1].clear();
            for (int i: P[1-(j&1)])
                if (i > 1 && t[i-1] == f(t[j]))
                    P[j&1].push_back(i-1);

            if (j > 1 && t[j-1] == f(t[j]))
                P[j&1].push_back(j-1);

            if (t[j] == f(t[j]))
                P[j&1].push_back(j);

            for (int q = 0; q <= maxGapsNum; ++q) {
                if (q > 0) {
                    if (MG1[j-1][q] <= MG[j-1][q-1]) {
                        MG1[j][q] = MG1[j-1][q] + 1;
                        D1[j][q] = D1[j-1][q];
                    } else {
                        MG1[j][q] = MG[j-1][q-1] + 1;
                        D1[j][q] = make_pair(j-1, q-1);
                    }
                }
                MG[j][q] = MG1[j][q];
                D[j][q] = D1[j][q];
                for (int i: P[j&1])
                    if (j - i + 1 >= minLength && MG[i-1][q] < MG[j][q]) {
                        MG[j][q] = MG[i-1][q];
                        D[j][q] = make_pair(i-1, q);
                    }
            }
        }
        return MG[n][maxGapsNum] > n ? -1 : MG[n][maxGapsNum];
    }

    void printDecomposition() {
        int prevj = n, prevq = maxGapsNum;
        int j, q;
        j = D[prevj][prevq].first, q = D[prevj][prevq].second;
        vector<int> rev, qs;
        qs.push_back(prevq - q);

        while (prevj != j || prevq != q || (j != 0 && q != 0)) {
            rev.push_back(prevj);
            prevj = j, prevq = q;
            j = D[prevj][prevq].first, q = D[prevj][prevq].second;
            qs.push_back(prevq - q);
        }
        qs.erase(qs.end() - 1);
        rev.push_back(0);
        std::reverse(std::begin(rev), std::end(rev));
        std::reverse(std::begin(qs), std::end(qs));

        for (uint i = 0; i < rev.size() - 1; ++i) {
            std::cout <<
                (!qs[i] ? std::string("") : std::string("[")) <<
                t.substr(rev[i]+1, rev[i+1]-rev[i]) <<
                (!qs[i] ? std::string("") : std::string("]")) <<
                " ";
        }
        std::cout << std::endl;
    }
};

