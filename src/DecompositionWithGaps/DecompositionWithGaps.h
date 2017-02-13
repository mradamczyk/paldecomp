#ifndef DECOMPOSITION_WITH_GAPS_H
#define DECOMPOSITION_WITH_GAPS_H

#include<string>
#include<vector>

class DecompositionWithGaps {
    public:
        DecompositionWithGaps(const std::string &t, int maxGapsNum) : maxGapsNum(maxGapsNum), n(t.size() - 1), t(t) {INFTY = 2 * n;}
        virtual std::vector<int> run() = 0;
        virtual void printDecomposition() = 0;

    protected:
        int maxGapsNum, n, INFTY;
        std::string t;
};

#endif
