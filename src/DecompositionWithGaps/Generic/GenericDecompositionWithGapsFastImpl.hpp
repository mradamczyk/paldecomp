/*
  Fast implementation of finding minimal (maxGapsNum, F)-factorization of t
  in O((n+|F|)*g) time and O(n*g+|F|) space complexity.
 */

#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<unistd.h>
#include "GenericDecompositionWithGaps.h"

using std::pair;
using std::make_pair;
using std::vector;
using std::string;

class GenericDecompositionWithGapsFastImpl : public GenericDecompositionWithGaps {
    private:
            // initialisation
            vector<vector<int>> MG, MG1;

            // Vectors D, D1 are used to recover decomposition.
            vector<vector<pair<int, int>>> D, D1;

    public:
        GenericDecompositionWithGapsFastImpl(string& t, const vector<pair<int, int>> &F, int maxGapsNum)
            : GenericDecompositionWithGaps(t, F, maxGapsNum) {}

        int run() {
            this->init();

            // sort F by end positions using bucket sort in vector P
            vector<vector<int> > P;
            P.resize(n+1);
            for (auto &p: F) P[p.second].push_back(p.first);

            for (int j = 1; j <= n; ++j) {
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
                    for (int i: P[j])
                            if (MG[i-1][q] < MG[j][q]) {
                                MG[j][q] = MG[i-1][q];
                                D[j][q] = make_pair(i-1, q);
                            }
                }
            }
            return MG[n][maxGapsNum] > n ? -1 : MG[n][maxGapsNum];
        }

        void printDecomposition() {
            int prevj = n, prevq = maxGapsNum;
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
                    t.substr(rev[i]+1, rev[i+1]-rev[i]) <<
                    (!qs[i] ? std::string("") : std::string("]")) <<
                    " ";
            }
            std::cout << std::endl;
        }
    private:
        void init() {
            MG.resize(n+1);
            MG1.resize(n+1);
            D.resize(n+1);
            D1.resize(n+1);

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
        }

};

