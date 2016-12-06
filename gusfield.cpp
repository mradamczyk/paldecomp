#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>

inline char standardPalindrom(const char &t) { return t; }
inline char dnaComplementaryPalindrom(const char &t) {
    switch (t) {
        case 'A': return 'T';
        case 'C': return 'G';
        case 'G': return 'C';
        case 'T': return 'A';
    }
    return t;
}

using std::min;
using std::make_pair;
using std::pair;
using std::vector;
using std::string;

inline int dist(const pair<int, int> &p) {
    return p.second - p.first + 1;
}

class LCEStructure {
private:
    std::string s;
    std::string t;
    std::function<char(char)> f;
    int N;

public:
    LCEStructure(std::string t, std::function<char(char)> f) {
        this->f = f;

        // s = t + f(t^R)
        this->s = "#";
        this->s.append(t);
        this->t = std::string(this->s);
        for (auto &c: t) c = f(c);
        std::reverse(std::begin(t), std::end(t));
        this->s.append(t);
        std::cout << "string s: " << this->s << std::endl;
        this->N = this->s.size() - 1;
    }

    // TODO: implement sth better here
    // LCE - brute
    int LCE(int i, int j) {
        int k = 0, n = this->t.size();
        while (i <= n && j < this->s.size() && this->s[i] == this->s[j])
            ++i, ++j, ++k;
        return k;
    }

    inline int LCEvenPalindrom(int i) {
        return LCE(i+1, this->N-i+1);
    }

    inline int LCOddPalindrom(int i) {
        return LCE(i, this->N-i+1);
    }

    // set j=i for odd-palindromes and j=i+1 for even palindromes
    inline int maxPal(int i, int j) {
        return LCE(j, this->N-i+1);
    }

    int LCPal(int i, int j) {
        int k = 0, n = this->t.size();
        while (i > 0 && j <= n && this->f(this->t[i]) == this->t[j]) --i, ++j, ++k;
        return k;
    }

    // Finding maximal g-palindrome centered between i and j under Hamming dist
    // For an even-length g-palindrome we set j=i+1
    // For an odd-length g-palindrome we set j=i -- run only when f(t[i])=t[i]
    int maxPalHam(int i, int j, int g) {
        int a = i, b = j, d;
        int n = this->t.size() - 1;
        for (int s = 0; s <= g; ++s) {
            if (a == 0 || b == n+1) return i - a;
            d = LCPal(a, b);
            a -= d;
            b += d;
            if (a == 0 || b == n+1) return i - a;
            if (s < g)
                --a, ++b;
        }
        return i - a;
    }

    vector< pair<int, int> > allMaxPalHam(int g) {
        int n = this->t.size() - 1;
        vector< pair<int, int> > P;
        // TODO: verify
        int dEven, dOdd;
        for (int i = 1; i <= n; ++i) {
            dEven = maxPalHam(i, i+1, g);
            dOdd = maxPalHam(i, i, g);
            if (dOdd > dEven)
                P.push_back(make_pair(i-dOdd+1, i+dOdd-1));
            else
                P.push_back(make_pair(i-dEven+1, i+dEven));
        }
        return P;
    }

    // Finding all maximal g-palindrome centered under Edit dist
    // For an even-length g-palindrome we set j=i+1
    // For an odd-length g-palindrome we set j=i -- run only when f(t[i])=t[i]
    vector< pair<int, int> > allMaxPalEdit(int g) {
        int n = this->t.size() - 1;
        vector< pair<int, int> > P;
        vector< pair<int, int> > buckets(n+1, make_pair(0,-1));

        int dEven, dOdd;
        for (int i = 1; i <= this->N; ++i) {
            dEven = maxPal(i, i+1);
            dOdd = maxPal(i, i);
            if (dOdd > dEven)
                P.push_back(make_pair(i-dOdd+1, i+dOdd-1));
            else
                P.push_back(make_pair(i-dEven+1, i+dEven));
        }

        int d, i, j, i1, j1, mid;
        pair<int, int> temp;
        for (int k = 1; k <= g; ++k) {
            // try to extend palindromes
            for (auto &p: P) {
                i = p.first, j = p.second;
                for (const pair<int,int> &c: vector< pair<int, int> >{{1,0}, {0,1}, {1,1}}) {
                    d = LCPal(i-1-c.first, j+1+c.second);
                    i1 = i - d - c.second, j1 = j + d + c.second;
                    temp = make_pair(i1, j1);
                    mid = (i1+j1) / 2;
                    if (i1 >= 1 && j1 <= this->N) {
                         if (dist(buckets[mid]) < dist(temp))
                            buckets[mid] = temp;
                    }
                }
                // TODO: add border reductions!!!
            }

            P.clear();
            // sort by center positions
            for (int i = 1; i <= this->N; ++i) {
                if (dist(buckets[i]) > 0) {
                    P.push_back(buckets[i]);
                    buckets[i] = make_pair(0, -1);
                }
            }
        }
        return P;
    }

};

int maxPalFact(const string &t, const vector< pair<int, int> > &Pal, int minLength, int maxGaps) {
    int n = t.size() - 1;
    std::vector<std::vector<int>> MG(n+1), MG1(n+1);
    for (int j = 0; j <= n; ++j) {
        MG[j].resize(maxGaps+1);
        MG1[j].resize(maxGaps+1);
        for (int q = 0; q <= maxGaps; ++q)
            MG[j][q] = MG1[j][q] = (j == 0 ? 0 : 2*n);
    }

    vector<vector<int> > P(n+1);
    for (auto &p: Pal) P[p.second].push_back(p.first);

    for (int j = 1; j <= n; ++j) {
        for (int q = 0; q <= maxGaps; ++q) {
            MG1[j][q] = MG1[j-1][q] + 1;
            if (q > 0)
                MG1[j][q] = min(MG1[j][q], MG[j-1][q-1]+1);

            MG[j][q] = MG1[j][q];
            for (int i: P[j])
                if (j - i + 1>= minLength)
                    MG[j][q] = min(MG[j][q], MG[i-1][q]);
        }
    }
    return MG[n][maxGaps];
}

int main() {
    std::string t;
    std::cin >> t;
    for (auto &c: t) c = toupper(c);

    LCEStructure Q(t, standardPalindrom);
    int n = t.size();
    for (int i = 1; i <= n; ++i) {
        std::cout << i << "#: " << Q.maxPal(i, i+1) << " " << Q.maxPal(i, i) << std::endl;
        std::cout << i << "$: " << Q.LCEvenPalindrom(i) << " " << Q.LCOddPalindrom(i) << std::endl;
    }
    for (int d = 0; d < 4; ++d) {
        std::cout << d << "-palindromes" << std::endl;
        for (int i = 1; i <= n; ++i)
            std::cout << i << ": " << Q.maxPalHam(i, i+1, d) << " " << Q.maxPalHam(i, i, d) << std::endl;
    }
    return 0;
}
