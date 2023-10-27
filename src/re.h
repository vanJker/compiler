#ifndef RE_H
#define RE_H

#include <string>
using namespace std;

#define EPSILON '$'
typedef string RegEp;

int operatorPriority(char c);
bool isOperator(char c);
RegEp addConcat(RegEp re);

#endif // RE_H
