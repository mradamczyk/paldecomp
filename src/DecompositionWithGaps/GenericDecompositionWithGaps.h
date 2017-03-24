/* minimal (maxGapsNum, F)-factorization of t */
#ifndef GENERIC_DECOMPOSITION_WITH_GAPS_H
#define GENERIC_DECOMPOSITION_WITH_GAPS_H

#include<vector>
#include<string>
#include<unistd.h>
#include "DecompositionWithGaps.h"

class GenericDecompositionWithGaps : public DecompositionWithGaps {
    public:
        GenericDecompositionWithGaps(const std::string& t, const std::vector<std::pair<int, int>> &F, int maxGapsNum) :
            DecompositionWithGaps(t, maxGapsNum), F(F) {}

        virtual int run() = 0;
        virtual void printDecomposition() = 0;
    protected:
        std::vector<std::pair<int, int>> F;
};

#endif
