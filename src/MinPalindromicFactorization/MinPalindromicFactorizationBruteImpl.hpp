// Naive O(n^2) implementation of a Simple Quadratic Algorithm presented in https://arxiv.org/pdf/1403.2431.pdf

#include<functional>
#include<string>
#include<vector>
#include "MinPalindromicFactorization.h"

using std::min;
using std::string;
using std::vector;
using std::function;

class MinPalindromicFactorizationBruteImpl : public MinPalindromicFactorization {
    public:
    int run(const string &s, function<char(char)> f) {
        string t = this -> prepareString(s);

        int n = t.size() - 1;
        vector<int> PL(n+1, 2*n);
        vector<int> P[2];

        PL[0] = 0;
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

            for (int i: P[j&1])
                PL[j] = min(PL[j], PL[i-1] + 1);
        }
        return PL[n] > n ? -1 : PL[n];
    }
};

