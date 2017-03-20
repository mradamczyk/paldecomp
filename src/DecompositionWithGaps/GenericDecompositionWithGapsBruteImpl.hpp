/*
  Slow implementation of finding minimal (maxGapsNum, F)-factorization of t.
  in O(n * 2^|F|) time complexity.
 */

#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<stdexcept>
#include<unistd.h>
#include "GenericDecompositionWithGaps.h"

using std::pair;
using std::make_pair;
using std::vector;
using std::string;

class GenericDecompositionWithGapsBruteImpl : public GenericDecompositionWithGaps {
    private:
            // Vectors D, D1 are used to recover decomposition.
            vector<bool> bestSolution;
            int bestSolutionScore = n;

    public:
        GenericDecompositionWithGapsBruteImpl(string& t, const vector<pair<int, int>> &F, int maxGapsNum)
            : GenericDecompositionWithGaps(t, F, maxGapsNum) {}

        int run() {
            this->init();

            vector<bool> emptySolution;
            findBestSolution(0,0,emptySolution,0);
            return bestSolutionScore > n ? -1 : bestSolutionScore;
        }

        void printDecomposition() {
            int x, y, end = 0;
            for (int i = 0; i < bestSolution.size(); ++i)
                if ( bestSolution[i] == 1) {
                    x = F[i].first, y = F[i].second;
                    if (end + 1 != x) {
                        std::cout << "[" << t.substr(end + 1, x - end - 1) << "]" << " ";
                    }
                    std::cout << t.substr(x, y - x + 1) << " ";
                    end = y;
                }
            if (bestSolutionScore <= n && end != n) {
                std::cout << "[" << t.substr(end + 1, n - end) << "]" << std::endl;
            }
        }

    private:
        void init() {
            bestSolutionScore = n+1;
            std::sort(F.begin(), F.end());
        }

        void findBestSolution(int lastEnd, int usedGaps, vector<bool> currentSolution, int currentScore) {
            int k = currentSolution.size();
            if (k == F.size()) {
                if (lastEnd != n && usedGaps >= maxGapsNum) {
                    return; // not allowed
                }
                currentScore += n - lastEnd;

                if (currentScore < bestSolutionScore) {
                    bestSolutionScore = currentScore;
                    bestSolution = currentSolution;
                }
                return;
            }

            pair<int,int> w = F[k];
            if (lastEnd + 1 > w.first) {
                currentSolution.push_back(0);
                findBestSolution(lastEnd, usedGaps, currentSolution, currentScore);
                return;
            }

            if (lastEnd + 1 == w.first) {
                currentSolution.push_back(0);
                findBestSolution(lastEnd, usedGaps, currentSolution, currentScore);

                currentSolution[k] = 1;
                findBestSolution(w.second, usedGaps, currentSolution, currentScore);
                return;
            }

            if (usedGaps >= maxGapsNum) {
                return; // all gaps used
            }

            currentSolution.push_back(0);
            findBestSolution(lastEnd, usedGaps, currentSolution, currentScore);

            currentSolution[k] = 1;
            findBestSolution(w.second, usedGaps + 1, currentSolution, currentScore + w.first - (lastEnd + 1));
        }
};

