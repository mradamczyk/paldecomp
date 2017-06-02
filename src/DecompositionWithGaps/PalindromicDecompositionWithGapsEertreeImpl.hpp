/* Implementation of a solution to 
   the Generalized Palindromic Decomposition with Gaps problem
   in O(n log n * g) time and O(n * g) space complexity
   using a modified Eertree structure presented by  M. Rubinchik and A. M. Shur in
    "EERTREE: an efficient data structure for processing palindromes in strings"
     Combinatorial Algorithms - 26th International Workshop, IWOCA 2015
*/

#include<algorithm>
#include<stdexcept>
#include<functional>
#include<vector>
#include<queue>
#include<string>
#include<tuple>
#include "Eertree.hpp"
#include "PalindromicDecompositionWithGaps.h"

using std::min;
using std::max;
using std::string;
using std::vector;
using std::function;

class PalindromicDecompositionWithGapsEertreeImpl : public PalindromicDecompositionWithGaps {
    vector<vector<int>> MG, MG1;

    // dp2 is used similarly as dp in M. Rubinchik, A. M. Shur palindromic length solution
    vector<vector<int>> dp2;
    Eertree eertree;

    public:
    PalindromicDecompositionWithGapsEertreeImpl(const string &t, function<char(char)> f, int minLength, int maxGapsNum)
        : eertree(26, minLength, f), PalindromicDecompositionWithGaps(t, f, minLength, maxGapsNum) {
        }

    int run() {
        init();
        for (int j = 1; j <= n; ++j) {
            eertree.add_letter(t[j]);
            computeMG(j);
        }

        return MG[n][maxGapsNum] > n ? -1 : MG[n][maxGapsNum];
    }

    void printDecomposition() {
        throw std::logic_error("Unimplemented method");
    }

    private:
    void init() {
        MG.resize(n+1);
        MG1.resize(n+1);
        dp2.resize(maxGapsNum+1);

        for (int j = 0; j <= maxGapsNum; ++j)
            dp2[j].resize(n+1);

        for (int j = 0; j <= n; ++j) {
            MG[j].resize(maxGapsNum+1);
            MG1[j].resize(maxGapsNum+1);
            for (int q = 0; q <= maxGapsNum; ++q) {
                MG[j][q] = (j==0) ? 0 : INFTY;
                MG1[j][q] = INFTY;
                dp2[q][j] = INFTY;
            }
        }
    }

    void computeMG(int j) {
        int i, d, k, r;

        for (int q = 0; q <= maxGapsNum; ++q) {
            if (q > 0)
                MG1[j][q] = min(MG1[j-1][q], MG[j-1][q-1]) + 1;
            MG[j][q] = MG1[j][q];
            for (Node* v = eertree.last; v->len >= minLength; v = v->slink) {
                dp2[q][v->id] = MG[j - (v->slink->len + v->diff)][q];
                if (v->diff == v->link->diff) {
                    dp2[q][v->id] = min(dp2[q][v->id], dp2[q][v->link->id]);
                }
                MG[j][q] = min(MG[j][q], dp2[q][v->id]);
            }
        }
    }
};

