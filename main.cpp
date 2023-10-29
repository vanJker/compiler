#include "mainwindow.h"
#include "src/nfa.h"
#include "src/dfa.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();


/****TEST****/

//    nfa = regep2nfa_basic("a");
//    nfa->display();

//    nfa = regep2nfa_union("b", "c");
//    nfa->display();

//    nfa = regep2nfa_concat("j", "k");
//    nfa->display();

//    nfa = regep2nfa_star("h");
//    nfa->display();

//    nfa = regep2nfa_option("g");
//    nfa->display();

    cout << "\nRE\n";
    RegEp test = "a(a|b)*";
    cout << test << endl;
    cout << addConcat(test) << endl;

    cout << "\nNFA\n";
    auto nfa = regep2nfa(test);
    nfa->display();

    cout << "\nLanguage Symbols\n";
    cout << "{";
    for (auto c: nfa->symbols) {
        cout << c << ", ";
    }
    cout << "}" << endl;

    cout << "\nNFA EPSILON\n";
    for (auto it = nfa->states.begin(); it != nfa->states.end(); it++) {
        cout << (*it)->getID() << ": {";

        auto s = (*it)->epsilon_closure();
        for (auto it2 = s.begin(); it2 != s.end(); it2++) {
            cout << (*it2)->getID() << ", ";
        }
        cout << "}" << endl;
    }


    cout << "\nNFA Delta\n";
    for (auto it = nfa->states.begin(); it != nfa->states.end(); it++) {
        cout << (*it)->getID() << ": [";

        set<char> Z = {EPSILON, 'a', 'b'};
        for (auto it2 = Z.begin(); it2 != Z.end(); it2++) {
            auto s = (*it)->delta(*it2);
            cout << *it2 << " -> " << "{";
            for (auto it3 = s.begin(); it3 != s.end(); it3++) {
                cout << (*it3)->getID() << ", ";
            }
            cout << "}, ";
        }
        cout << "]" << endl;
    }

    cout << "\nDFA\n";
    auto dfa = new DFA(nfa);
    dfa->display();

    cout << "\nMinial DFA\n";
    auto min_dfa = new MinDFA(dfa);
    min_dfa->display();

    return 0;
}
