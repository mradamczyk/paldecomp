#include<algorithm>
#include<functional>
#include<vector>
#include<string>
#include<unistd.h>
#include "LCEStructure.hpp"

using std::pair;
using std::make_pair;
using std::vector;
using std::string;
using std::function;

class MaximalGPalindromes {
    private:
        int n;
        LCEStructure q;

    public:
        MaximalGPalindromes(const string &t, function<char(char)> f)
            : n(t.size()), q(LCEStructure(t, f)) {}

        // Finding all maximal g-palindromes under Hamming dist
        vector< pair<int, int> > allMaximalHammingDistGPalindromes(int g) {
            vector< pair<int, int> > P;

            int d;
            for (int i = 1; i <= n; ++i) {
                for (int k = 0; k < 2 && i + k <= n; ++k) {
                    d = maximalGHammingPalindrome(i, i+k, g);
                    P.push_back(make_pair(i-d+1, i+k+d-1));
                }
            }
            return P;
        }

        // Finding all maximal g-palindromes under Edit dist
        vector< pair<int, int> > allMaximalEditDistGPalindromes(int g) {
            vector< pair<int, int> > P;
            vector< pair<int, int> > buckets(2*n+1, make_pair(0,-1));

            // all max palindromes without errors
            P = allMaximalHammingDistGPalindromes(0);
            int d, i, j, i1, j1;
            for (int k = 1; k <= g; ++k) {
                // try to extend palindromes
                for (auto &p: P) {
                    i = p.first, j = p.second;
                    for (const pair<int,int> &c: vector< pair<int, int> >{{1,0}, {0,1}, {1,1}}) {
                        d = q.LGPal(i-1-c.first, j+1+c.second);
                        i1 = i - c.first - d, j1 = j + c.second + d;
                        addPalToBucketIfLonger(make_pair(i1, j1), buckets);
                    }
                    // border reductions
                    if (i == 1)  addPalToBucketIfLonger(make_pair(1, j-1), buckets);
                    if (j == n)  addPalToBucketIfLonger(make_pair(i+1, n), buckets);
                }

                P.clear();
                // sort by center positions
                for (int i = 2; i <= 2*n; ++i) {
                    if (dist(buckets[i]) > 0) {
                        P.push_back(buckets[i]);
                        buckets[i] = make_pair(0, -1);
                    }
                }
            }
            return P;
        }

    private:
        // Helper function
        // Returns length of word between first and second position
        inline int dist(const pair<int, int> &p) {
            return p.second - p.first + 1;
        }

        // Helper function
        // Compares length of palindrome with longest in this palindrome bucket, replace it when longer
        void addPalToBucketIfLonger(const pair<int, int> &temp, vector<pair<int, int>> &buckets) {
            int mid = temp.first + temp.second;
            if (temp.first >= 1 && temp.second <= n && mid <= 2*n && dist(buckets[mid]) < dist(temp)) {
                buckets[mid] = temp;
            }
        }

        // Finding maximal g-palindrome centered between i and j under Hamming dist
        // For an even-length g-palindrome we set j=i+1
        // For an odd-length g-palindrome we set j=i -- run only when f(t[i])=t[i]
        int maximalGHammingPalindrome(int i, int j, int g) {
            int a = i, b = j, d;
            for (int s = 0; s <= g; ++s) {
                if (a == 0 || b == n+1) return i - a;
                d = q.LGPal(a, b);
                a -= d;
                b += d;
                if (a == 0 || b == n+1) return i - a;
                if (s < g)
                    --a, ++b;
            }
            return i - a;
        }
};
