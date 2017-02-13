#include<functional>
#include<iostream>
#include<string>
#include "relations.hpp"
#include "MinPalindromicFactorization/MinPalindromicFactorizationBruteImpl.hpp"
#include "MinPalindromicFactorization/MinPalindromicFactorizationFiciImpl.hpp"

using std::string;
using std::function;

int main(int argc, char **argv) {
    int opt, brute, dna;
    brute = dna = 0;
    while ((opt = getopt(argc,argv,"bdh")) != EOF) {
        switch(opt) {
            case 'b': brute = 1; break;
            case 'd': dna = 1; break;
            case 'h': fprintf(stderr, "usuage is \n -b : for running brute \n -d : for DNA complement palindromes [default: standard palindromes]\n -h : help\n"); break;
        }
    }

    function<char(char)> f = dna ? relation::dnaComplementarity : relation::equality;

    MinPalindromicFactorization *solver;
    if (brute) {
        solver = new MinPalindromicFactorizationBruteImpl();
    } else {
        solver = new MinPalindromicFactorizationFiciImpl();
    }

    string t;
    std::cin >> t;
    std::cout << solver->run(t, f) << std::endl;
    return 0;
}

