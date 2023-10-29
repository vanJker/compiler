#include "dfa.h"
#include <algorithm>

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
    sort(this->trans.begin(), this->trans.end());
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

bool DFAState::transEqual(DFAState* state) {
    return this->trans == state->trans;
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

// just for debug
static int counter1 = 0;

// construct a minial DFA state
MinDFAState::MinDFAState(set<DFAState*> s) {
    this->id = counter1++;
    this->dfa_states = s;
}

// add transition to minial DFA state
void MinDFAState::addTrans(char c, MinDFAState* state) {
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        if (get<0>(*it) == c) {
            get<1>(*it) = state;
            return;
        }
    }
    this->trans.push_back({c, state});
    sort(this->trans.begin(), this->trans.end());
}

// display the minial DFA state
void MinDFAState::display() {
    cout << this->id << ": ";

    cout << "[";
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        cout << get<0>(*it) << " -> ";
        cout << get<1>(*it)->id <<  ", ";
    }
    cout << "] | ";

    cout << "{";
    for (auto it = this->dfa_states.begin(); it != this->dfa_states.end(); it++) {
        cout << (*it)->id << ", ";
    }
    cout << "}";
}

// construct a minial DFA
MinDFA::MinDFA(DFA* dfa) {
    // DFA2MinDFA

    set<DFAState*> N, A; // Non-Accept States, Accept States
    A = dfa->end;
    for (auto s: dfa->full_list) {
        if (!dfa->end.count(s)) {
            N.insert(s);
        }
    }

    deque<set<DFAState*>> state_sets;
    state_sets.push_back(N);
    state_sets.push_back(A);
    while (!state_sets.empty()) {
        auto states = state_sets.front(); state_sets.pop_front();

        if (states.size() == 1) {
            // only one element, cann't be splited
            this->states.push_back(new MinDFAState(states));
            continue;
        }

        // split states
        bool is_split = false;
        auto s0 = *states.begin();
        for (auto it = states.begin(); it != states.end(); it++) {
            if (*it == s0) continue;

            if (!s0->transEqual(*it)) {
                set<DFAState*> temp;
                for (auto it2 = states.begin(); it2 != states.end(); it2++) {
                    if ((*it)->transEqual(*it2)) {
                        temp.insert(*it2);
                    }
                }
                set<DFAState*> res;
                set_difference(states.begin(), states.end(), temp.begin(), temp.end(), inserter(res, res.begin()));
                state_sets.push_back(temp);
                state_sets.push_back(res);

                is_split = true;

                break;
            }
        }

        if (!is_split) this->states.push_back(new MinDFAState(states));
    }

    for (auto it = this->states.begin(); it != this->states.end(); it++) {
        // start state
        if ((*it)->dfa_states.count(dfa->start)) {
            this->start = *it;
        }

        // end states
        for (auto it2 = dfa->end.begin(); it2 != dfa->end.end(); it2++) {
            if ((*it)->dfa_states.count(*it2)) {
                this->end.insert(*it);
            }
        }

        // state trans
        auto s = *((*it)->dfa_states.begin());
        for (auto it2 = s->trans.begin(); it2 != s->trans.end(); it2++) {
            auto symbol = get<0>(*it2);
            auto dest   = get<1>(*it2);

            for (auto it3 = this->states.begin(); it3 != this->states.end(); it3++) {
                if ((*it3)->dfa_states.count(dest)) {
                    (*it)->addTrans(symbol, *it3);
                }
            }
        }
    }

    // error states
    for (auto it = this->states.begin(); it != this->states.end(); it++) {
        auto s = *it;
        bool is_error = true;
        for (auto it2 = s->trans.begin(); it2 != s->trans.end(); it2++) {
            if (get<1>(*it2) != s) {
                is_error = false;
            }
        }
        if (is_error && !this->end.count(s)) {
            this->error = s;
            break;
        }
    }
}

// display the minial DFA
void MinDFA::display() {
    cout << "start: " << this->start->id << endl;
    cout << "end:   {";
    for (auto state: this->end) {
        cout << state->id << ", ";
    }
    cout << "}" << endl;
    cout << "error: " << this->error->id << endl;

    for (auto it = this->states.begin(); it != this->states.end(); it++) {
        (*it)->display();
        cout << endl;
    }
}
