// TODO: rename from LCE
// TODO: comment here
#ifndef MAX_PAL_DECOMP_LCE
#define MAX_PAL_DECOMP_LCE

#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<unistd.h>

#include <sdsl/suffix_arrays.hpp>
#include <sdsl/lcp.hpp>
#include <sdsl/rmq_support.hpp>

using std::vector;
using std::string;
using std::function;

using namespace sdsl;


class LGPal {
    private:
        int n;
        string x; // x = #t
        string y; // y = #t$f(t^R)
        function<char(char)> f;
        csa_bitcompressed<> csa;
        lcp_bitcompressed<> lcp;
        rmq_succinct_sct<> rmq;

    public:
        LGPal(string t, function<char(char)> f) : n(t.size()), f(f) {
            // construct string x = #t (for brute) and y = #t$f(t^R)
            y = "#";
            y.append(t);
            x = string(this->y);
            y.append("$");
            for (auto &c: t) c = f(c);
            std::reverse(std::begin(t), std::end(t));
            y.append(t);

            // construct structures for LCE queries
            construct_im(csa, y, 1); // suffix array
            construct_im(lcp, y, 1); // longest common prefix
            rmq = rmq_succinct_sct<>(&lcp); // range minimum query over sequence of LCP
        }

        int brute(int i, int j) const {
            int k = 0;
            while (i > 0 && j <= n && f(x[i]) == x[j]) --i, ++j, ++k;
            return k;
        }

        int fast(int i, int j) const {
            return LCE(2*n + 2 - i, j);
        }

        int operator()(int i, int j) const {
            return LCE(2*n + 2 - i, j);
        }

    private:
        // Compute LCE(i, j) using inverted suffix array, longest common prefix and range-minimum queries
        // https://pdfs.semanticscholar.org/55c7/218e5e1e9fcddaa7bebeb6badabd2d5ae7fb.pdf

        inline int LCE(int i, int j) const {
            int a = csa.isa[i];
            int b = csa.isa[j];
            int mn, mx;
            if (a < b)  {
                mn = a + 1;
                mx = b;
            } else {
                mn = b + 1;
                mx = a;
            }
            return lcp[rmq(mn, mx)];
        }
};

#endif
