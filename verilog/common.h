#ifndef __COMMON_H_DEFINED__
#define __COMMON_H_DEFINED__

#include <ctype.h>
#include <string.h>
#include <set>
#include <vector>
#include "cuddObj.hh"

typedef std::vector<std::string> stringlist_t;
typedef std::set<std::string> stringset_t;

struct string_cmp_t {
    bool operator() (const char* s1, const char* s2) const {
        return strcmp(s1, s2) < 0;
    }
};

struct string_case_cmp_t {
    bool operator() (const char* s1, const char* s2) const {
        return strcasecmp(s1, s2) < 0;
    }
};

// does the string 's' start with the string 'p'?
bool startswith(const char* s, const char* p);

char* strtolower(char* s);
char* strup_dup(const char* s);
void printBDD(FILE* out, BDD bdd);

bool is_number(const std::string& s);

int gcd(int a, int b);

#endif /* __COMMON_H_DEFINED__ */
