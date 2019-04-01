#ifndef AUTOSYNTACTICALANALYZER_H
#define AUTOSYNTACTICALANALYZER_H

#include "lexicalanalyzer.h"

struct autom {
    int alpha;
    int label;
    int stack;
    int beta;

    autom(int alpha, int beta, int label = -1,  int stack = 0){
        this->alpha = alpha;
        this->label = label;
        this->beta = beta;
        this->stack = stack;
    }
};

struct convers {
    string lexem;
    int state;
    int stack;
    convers(string lexem, int state, int stack){
        this->lexem = lexem;
        this->state = state;
        this->stack = stack;
    }
};

class AutoSyntacticalAnalyzer
{
private:
    vector<lex_table> table;
    size_t index;
    vector<autom> ways;
    vector<int> main_stack;
    vector<convers> conversion;
public:
    AutoSyntacticalAnalyzer();
    int getNextState(int alpha, int label);
    QString run(vector<lex_table> lexems, string lexError);
    vector<autom> getWays();
    vector<convers> getConversion();
};

#endif // AUTOSYNTACTICALANALYZER_H
