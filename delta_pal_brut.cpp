#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<cstdlib>
using namespace std;

inline char dnaComplementaryPalindrome(const char &t) {
    switch (t) {
        case 'A': return 'T';
        case 'C': return 'G';
        case 'G': return 'C';
        case 'T': return 'A';
    }
    return t;
}

vector<int> positions;

// Writes a decomposition of s as a complemented delta-palindrome, if any.
void check(string s, int i, int j, int delta)
{
  int n = (int)s.size();
  if (delta < 0)
    return;
  if (i > j)
  {
    puts("TAK");
    for (int l = 0; l < (int)s.size(); ++l)
    {
      for (int k = 0; k < (int)positions.size(); ++k)
        if (positions[k] == l)
          printf("\\textcolor{red}{");
        else if (positions[k] == l + n)
          printf("\\textcolor{blue}{");
      putchar(s[l]);
      for (int k = 0; k < (int)positions.size(); ++k)
        if (positions[k] == l)
          printf("}");
        else if (positions[k] == l + n)
          printf("}");
    }
    puts("");
    exit(0); // HACKY
  }
  if (s[i] == dnaComplementaryPalindrome(s[j]))
    check(s, i + 1, j - 1, delta);
  positions.push_back(i);
  check(s, i + 1, j, delta - 1);
  positions.pop_back();
  positions.push_back(j);
  check(s, i, j - 1, delta - 1);
  positions.pop_back();
  positions.push_back(i + n);
  positions.push_back(j + n);
  check(s, i + 1, j - 1, delta - 1);
  positions.pop_back();
  positions.pop_back();
}

int main()
{
  string s;
  int delta;
  cin >> s >> delta;
  check(s, 0, (int)s.size() - 1, delta);
  return 0;
}
