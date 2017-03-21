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

int main() {
    string t = "#aaaabbbbccccddddeeee";
    vector<pair<int,int>> F;
    F.push_back(make_pair(2,7));
    F.push_back(make_pair(9,12));
    F.push_back(make_pair(6,16));
    F.push_back(make_pair(14,17));
    F.push_back(make_pair(18,19));

    GenericDecompositionWithGaps *solver = nullptr;

    std::cout << "gaps=4" << std::endl;
    solver = new GenericDecompositionWithGapsBruteImpl(t, F, 4);
    std::cout << "brute:" << solver->run() << std::endl;
    solver->printDecomposition();
    solver = new GenericDecompositionWithGapsFastImpl(t, F, 4);
    std::cout << "fast:" << solver->run() << std::endl;
    solver->printDecomposition();

    std::cout << "\ngaps=3" << std::endl;
    solver = new GenericDecompositionWithGapsBruteImpl(t, F, 3);
    std::cout << "brute:" << solver->run() << std::endl;
    solver->printDecomposition();
    solver = new GenericDecompositionWithGapsFastImpl(t, F, 3);
    std::cout << "fast:" << solver->run() << std::endl;
    solver->printDecomposition();

    std::cout << "\ngaps=2" << std::endl;
    solver = new GenericDecompositionWithGapsBruteImpl(t, F, 2);
    std::cout << "brute:" << solver->run() << std::endl;
    solver->printDecomposition();
    solver = new GenericDecompositionWithGapsFastImpl(t, F, 2);
    std::cout << "fast:" << solver->run() << std::endl;
    solver->printDecomposition();

    std::cout << "\ngaps=1" << std::endl;
    solver = new GenericDecompositionWithGapsBruteImpl(t, F, 1);
    std::cout << "brute:" << solver->run() << std::endl;
    solver->printDecomposition();
    solver = new GenericDecompositionWithGapsFastImpl(t, F, 1);
    std::cout << "fast:" << solver->run() << std::endl;
    solver->printDecomposition();

    std::cout << "\ngaps=0" << std::endl;
    solver = new GenericDecompositionWithGapsBruteImpl(t, F, 0);
    std::cout << "brute:" << solver->run() << std::endl;
    solver->printDecomposition();
    solver = new GenericDecompositionWithGapsFastImpl(t, F, 0);
    std::cout << "fast:" << solver->run() << std::endl;
    solver->printDecomposition();

    return 0;
}
