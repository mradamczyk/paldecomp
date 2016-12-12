// Implementations of https://arxiv.org/pdf/1403.2431.pdf

#include<functional>
#include<iostream>
#include<queue>
#include<string>
#include<tuple>
#include<unistd.h>
#include<vector>

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
using std::make_tuple;
using std::get;
using std::min;
using std::tie;
using std::string;
using std::vector;
using std::function;

int minPalFactFICI(const string &t, function<char(char)> f) {
	std::cerr << "FICI"  << std::endl;
	int n = t.size() - 1;

	vector<int> PL(n + 1, 2*n);
	vector<int> GPL(n + 1, 2*n);
	vector<triple> G, G1;
	std::queue<triple> G2;
	int i, d, k;
	int i1, d1, k1;

	PL[0] = 0;
	G.clear();

	for (int j = 1; j <= n; ++j) {
		G1.clear();
		for (const triple &g : G) {
			tie(i, d, k) = g;
			if (i > 1 && t[i-1] == f(t[j]))
				G1.push_back(make_tuple(i-1,d,k));
		}

		int r = -j; // i-r big enough to act as inf
		for (const triple &g : G1) {
			tie(i, d, k) = g;
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
			tie(i1, d1, k1) = G2.front();
			G2.pop();
			while (!G2.empty()) {
				tie(i,d,k) = G2.front();
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

		for (const triple &g : G) {
			tie(i, d, k) = g;

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

int minPalFactN2(const string &t, function<char(char)> f) {
	std::cerr << "BRUTE"  << std::endl;

	int n = t.size() - 1;
	vector<int> PL(n+1, 2*n);
	vector<int> P[2];

	PL[0] = 0;
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

		for (int i: P[j&1])
			PL[j] = min(PL[j], PL[i-1] + 1);
	}
	return PL[n] > n ? -1 : PL[n];
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


	function<char(char)> f = dna ? dnaComplementaryPalindrom : standardPalindrom;
	function<int(string, function<char(char)>)> minPalFact = brute ? minPalFactN2 : minPalFactFICI;

	string t;
	while (std::cin >> t) {
		for (auto &c: t) c = toupper(c);

		string s = "#";
		s.append(t);
	
		std::cout << minPalFact(s, f) << std::endl;
	}
	return 0;
}
