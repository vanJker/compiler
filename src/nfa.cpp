#include "nfa.h"

// Just for Debug
static int counter = 0;

NFAState::NFAState() {
    this->id = counter++;
}

// construct a NFA state from given transitions
NFAState::NFAState(char c, set<NFAState*> states) {
    this->id = counter++;
    this->trans.push_back({c, states});
}

// add transition to NFA state
void NFAState::addTrans(char c, set<NFAState*> states) {
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        if (get<0>(*it) == c) {
            get<1>(*it).merge(states);
            return;
        }
    }
    this->trans.push_back({c, states});
}

// epsilon closure of the NFA state
set<NFAState*> NFAState::epsilon_closure() {
    set<NFAState*> s, procd;
    s.insert(this);
    this->epsilon_closure(&s, &procd);
    return s;
}

// DFS for epsilon closure
void NFAState::epsilon_closure(set<NFAState*>* s, set<NFAState*>* procd) {
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        if (get<0>(*it) == EPSILON && !procd->count(this)) {
            auto to = get<1>(*it);

            s->insert(to.begin(), to.end());
            procd->insert(this);

            for (auto it2 = to.begin(); it2 != to.end(); it2++) {
                (*it2)->epsilon_closure(s, procd);
            }

            break;
        }
    }
}

// next NFA states of this NFA state with char c
set<NFAState*> NFAState::delta(char c) {
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        if (get<0>(*it) == c) {
            return get<1>(*it);
        }
    }
    return {};
}

int NFAState::getID() {
    return this->id;
}

// display the NFA state
void NFAState::display() {
    cout << this->id << ": ";
    cout << "[";
    for (auto it = this->trans.begin(); it != this->trans.end(); it++) {
        cout << get<0>(*it) << " -> ";

        auto nexts = get<1>(*it);
        cout << "{";
        for (auto it2 = nexts.begin(); it2 != nexts.end(); it2++) {
            cout << (*it2)->id <<  ", ";
        }
        cout << "}";

        cout << "; ";
    }
    cout << "]";
}

NFA::NFA() {
    // default should contain two states: start and end
    this->states = {new NFAState(), new NFAState()};
    this->start = this->states.at(0);
    this->end   = this->states.at(1);
}

NFA::NFA(char c) {
    new (this)NFA();
    this->start->addTrans(c, {this->end});
}

void NFA::mergeNFA(NFA* nfa) {
    this->states.insert(this->states.end(), nfa->states.begin(), nfa->states.end());
}

// this | nfa
NFA* NFA::unionNFA(NFA* nfa) {
    NFA* res = new NFA();

    res->start->addTrans(EPSILON, {this->start, nfa->start});
    this->end->addTrans(EPSILON, {res->end});
    nfa->end->addTrans(EPSILON, {res->end});

    res->mergeNFA(this);
    res->mergeNFA(nfa);

    return res;
}

// this · nfa
NFA* NFA::concatNFA(NFA* nfa) {
    NFA *res = new NFA();

    res->start->addTrans(EPSILON, {this->start});
    this->end->addTrans(EPSILON, {nfa->start});
    nfa->end->addTrans(EPSILON, {res->end});

    res->mergeNFA(this);
    res->mergeNFA(nfa);

    return res;
}

NFA* NFA::starNFA() {
    NFA* res = new NFA();

    res->start->addTrans(EPSILON, {this->start, res->end});
    this->end->addTrans(EPSILON, {res->start, res->end});

    res->mergeNFA(this);

    return res;
}

NFA* NFA::optionNFA() {
    NFA* res = new NFA();

    res->start->addTrans(EPSILON, {this->start, res->end});
    this->end->addTrans(EPSILON, {res->end});

    res->mergeNFA(this);

    return res;
}

// display the NFA
void NFA::display() {
    cout << "start: " << this->start->getID() << endl;
    cout << "end:   " << this->end->getID()   << endl;
    for (auto it = this->states.begin(); it != this->states.end(); it++) {
        (*it)->display();
        cout << endl;
    }
}

// RE -> NFA
NFA* regep2nfa(RegEp re) {
    NFA* nfa;			// result NFA for this RE
    stack<NFA*> st1;	// stack about operand
    stack<char> st2;	// stack about operatoro
    set<char> symbols;	// Language symbols
    char c; 			// current char
    re = addConcat(re);

    for (auto it = re.begin(); it != re.end(); it++) {
        c = *it;

        if (isOperator(c)) {
            NFA* res;
            NFA* left;
            NFA* right;

            // process (RE)
            if (c == '(') {
                st2.push(c);
                continue;
            }

            char pre = st2.top();
            if (c == ')') {
                // process (RE)
                st2.pop();
                while (pre != '(') {
                    switch (pre) {
                    case '|':
                        right = st1.top(); st1.pop();
                        left  = st1.top(); st1.pop();
                        res = regep2nfa_union(left, right);
                        break;
                    case '&':
                        right = st1.top(); st1.pop();
                        left  = st1.top(); st1.pop();
                        res = regep2nfa_concat(left, right);
                        break;
                    case '*':
                        right = st1.top(); st1.pop();
                        res = regep2nfa_star(right);
                        break;
                    case '?':
                        right = st1.top(); st1.pop();
                        res = regep2nfa_option(right);
                        break;
                    }
                    st1.push(res);
                    pre = st2.top(); st2.pop();
                }
            } else if (pre != '(' && operatorPriority(pre) > operatorPriority(c)) {
                // operator proiority
                st2.pop();
                switch (pre) {
                case '&':
                    right = st1.top(); st1.pop();
                    left  = st1.top(); st1.pop();
                    res = regep2nfa_concat(left, right);
                    break;
                case '*':
                    right = st1.top(); st1.pop();
                    res = regep2nfa_star(right);
                    break;
                case '?':
                    right = st1.top(); st1.pop();
                    res = regep2nfa_option(right);
                    break;
                }
                st1.push(res);
                st2.push(c);
            } else {
                st2.push(c);
            }
        } else {
            symbols.insert(c);
            NFA* nfa = regep2nfa_basic(c);
            st1.push(nfa);
        }
    }

    assert(st2.empty());
    assert(st1.size() == 1);

    nfa = st1.top(); st1.pop();
    assert(st1.empty());

    nfa->symbols = symbols;

    return nfa;
}

// RE: a -> NFA
NFA* regep2nfa_basic(char c) {
    return new NFA(c);
}

// RE: L1|L2 -> NFA
NFA* regep2nfa_union(NFA* nfa1, NFA* nfa2) {
    return nfa1->unionNFA(nfa2);
}

// RE: L1·L2 -> NFA
NFA* regep2nfa_concat(NFA* nfa1, NFA* nfa2) {
    return nfa1->concatNFA(nfa2);
}

// RE: L1* -> NFA
NFA* regep2nfa_star(NFA* nfa) {
    return nfa->starNFA();
}

// RE: L1? -> NFA
NFA* regep2nfa_option(NFA* nfa) {
    return nfa->optionNFA();
}

