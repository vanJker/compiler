#ifndef NFA_H
#define NFA_H

#include "re.h"

#include <vector>
#include <tuple>
#include <set>
#include <stack>
#include <iostream>
#include <assert.h>
using namespace std;

class NFAState {
private:
    int id;
    vector<tuple<char, set<NFAState*>>> trans;

public:
    NFAState();
    NFAState(char c, set<NFAState*> states);
    void addTrans(char c, set<NFAState*> states);
    set<NFAState*> epsilon_closure();
    set<NFAState*> delta(char c);
    int getID();
    void display();

private:
    void epsilon_closure(set<NFAState*>* s, set<NFAState*>* procd);
};

class NFA {
public:
    NFAState* start;
    NFAState* end;
    vector<NFAState*> states;
    set<char> symbols;

public:
    NFA();
    NFA(char c);
    NFA* unionNFA(NFA* nfa);
    NFA* concatNFA(NFA* nfa);
    NFA* starNFA();
    NFA* optionNFA();
    void display();

private:
    void mergeNFA(NFA* nfa);
};

NFA* regep2nfa(RegEp re);
NFA* regep2nfa_basic(char c);
NFA* regep2nfa_union(NFA* nfa1, NFA* nfa2);
NFA* regep2nfa_concat(NFA* nfa1, NFA* nfa2);
NFA* regep2nfa_star(NFA* nfa1);
NFA* regep2nfa_option(NFA* nfa1);

#endif // NFA_H
