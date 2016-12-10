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
    }

    // TODO: implement sth better here
    // LCE - brute
    int LCE(int i, int j) {
        int k = 0, n = this->t.size(), N = this->s.size();
        while (i <= n && j < N && this->s[i] == this->s[j])
            ++i, ++j, ++k;
        return k;
    }

    int LGPal(int i, int j) {
        int k = 0, n = this->t.size();
        while (i > 0 && j <= n && this->f(this->t[i]) == this->t[j]) --i, ++j, ++k;
        return k;
    }

    // TODO: implement LGPal using LCE

    // Finding maximal g-palindrome centered between i and j under Hamming dist
    // For an even-length g-palindrome we set j=i+1
    // For an odd-length g-palindrome we set j=i -- run only when f(t[i])=t[i]
    int maxPalHam(int i, int j, int g) {
        int a = i, b = j, d;
        int n = this->t.size() - 1;
        for (int s = 0; s <= g; ++s) {
            if (a == 0 || b == n+1) return i - a;
            d = LGPal(a, b);
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

        int d;
        for (int i = 1; i <= n; ++i) {
            for (int k = 0; k < 2 && i + k <= n; ++k) {
                d = maxPalHam(i, i+k, g);
                P.push_back(make_pair(i-d+1, i+k+d-1));
            }
        }
        return P;
    }

    // Finding all maximal g-palindrome centered under Edit dist
    // For an even-length g-palindrome we set j=i+1
    // For an odd-length g-palindrome we set j=i -- run only when f(t[i])=t[i]
    vector< pair<int, int> > allMaxPalEdit(int g) {
        int n = this->t.size() - 1;
        vector< pair<int, int> > P;
        vector< pair<int, int> > buckets(2*n+1, make_pair(0,-1));

        // all max palindromes without errors
        P = allMaxPalHam(0);

        int d, i, j, i1, j1;
        for (int k = 1; k <= g; ++k) {
            // try to extend palindromes
            for (auto &p: P) {
                i = p.first, j = p.second;
                for (const pair<int,int> &c: vector< pair<int, int> >{{1,0}, {0,1}, {1,1}}) {
                    d = LGPal(i-1-c.first, j+1+c.second);
                    i1 = i - c.first - d, j1 = j + c.second + d;
                    addPalToBucketIfLonger(make_pair(i1, j1), buckets);
                }
                // Border reductions
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

    inline std::string getPal(const pair<int, int> &t) {
        if (t.first > t.second) return "";
        return this->t.substr(t.first, t.second-t.first+1);
    }

    void addPalToBucketIfLonger(const pair<int, int> &temp, vector<pair<int, int>> &buckets) {
        int mid = temp.first + temp.second;
        if (temp.first >= 1 && temp.second <= this->t.size() - 1) {
             if (dist(buckets[mid]) < dist(temp))
                buckets[mid] = temp;
    }
}

};

int main() {
    std::string t;
    std::cin >> t;
    for (auto &c: t) c = toupper(c);
    LCEStructure Q(t, standardPalindrom);

    vector<pair<int,int>> pals;

    std::cout << "allMaxPalHam(1)" << std::endl;
    pals = Q.allMaxPalHam(1);
    for (pair<int, int> p: pals)
        std::cout << p.first << ", " << p.second << " # " << Q.getPal(p) << std::endl;

    std::cout << "allMaxPalEdit(1)" << std::endl;
    pals = Q.allMaxPalEdit(1);
    for (pair<int, int> p: pals)
        std::cout << p.first << ", " << p.second << " # " << Q.getPal(p) << std::endl;
    return 0;
}


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

