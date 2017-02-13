/* Implementation of a solution to
   the Generalized Maximal Palindromic Decomposition with Gaps and Erros problem
   in O(n * (g + delta)) time and O(n * g) space
 */

#ifndef MAX_PAL_DECOMP_LCE
#define MAX_PAL_DECOMP_LCE

#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<unistd.h>

using std::vector;
using std::string;
using std::function;

class LCEStructure {
    private:
        string x;
        string y;
        function<char(char)> f;

    public:
        LCEStructure(string t, function<char(char)> f) {
            this->f = f;

            // x = #t
            // y = x + f(t^R)
            this->y = "#";
            this->y.append(t);
            this->x = string(this->y);
            for (auto &c: t) c = f(c);
            this->y.append(t);
            //TODO
        }

        // TODO: implement sth better here
        // LCE - brute
        int LCE(int i, int j) {
            int k = 0, n = this->x.size(), N = this->y.size();
            while (i <= n && j < N && this->y[i] == this->y[j])
                ++i, ++j, ++k;
            return k;
        }

        int LGPal(int i, int j) {
            int k = 0, n = this->x.size();
            while (i > 0 && j <= n && this->f(this->x[i]) == this->x[j]) --i, ++j, ++k;
            return k;
        }

        // TODO: implement LGPal using LCE
};

#endif
