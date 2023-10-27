#include "re.h"

// operator priority
int operatorPriority(char c) {
    if (c == '|') return 0;
    if (c == '&') return 1;
    if (c == '*') return 2;
    if (c == '?') return 2;
    if (c == '(') return 3;
    if (c == ')') return 3;
    else return -1;
}

bool isOperator(char c) {
    return c == '&' || c == '|' || c == '*' || c == '?' || c == '(' || c == ')';
}

// add concat operator as '&, and () to contain RE
RegEp addConcat(RegEp re) {
    RegEp res;
    res = "(";
    char pre; // previous char
    for (auto it = re.begin(); it != re.end(); it++) {
        char c = *it;
        if (res.length() > 0
                && (isalpha(pre) || pre == '*' || pre == '?' || pre == ')')
                && isalpha(c) || c == '(') {
            res += '&';
        }
        res += c;
        pre = c;
    }
    res += ")";
    return res;
}
