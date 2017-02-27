#include<functional>
#include<iostream>
#include<queue>
#include<string>
#include<tuple>
#include<unistd.h>
#include<vector>
#include "MinPalindromicFactorization.h"

using triple = std::tuple<int, int, int>;
using std::make_tuple;
using std::get;
using std::min;
using std::tie;
using std::string;
using std::vector;
using std::queue;
using std::function;

// Implementations of https://arxiv.org/pdf/1403.2431.pdf
class MinPalindromicFactorizationFiciImpl : public MinPalindromicFactorization {
    public:
    int run(const string &s, function<char(char)> f) {
        string t = this->prepareString(s);
        int n = t.size() - 1;

        vector<int> PL(n + 1, 2*n);
        vector<int> GPL(n + 1, 2*n);
        vector<triple> G, G1;
        queue<triple> G2;
        int i, d, k;
        int i1, d1, k1;

        PL[0] = 0;
        G.clear();

        for (int j = 1; j <= n; ++j) {
            G1.clear();
            for (const triple &g : G) {
                tie(i, d, k) = g;
                if (i > 1 && t[i-1] == f(t[j]))
                    G1.push_back(make_tuple(i-1,d,k));
            }

            int r = -j; // i-r big enough to act as inf
            for (const triple &g : G1) {
                tie(i, d, k) = g;
                if (i - r != d) {
                    G2.push(make_tuple(i,i-r,1));
                    if (k > 1)
                        G2.push(make_tuple(i+d, d, k-1));
                } else {
                    G2.push(make_tuple(i,d,k));
                }
                r = i + (k-1)*d;
            }
            if (j > 1 && t[j-1] == f(t[j])) {
                G2.push(make_tuple(j-1, j-1-r, 1));
                r = j-1;
            }
            if (t[j] == f(t[j]))
                G2.push(make_tuple(j, j-r, 1));

            G.clear();
            if (!G2.empty()) {
                tie(i1, d1, k1) = G2.front();
                G2.pop();
                while (!G2.empty()) {
                    tie(i,d,k) = G2.front();
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

            for (const triple &g : G) {
                tie(i, d, k) = g;

                r = i + (k-1) * d;
                int m = PL[r-1] + 1;
                if (k > 1)
                    m = min(m, GPL[i - d]);
                if (d <= i)
                    GPL[i-d] = m;
                PL[j] = min(PL[j], m);
            }
        }
        return PL[n] > n ? -1 : PL[n];
    }
};

