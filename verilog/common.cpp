#include "common.h"
#include <ctype.h>

bool startswith(const char* s, const char* p)
{
    return (strstr(s, p) == s);
}

char* strtolower(char* s) 
{
    char* p = s;
    for(;s && *s != '\0'; s++) {
        *s = tolower(*s);
    }
    return p;
}

char* strup_dup(const char* s)
{
    char* upperstr = strdup(s);
    for(char* p = upperstr; *p != '\0'; p++) {
        *p = toupper(*p);
    }
    return upperstr;
}

void printBDD(FILE* out, BDD bdd)
{
    DdNode* node = bdd.getNode();
    Cudd_DumpFactoredForm(bdd.manager()->getManager(), 1, &node, NULL, NULL, out); 
    fprintf(out, "\n");
}

int gcd(int a, int b)
{
    if((a % b) == 0) return b;
    else if(a == b) return a;
    else if(a < b) return gcd(b, a);
    else {
        return gcd(b, a%b);
    }
}

bool is_number(const std::string& s)
{
    unsigned i=0;
    for(i=0; i != s.size(); i++) {
        if(!isdigit(s[i])) return false;
    }
    return true;
}
