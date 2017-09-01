#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<unistd.h>
#include "relations.hpp"
#include "MaximalGPalindromes/MaximalGPalindromes.hpp"
#include "DecompositionWithGaps/GenericDecompositionWithGapsFastImpl.hpp"
#include "DecompositionWithGaps/GenericDecompositionWithGapsBruteImpl.hpp"

using std::pair;
using std::make_pair;
using std::vector;
using std::string;
using std::function;

int main(int argc, char **argv) {
    int opt, dna = 0, errorsAllowed = 0, minLength = 1, maxGapsNum = 0, print = 0, brute = 0;
    string errorsMetric = "edit"; // or "ham"
    while ((opt = getopt(argc,argv,"bhHdpL:G:E:")) != EOF) {
        switch(opt) {
            case 'b': brute = 1; break;
            case 'd': dna = 1; break;
            case 'p': print = 1; break;
            case 'H': errorsMetric = "ham"; break;
            case 'L': minLength = atoi(optarg); break;
            case 'G': maxGapsNum = atoi(optarg); break;
            case 'E': errorsAllowed = atoi(optarg); break;
            case 'h':
            default:
                fprintf(stderr, "usage is \n"
                        "-b : for running brute\n"
                        "-h : for help\n"
                        "-d : for DNA complement palindromes [default: standard palindromes]\n"
                        "-p : print decomposition\n"
                        "-E x: set number of allowed errors to X [default: 0]\n"
                        "-H : for running with Hamming distance [default: edit distance]\n"
                        "-G X: set number of allowed gaps to X [default: 0]\n"
                        "-L X: set minimum palindrome length to X [default: 1]\n");
                exit(0);
        }
    }

    string t;
    while(std::cin >> t) {
        for (auto & c: t) c = toupper(c);

        function<char(char)> f = dna ? relation::dnaComplementarity : relation::equality;

        MaximalGPalindromes palindromesFinder(t, f);

        vector< pair<int,int> > palindromes, allGPalindromes;
        if (errorsMetric == "edit") {
            palindromes = palindromesFinder.allMaximalEditDistGPalindromes(errorsAllowed);
        } else {
            palindromes = palindromesFinder.allMaximalHammingDistGPalindromes(errorsAllowed);
        }

        for (const pair<int, int> &p: palindromes) {
            int begin = p.first, end = p.second;
            while (end - begin + 1 >= minLength) {
                allGPalindromes.push_back(make_pair(begin, end));
                begin++;
                end--;
            }
            // TODO: this is probably not all yet, let's see if we can find an missing
        }

        GenericDecompositionWithGaps *solver = nullptr;
        if (brute) {
            solver = new GenericDecompositionWithGapsBruteImpl(t, allGPalindromes, maxGapsNum);
        } else {
            solver = new GenericDecompositionWithGapsFastImpl(t, allGPalindromes, maxGapsNum);
        }

        int k = solver->run();
        std::cout << k << std::endl;

        if (print) {
            solver->printDecomposition();
        }
    }

    return 0;
}
