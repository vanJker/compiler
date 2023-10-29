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
    bool transEqual(DFAState* state);
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

class MinDFAState {
public:
    int id;
    set<DFAState*> dfa_states;
    vector<tuple<char, MinDFAState*>> trans;

    MinDFAState(set<DFAState*> s);
    void addTrans(char c, MinDFAState* state);
    void display();
};

class MinDFA {
public:
    MinDFAState* start;
    set<MinDFAState*> end;
    MinDFAState* error;
    vector<MinDFAState*> states;

    MinDFA(DFA* dfa);
    void display();
};

#endif // DFA_H
