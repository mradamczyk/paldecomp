// Implementations of https://arxiv.org/pdf/1403.2431.pdf

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

using triple = std::tuple<int, int, int>;
using std::get;
using std::min;
using std::max;
using std::tie;
using std::make_tuple;

int minPalFactFICI(const std::string &t, std::function<char(char)> f, int minLength, int maxGaps) {
    int n = t.size() - 1;

    std::vector<int> PL(n + 1, 2*n);
    std::vector<int> GPL(n + 1, 2*n);
    std::vector<triple> G, G1;
    std::queue<triple> G2;
    int i, d, k;
    int i1, d1, k1;

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

            int nk = (maxPos - i + 1) / d;
            if (k > 1 && nk > 0)
                G1.push_back(make_tuple(i, d, nk));
            if (k == 1 && i <= maxPos)
                G1.push_back(make_tuple(i, d, k));
        }
        //std::cerr << "G1: "; for (const triple &g: G1) { std::tie(i, d, k) = g; std::cerr << '(' << i << ',' << d << ',' << k << "), "; } std::cerr << std::endl;

        for (const triple &g : G1) {
            std::tie(i, d, k) = g;

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

int minPalFactN2(const std::string &t, std::function<char(char)> f, int minLength, int maxGaps) {
    int n = t.size() - 1;
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

        for (int i: P[j&1])
            if (j - i + 1>= minLength)
                PL[j] = std::min(PL[j], PL[i-1] + 1);
    }
    return PL[n] > n ? -1 : PL[n];
}


int main() {
    std::string t;
    std::cin >> t;
    for (auto &c: t) c = toupper(c);

    std::string s = "#";
    s.append(t);

    for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9}) {
        std::cout << i << ": " << minPalFactN2(s, standardPalindrom, i, 10) << " " << minPalFactFICI(s, standardPalindrom, i, 10) << std::endl;
    }
    // std::cout << minPalFactFICI(s, standardPalindrom) << std::endl;
    // std::cout << minPalFactN2(s, dnaComplementaryPalindrom) << std::endl;
    // std::cout << minPalFactFICI(s, dnaComplementaryPalindrom) << std::endl;

    return 0;
}
