#include "MaximalGPalindromes/LGPal.hpp"
#include "./relations.hpp"

#include <functional>
#include <iostream>
#include <string>

using namespace std;

int main() {
    function<char(char)> f = relation::equality;

    string in;
    while (cin >> in) {
        LGPal lgpal(in, f);
        for (uint i = 1; i <= in.size(); ++i)
            for (uint j = i; j <= in.size(); ++j) {
                int brute = lgpal.brute(i, j);
                int fast = lgpal(i, j);
                if (brute != fast)
                {
                    cout << "(" << i << ", " << j << "): " << brute << " vs " << fast << endl;
                }
            }
    }
    return 0;
}
