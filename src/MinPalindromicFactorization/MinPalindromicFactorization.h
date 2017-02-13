#ifndef MIN_PALINDROMIC_FACTORIZATION_H
#define MIN_PALINDROMIC_FACTORIZATION_H

#include<functional>
#include<string>

class MinPalindromicFactorization {
    public:
        virtual int run(const std::string &s, std::function<char(char)> f) = 0;

    protected:
        std::string prepareString(std::string t) {
            for (auto &c: t) c = toupper(c);
            std::string s = "#";
            s.append(t);
            return s;
        }
};

#endif
