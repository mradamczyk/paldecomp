#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<tuple>
#include<unistd.h>

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

using triple = std::tuple<int, int, int>;
using std::get;
using std::min;
using std::max;
using std::tie;
using std::make_tuple;

std::vector<int> minPalFactFICI(const std::string &t, std::function<char(char)> f, int minLength, int maxGaps) {
    std::cerr << "FICI(" << minLength <<", " << maxGaps << ")" << std::endl;
    int n = t.size() - 1;

    std::vector<int> PL(n + 1, 2*n);
    std::vector<int> GPL(n + 1, 2*n);
    std::vector<triple> G, G1;
    std::queue<triple> G2;
    int i, d, k;
    int i1, d1, k1;

    std::vector<std::vector<int>> MG(n+1), MG1(n+1), GPL2(maxGaps+1);
    for (int j = 0; j <= maxGaps; ++j)
        GPL2[j].resize(n+1);

    for (int j = 0; j <= n; ++j) {
        MG[j].resize(maxGaps+1);
        MG1[j].resize(maxGaps+1);
        for (int q = 0; q <= maxGaps; ++q)
            MG[j][q] = MG1[j][q] = (j == 0 ? 0 : 2*n);
    }

    PL[0] = 0;
    G.clear();

    for (int j = 1; j <= n; ++j) {
        //std::cerr << "Loop " << j << ", " << t[j] << std::endl;
        G1.clear();
        for (const triple &g : G) {
            std::tie(i, d, k) = g;
            if (i > 1 && t[i-1] == f(t[j]))
                G1.push_back(make_tuple(i-1,d,k));
        }

        int r = -j; // i-r big enough to act as inf
        for (const triple &g : G1) {
            std::tie(i, d, k) = g;
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

        //std::cerr << "G : "; for (const triple &g: G) { std::tie(i, d, k) = g; std::cerr << '(' << i << ',' << d << ',' << k << "), "; } std::cerr << std::endl;
        // filter G on length constraint
        G1.clear();
        int maxPos = max(j - minLength + 1, 0);
        for (const triple &g : G) {
            std::tie(i, d, k) = g;

            int nk = (maxPos - i) / d + (i <= maxPos);
            //std::cerr << "maxPos: " << maxPos << ", nk: " << nk << std::endl;
            if (k > 1 && nk > 0)
                G1.push_back(make_tuple(i, d, nk));
            if (k == 1 && i <= maxPos)
                G1.push_back(make_tuple(i, d, k));
        }
        //std::cerr << "G1: "; for (const triple &g: G1) { std::tie(i, d, k) = g; std::cerr << '(' << i << ',' << d << ',' << k << "), "; } std::cerr << std::endl;

        for (const triple &g : G1) {
            std::tie(i, d, k) = g;

            r = i + (k-1) * d;
            int mn = PL[r-1] + 1;
            if (k > 1 && d <= i)
                mn = min(mn, GPL[i - d]);
            if (d <= i)
                GPL[i - d] = mn;
            PL[j] = min(PL[j], mn);
        }

        MG[j][0] = PL[j] > n ? 2*n : 0; // infty or 0
        for (int q = 1; q <= maxGaps; ++q) {
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
    return MG[n];
}

std::vector<int> minPalFactN2(const std::string &t, std::function<char(char)> f, int minLength, int maxGaps) {
    std::cerr << "BRUTE(" << minLength <<", " << maxGaps << ")" << std::endl;
    int n = t.size() - 1;

    std::vector<std::vector<int>> MG(n+1), MG1(n+1);
    for (int j = 0; j <= n; ++j) {
        MG[j].resize(maxGaps+1);
        MG1[j].resize(maxGaps+1);
        for (int q = 0; q <= maxGaps; ++q)
            MG[j][q] = MG1[j][q] = (j == 0 ? 0 : 2*n);
    }

    std::vector<int> PL(n+1, 2*n);
    std::vector<int> P[2];

    PL[0] = 0;
    P[0].clear();
    for (int j = 1; j <= n; ++j) {
        P[j&1].clear();
        for (int i: P[1-j&1])
            if (i > 1 && t[i-1] == f(t[j]))
                P[j&1].push_back(i-1);

        if (j > 1 && t[j-1] == f(t[j]))
            P[j&1].push_back(j-1);

        if (t[j] == f(t[j]))
            P[j&1].push_back(j);

        for (int i: P[j&1]) {
            if (j - i + 1>= minLength)
                PL[j] = std::min(PL[j], PL[i-1] + 1);
        }

        MG[j][0] = PL[j] > n ? 2*n : 0; // infty or 0
        for (int q = 1; q <= maxGaps; ++q) {
            MG1[j][q] = min(MG1[j-1][q], MG[j-1][q-1]) + 1;
            MG[j][q] = MG1[j][q];
            for (int i: P[j&1])
                if (j - i + 1 >= minLength)
                    MG[j][q] = min(MG[j][q], MG[i-1][q]);
        }
    }
    return MG[n];
}

int main(int argc, char **argv) {
    int opt, brute, dna;
    brute = dna = 0;
    while ((opt = getopt(argc,argv,"bd")) != EOF) {
        switch(opt) {
            case 'b': brute = 1; break;
            case 'd': dna = 1; break;
            case '?': fprintf(stderr, "usuage is \n -b : for running brute \n -d : for DNA complement palindromes [default: standard palindromes]"); break;
        }
    }

    int MAX_GAPS = 4;

    std::function<char(char)> f = dna ? dnaComplementaryPalindrom : standardPalindrom;
    std::function<std::vector<int>(std::string, std::function<char(char)>, int, int)> minPalFact = brute ? minPalFactN2 : minPalFactFICI;

    std::string t;
    while (std::cin >> t) {
        for (auto &c: t) c = toupper(c);

        std::string s = "#";
        s.append(t);

        for (int i = 1; i <= 5; ++i) {
            for (int k: minPalFact(s, f, i, MAX_GAPS))
                std::cout << k << " ";
            std::cout << std::endl;
        }
    }
    return 0;
}
