/* Implementation of a solution to
   the Generalized Palindromic Decomposition with Gaps problem
   in O(n log n * g) time and O(n * g) space
 */

// JR: For the code of brute force, add a similar comment.

#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<tuple>
#include<unistd.h>

inline char equality(const char &t) { return t; }
inline char dnaComplementarityRelation(const char &t) {
    switch (t) {
        case 'A': return 'T';
        case 'C': return 'G';
        case 'G': return 'C';
        case 'T': return 'A';
    }
    return t;
}

using triple = std::tuple<int, int, int>;
using std::get;
using std::min;
using std::max;
using std::tie;
using std::make_tuple;
using std::pair;
using std::make_pair;
using std::string;
using std::vector;
using std::function;

int print = 0; // JR: this should be a parameter. To be added to palDecompWithGaps

// JR: Better return a single number.
vector<int> palDecompWithGaps(const string &t, function<char(char)> f, int minLength, int maxGapsNum) {
    // JR if (print) ... std::cerr << "FICI(" << minLength <<", " << maxGapsNum << ")" << std::endl;
    int n = t.size() - 1;
    int INFTY = 2*n;

    vector<triple> G, G1;
    std::queue<triple> G2;
    int i, d, k;
    int i1, d1, k1;

    // initialisation
    vector<vector<int>> MG(n+1), MG1(n+1), GPL2(maxGapsNum+1);
    for (int j = 0; j <= maxGapsNum; ++j)
        GPL2[j].resize(n+1); // JR: Can this array be hidden inside the computation of MG & MG1?
    for (int j = 0; j <= n; ++j) {
        MG[j].resize(maxGapsNum+1);
        MG1[j].resize(maxGapsNum+1);
        for (int q = 0; q <= maxGapsNum; ++q) {
            MG[j][q] = (j==0) ? 0 : INFTY;
            MG1[j][q] = INFTY;
        }
    }
    G.clear();

    // JR: Can this be subdivided into functions? E.g., first we compute G, then G-trimmed, then MG & MG1?
    for (int j = 1; j <= n; ++j) {
        // JR: Some comment on what is being computed here?
        // JR: What is G1, G2?
        G1.clear();
        for (const triple &g : G) {
            std::tie(i, d, k) = g;
            if (i > 1 && t[i-1] == f(t[j]))
                G1.push_back(make_tuple(i-1, d, k));
        }

        int r = -j; // i-r big enough to act as inf
        for (const triple &g : G1) {
            std::tie(i, d, k) = g;
            if (i - r != d) {
                G2.push(make_tuple(i, i-r, 1));
                if (k > 1)
                    G2.push(make_tuple(i+d, d, k-1));
            } else {
                G2.push(make_tuple(i, d, k));
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
            std::tie(i1, d1, k1) = G2.front();
            G2.pop();
            while (!G2.empty()) {
                std::tie(i,d,k) = G2.front();
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

        // filter G on length constraint
        G1.clear();
        int maxPos = max(j - minLength + 1, 0);
        for (const triple &g : G) {
            std::tie(i, d, k) = g;

            int nk = (maxPos - i) / d + (i <= maxPos);
            if (k > 1 && nk > 0)
                G1.push_back(make_tuple(i, d, nk));
            if (k == 1 && i <= maxPos)
                G1.push_back(make_tuple(i, d, k));
        }

        // compute MG[j] and MG1[j]
        for (int q = 0; q <= maxGapsNum; ++q) {
            if (q > 0)
                MG1[j][q] = min(MG1[j-1][q], MG[j-1][q-1]) + 1;
            MG[j][q] = MG1[j][q];
            for (const triple &g : G1) {
                std::tie(i, d, k) = g;

                r = i + (k-1) * d;
                int mn = MG[r-1][q];
                if (k > 1 && d <= i)
                    mn = min(mn, GPL2[q][i - d]);
                if (d <= i)
                    GPL2[q][i-d] = mn;
                MG[j][q] = min(MG[j][q], mn);
            }
        }
    }
    return MG[n]; // JR: In the end, if INFTY, return -1.
}

// JR: To be moved to another file for production code.
vector<int> palDecompWithGaps_N2(const string &t, function<char(char)> f, int minLength, int maxGapsNum) {
    std::cerr << "BRUTE(" << minLength <<", " << maxGapsNum << ")" << std::endl;
    int n = t.size() - 1;
    int INFTY = 2 * n;

    vector<vector<int>> MG(n+1), MG1(n+1);
    vector<vector<std::pair<int, int>>> D(n+1), D1(n+1);
    for (int j = 0; j <= n; ++j) {
        MG[j].resize(maxGapsNum+1);
        MG1[j].resize(maxGapsNum+1);
        D[j].resize(maxGapsNum+1);
        D1[j].resize(maxGapsNum+1);
        for (int q = 0; q <= maxGapsNum; ++q) {
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

    vector<int> P[2];
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

        for (int q = 0; q <= maxGapsNum; ++q) {
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
            for (int i: P[j&1])
                if (j - i + 1 >= minLength)
                    if (MG[i-1][q] < MG[j][q]) {
                        MG[j][q] = MG[i-1][q];
                        D[j][q] = make_pair(i-1, q);
                    }
        }
    }
    if (print && MG[n][maxGapsNum] > n)
        std::cout << "NOT POSSIBLE" << std::endl;
    else if (print) {
        int prevj = n, prevq = maxGapsNum;
        int j, q;
        j = D[prevj][prevq].first, q = D[prevj][prevq].second;
        vector<int> rev;
        while (prevj != j || prevq != q || (j != 0 && q != 0)) {
            rev.push_back(prevj);
            prevj = j, prevq = q;
            j = D[prevj][prevq].first, q = D[prevj][prevq].second;
        }
        rev.push_back(0);
        std::reverse(std::begin(rev), std::end(rev));

        for (uint i = 0; i < rev.size() - 1; ++i) {
            std::cout << t.substr(rev[i]+1, rev[i+1]-rev[i]) << " ";
        }
        std::cout << std::endl;
    }

    return MG[n];
}

// JR: This could be in a yet separate file, which imports the two previous ones.
// JR: Better to run on a single string, not in a loop.
int main(int argc, char **argv) {
    int opt, brute, dna, minLength = 1, maxGapsNum = 0;
    brute = dna = 0;
    fprintf(stderr, "usage is \n -b : for running brute \n -d : for DNA complement palindromes [default: standard palindromes]\n -p : print decomposition (works only with -b)\n -L X: set minimum palindrome length to X [default: 1]\n -G X: set maximum allowed number of gaps to X [default: 0]\n");
    while ((opt = getopt(argc,argv,"bdpL:G:")) != EOF) {
        switch(opt) {
            case 'b': brute = 1; break;
            case 'd': dna = 1; break;
            case 'p': print = 1; break;
            case 'L': minLength = atoi(optarg); break;
            case 'G': maxGapsNum = atoi(optarg); break;
        }
    }

    function<char(char)> f = dna ? dnaComplementarityRelation : equality;
    function<vector<int>(string, function<char(char)>, int, int)> minPalFact = brute ? palDecompWithGaps_N2 : palDecompWithGaps;

    string t;
    while (std::cin >> t) {
        for (auto &c: t) c = toupper(c);

        string s = "#";
        s.append(t);

        for (int k: minPalFact(s, f, minLength, maxGapsNum))
            std::cout << (k > int(s.size()) - 1 ? -1 : k)  << " ";
        std::cout << std::endl;
    }
    return 0;
}
