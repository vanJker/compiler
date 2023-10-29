#include "codegen.h"

string codeGen(MinDFA* dfa) {
    string res;
    string blanks = string(4, ' ');
    string ident;

    res += ident + "bool scanner(char *str) {\n";
    ident += blanks;

    res += ident + "int state = " + to_string(dfa->start->id) + ";\n";
    res += ident + "char c;\n";

    res += ident + "while (1) {\n";
    ident += blanks;

    res += ident + "switch (state) {\n";
    ident += blanks;

    for (auto it = dfa->states.begin(); it != dfa->states.end(); it++) {
        auto s = *it;
        res += ident + "case " + to_string(s->id) + ":\n";
        ident += blanks;

        res += ident + "c = *str++;\n";

        // Accept state
        if (dfa->end.count(s)) {
            res += ident + "return true;\n";
            ident.erase(ident.size() - blanks.size(), blanks.size());
            continue;
        }

        // Error state
        if (dfa->error == s) {
            res += ident + "return false;\n";
            ident.erase(ident.size() - blanks.size(), blanks.size());
            continue;
        }

        res += ident + "switch (c) {\n";
        ident += blanks;

        for (auto it2 = s->trans.begin(); it2 != s->trans.end(); it2++) {
            auto symbol = get<0>(*it2);
            auto next_state = get<1>(*it2);

            res += ident + "case '" + symbol + "':\n";
            ident += blanks;

            res += ident + "state = " + to_string(next_state->id) + "\n";
            res += ident + "break;\n";

            ident.erase(ident.size() - blanks.size(), blanks.size());
        }
        ident.erase(ident.size() - blanks.size(), blanks.size());

        res += ident + "}\n";
        res += ident + "break;\n";
        ident.erase(ident.size() - blanks.size(), blanks.size());
    }
    ident.erase(ident.size() - blanks.size(), blanks.size());

    res += ident + "}\n";
    ident.erase(ident.size() - blanks.size(), blanks.size());

    res += ident + "}\n";
    ident.erase(ident.size() - blanks.size(), blanks.size());

    res += ident + "}\n";

    return res;
}
