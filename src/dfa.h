#ifndef DFA_H
#define DFA_H

#include "nfa.h"

#include <deque>

class DFAState {
public:
    int id;
    set<NFAState*> nfa_states;
    vector<tuple<char, DFAState*>> trans;

    DFAState(set<NFAState*> s);
    void addTrans(char c, DFAState* state);
    DFAState* epsilon_closure();
    DFAState* delta(char c);
    bool containsNFAState(NFAState* state);
    void display();
};

class DFA {
public:
    DFAState* start;
    set<DFAState*> end;
    deque<DFAState*> full_list, work_list;

    DFA(NFA *nfa);
    void display();

private:
    bool inFullList(DFAState* state);
    DFAState* convert(DFAState* state);
};

#endif // DFA_H
