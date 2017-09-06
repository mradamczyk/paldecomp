#ifndef PALINDROMIC_DECOMPOSITION_WITH_GAPS_H
#define PALINDROMIC_DECOMPOSITION_WITH_GAPS_H

#include<functional>
#include<string>
#include<vector>
#include "../DecompositionWithGaps.h"

class PalindromicDecompositionWithGaps : public DecompositionWithGaps {
    public:
        PalindromicDecompositionWithGaps(const std::string &t, std::function<char(char)> f, int minLength, int maxGapsNum) :
            DecompositionWithGaps(t, maxGapsNum), f(f), minLength(minLength) {}

        virtual int run() = 0;
        virtual void printDecomposition() = 0;
    protected:
        std::function<char(char)> f;
        int minLength;

        std::string prepareString(std::string t) {
            for (auto &c: t) c = toupper(c);
            std::string s = "#";
            s.append(t);
            return s;
        }
};

#endif
