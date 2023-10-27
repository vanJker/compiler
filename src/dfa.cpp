#include "dfa.h"

// Just for Debug
static int counter = 0;

// construct a DFA
DFAState::DFAState(set<NFAState*> s) {
    this->id = counter++;
    this->nfa_states = s;
}

// add transition to DFA state
void DFAState::addTrans(char c, DFAState* state) {
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        if (get<0>(*it) == c) {
            get<1>(*it) = state;
            return;
        }
    }
    this->trans.push_back({c, state});
}

// epsilon closure of DFA state
DFAState* DFAState::epsilon_closure() {
    set<NFAState*> s;
    for (auto it = this->nfa_states.begin(); it != this->nfa_states.end(); it++) {
        auto temp = (*it)->epsilon_closure();
        s.insert(temp.begin(), temp.end());
    }
    return new DFAState(s);
}

// delta with given symbol c of DFA state
DFAState* DFAState::delta(char c) {
    set<NFAState*> s;
    for (auto it = this->nfa_states.begin(); it != this->nfa_states.end(); it++) {
        auto temp = (*it)->delta(c);
        s.insert(temp.begin(), temp.end());
    }
    return new DFAState(s);
}

// is the DFA state contains given NFA state
bool DFAState::containsNFAState(NFAState* state) {
    return this->nfa_states.count(state);
}

// display the DFA state
void DFAState::display() {
    cout << this->id << ": ";

    cout << "[";
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        cout << get<0>(*it) << " -> ";
        cout << get<1>(*it)->id <<  ", ";
    }
    cout << "] | ";

    cout << "{";
    for (auto it = this->nfa_states.begin(); it != this->nfa_states.end(); it++) {
        cout << (*it)->getID() << ", ";
    }
    cout << "}";
}

// NFA2DFA
DFA::DFA(NFA* nfa) {
    // d0 = epsilon-closure(n0);
    DFAState* d0 = new DFAState(nfa->start->epsilon_closure());
    this->start = d0; // DFA start state

    // add d0 to FullList and WorkList
    full_list.push_back(d0);
    work_list.push_back(d0);

    while (!work_list.empty()) {
        auto d = work_list.front(); work_list.pop_front();
        for (auto c: nfa->symbols) {
            auto t = convert(d->delta(c)->epsilon_closure());
            d->addTrans(c, t);

            if (!inFullList(t)) {
                full_list.push_back(t);
                work_list.push_back(t);

                // DFA end state
                if (t->containsNFAState(nfa->end)) {
                    this->end.insert(t);
                }
            }
        }
    }
}

// is the given DFA state in FullList
bool DFA::inFullList(DFAState* state) {
    for (auto t: full_list) {
        if (t == state) {
            return true;
        }
    }
    return false;
}

// convert given DFA state to equivalent DFA state in FullList
DFAState* DFA::convert(DFAState* state) {
    for (auto t: full_list) {
        if (t->nfa_states == state->nfa_states) {
            return t;
        }
    }
    return state;
}

// display the DFA
void DFA::display() {
    cout << "start: " << this->start->id << endl;
    cout << "end:   {";
    for (auto state: this->end) {
        cout << state->id << ", ";
    }
    cout << "}" << endl;

    for (auto it = this->full_list.begin(); it != this->full_list.end(); it++) {
        (*it)->display();
        cout << endl;
    }
}
