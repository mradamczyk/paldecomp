#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<tuple>
#include<unistd.h>
#include "relations.hpp"
#include "DecompositionWithGaps/PalindromicDecompositionWithGapsBruteImpl.hpp"
#include "DecompositionWithGaps/PalindromicDecompositionWithGapsFastImpl.hpp"

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

int main(int argc, char **argv) {
    int opt, brute, dna, minLength = 1, maxGapsNum = 0, print = 0;
    brute = dna = 0;
    fprintf(stderr, "usage is \n"
            "-b : for running brute \n"
            "-d : for DNA complement palindromes [default: standard palindromes]\n"
            "-p : print decomposition (works only with -b)\n"
            "-L X: set minimum palindrome length to X [default: 1]\n"
            "-G X: set maximum allowed number of gaps to X [default: 0]\n");
    while ((opt = getopt(argc,argv,"bdpL:G:")) != EOF) {
        switch(opt) {
            case 'b': brute = 1; break;
            case 'd': dna = 1; break;
            case 'p': print = 1; break;
            case 'L': minLength = atoi(optarg); break;
            case 'G': maxGapsNum = atoi(optarg); break;
        }
    }
    function<char(char)> f = dna ? relation::dnaComplementarity : relation::equality;

    string t;
    while(std::cin >> t) {
        for (auto & c: t) c = toupper(c);

        PalindromicDecompositionWithGaps *solver;
        if (brute) {
            solver = new PalindromicDecompositionWithGapsBruteImpl(t, f, minLength, maxGapsNum);
        } else {
            solver = new PalindromicDecompositionWithGapsFastImpl(t, f, minLength, maxGapsNum);
        }

        int k = solver->run();
        std::cout << k << std::endl;

        if (print)
            solver->printDecomposition();
    }
    return 0;
}
