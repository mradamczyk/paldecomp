#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<unistd.h>

inline char standardPalindrome(const char &t) { return t; }
inline char dnaComplementaryPalindrome(const char &t) {
    switch (t) {
        case 'A': return 'T';
        case 'C': return 'G';
        case 'G': return 'C';
        case 'T': return 'A';
    }
    return t;
}

using std::pair;
using std::make_pair;
using std::vector;
using std::string;
using std::function;

int print = 0;

inline int dist(const pair<int, int> &p) {
    return p.second - p.first + 1;
}

class LCEStructure {
    private:
        string y;
        string x;
        function<char(char)> f;

    public:
        LCEStructure(string t, function<char(char)> f) {
            this->f = f;

            // x = #t
            // y = x + f(x^R)
            this->y = "#";
            this->y.append(t);
            this->x = string(this->y);
            for (auto &c: t) c = f(c);
            std::reverse(std::begin(t), std::end(t));
            this->y.append(t);
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

        // Finding maximal g-palindrome centered between i and j under Hamming dist
        // For an even-length g-palindrome we set j=i+1
        // For an odd-length g-palindrome we set j=i -- run only when f(t[i])=t[i]
        int maxPalHam(int i, int j, int g) {
            int a = i, b = j, d;
            int n = this->x.size() - 1;
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
            int n = this->x.size() - 1;
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
            int n = this->x.size() - 1;
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

        inline string getPal(const pair<int, int> &t) {
            if (t.first > t.second) return "";
            return this->x.substr(t.first, t.second-t.first+1);
        }

        void addPalToBucketIfLonger(const pair<int, int> &temp, vector<pair<int, int>> &buckets) {
            int mid = temp.first + temp.second;
            int n = this->x.size() - 1;
            if (temp.first >= 1 && temp.second <= n && mid <= 2*n && dist(buckets[mid]) < dist(temp)) {
                buckets[mid] = temp;
            }
        }

        vector<int> maxPalFactWithErrosGaps(const string &errorsMetric, int errorsAllowed, int minLength, int maxGaps) {
            vector<pair<int, int>> Pal;
            if (errorsMetric == "edit") { // Edit distance
                Pal = allMaxPalEdit(errorsAllowed);
            } else { // Hamming dist
                Pal = allMaxPalHam(errorsAllowed);
            }

            int n = this->x.size() - 1;
            int INFTY = 2 * n;

            vector<vector<int>> MG(n+1), MG1(n+1);
            vector<vector<pair<int, int>>> D(n+1), D1(n+1);
            for (int j = 0; j <= n; ++j) {
                MG[j].resize(maxGaps+1);
                MG1[j].resize(maxGaps+1);
                D[j].resize(maxGaps+1);
                D1[j].resize(maxGaps+1);
                for (int q = 0; q <= maxGaps; ++q) {
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

            vector<vector<int> > P(n+1);
            for (auto &p: Pal) P[p.second].push_back(p.first);

            for (int j = 1; j <= n; ++j) {
                for (int q = 0; q <= maxGaps; ++q) {
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
                    for (int i: P[j])
                        if (j - i + 1 >= minLength)
                            if (MG[i-1][q] < MG[j][q]) {
                                MG[j][q] = MG[i-1][q];
                                D[j][q] = make_pair(i-1, q);
                            }
                }
            }

            if (print) {
                int prevj = n, prevq = maxGaps;
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
                      x.substr(rev[i]+1, rev[i+1]-rev[i]) <<
                      (!qs[i] ? std::string("") : std::string("]")) <<
                      " ";
                }
                std::cout << std::endl;
            }
            return MG[n];
        }
};

int main(int argc, char **argv) {
    int opt, dna = 0, errorsAllowed = 0, minLength = 1, maxGaps = 0;
    string errorsMetric = "edit"; // or "ham"
    fprintf(stderr, "usage is \n -h : for running with Hamming distance [default: edit distance]\n -d : for DNA complement palindromes [default: standard palindromes]\n -p : to print decomposition\n -L X: set minimum palindrome length to X [default: 1]\n -G X: set maximum allowed gaps to X [default: 0]\n -E x: set number of allowed errors to X [default: 0]\n");
    while ((opt = getopt(argc,argv,"hdpL:G:E:")) != EOF) {
        switch(opt) {
            case 'h': errorsMetric = "ham"; break;
            case 'd': dna = 1; break;
            case 'p': print = 1; break;
            case 'L': minLength = atoi(optarg); break;
            case 'G': maxGaps = atoi(optarg); break;
            case 'E': errorsAllowed = atoi(optarg); break;
        }
    }

    function<char(char)> f = dna ? dnaComplementaryPalindrome : standardPalindrome;

    string t;
    while (std::cin >> t) {
        for (auto &c: t) c = toupper(c);
        LCEStructure Q(t, f);
        vector<pair<int, int> > pals = errorsMetric == "edit" ? Q.allMaxPalEdit(errorsAllowed) : Q.allMaxPalHam(errorsAllowed);
        std::cerr << "maximal " << errorsAllowed << "-palindromes:" << std::endl;
        for (pair<int, int> p: pals)
            std::cout << p.first << ", " << p.second << " # " << Q.getPal(p) << std::endl;
        std::cerr << "maximal " << errorsAllowed << "-palindromes decomposition (" << errorsMetric << " distance, min palindrome length: " << minLength << ", max gaps: " << maxGaps << "):" << std::endl;
        for (int k: Q.maxPalFactWithErrosGaps(errorsMetric, errorsAllowed, minLength, maxGaps))
            std::cout << (k > int(t.size()) ? -1 : k) << " ";
        std::cout << std::endl;
    }

    return 0;
}
