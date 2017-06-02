/**
   A modified implementation of a Eertree structure
   presented by  M. Rubinchik and A. M. Shur in
    "EERTREE: an efficient data structure for processing palindromes in strings"
     Combinatorial Algorithms - 26th International Workshop, IWOCA 2015

   After modification, Eertree supports generalized palindroms (by passing f function)
   and a minimum length of a palindrom in series.
*/

#include <vector>
#include <functional>
#include <string>
using std::vector;
using std::function;
using std::string;

struct Node {
    vector<Node*> to;
    Node *link; // suffix link
    Node *slink; // serial link: used in palindromic factorization
    int len; // the length of the palindrom represented by Node
    int id;
    int diff;

    Node(const int sigma, int len, int id): link(NULL), slink(NULL), len(len), id(id), diff(0) {
        to.resize(sigma);
        for (int i = 0; i < sigma; ++i) {
            to[i] = NULL;
        }
    }
};

class Eertree {
    private:
        const int sigma;
        const int minLength;
        const function<char(char)> f;

        vector<Node*> nodes;
        Node *rootOdd, *rootEven; // two initial root nodes
        string s;  // string

    public:
        Node *last;

        Eertree(const int sigma, const int minLength, const function<char(char)> f): sigma(sigma), minLength(minLength), f(f) {
            rootOdd = new Node(sigma, -1, 0), rootEven = new Node(sigma, 0, 1);
            rootOdd->link = rootEven->link = rootOdd;
            s.push_back('$');
            last = rootEven;
        }

        Node* get_link(Node *u) {
            int n = s.size() - 1;
            while (s[n - u->len - 1] != f(s[n]) && u != u->link) u = u->link;
            return u;
        }

        void add_letter(char c) {
            s.push_back(c);
            last = get_link(last);
            if (last->len != -1 || c == f(c)) { // create single letter palindrom only if c==f(c)
                c -= 'A';
                if (last->to[c] == NULL) {
                    Node *p = new Node(sigma, last->len + 2, nodes.size() + 2);
                    p->link = get_link(last->link)->to[c];
                    if (p->link == NULL) {
                        p->link = rootEven;
                    }

                    p->diff = p->len - p->link->len;
                    if (p->diff == p->link->diff && p->link->slink->len >= minLength) {
                        // shorcut series palindromes path only up to the shortest palindrom that is no shorter then minLength
                        p->slink = p->link->slink;
                    } else {
                        p->slink = p->link;
                    }

                    last->to[c] = p;
                    nodes.push_back(p);
                }
                last = last->to[c];
            } else {
                last = rootEven;
            }
        }

        ~Eertree() {
            for(Node* x: nodes) {
                delete x;
            }
            nodes.clear();
            delete rootOdd;
            delete rootEven;
        }
};
