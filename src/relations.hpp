#ifndef PALINDROMES_RELATIONS
#define PALINDROMES_RELATIONS

namespace relation {

inline char equality(const char &t) { return t; }

inline char dnaComplementarity(const char &t) {
    switch (t) {
        case 'A': return 'T';
        case 'C': return 'G';
        case 'G': return 'C';
        case 'T': return 'A';
    }
    return t;
}

}

#endif
