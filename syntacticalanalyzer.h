#ifndef SYNTACTICALANALYZER_H
#define SYNTACTICALANALYZER_H
#include "lexicalanalyzer.h"

class SyntacticalAnalyzer
{
private:
    size_t i;
    vector<lex_table> table;
    vector<con_table> conTable;
    vector<string> errors;

    bool prog();
    bool spOg();
    bool og();
    bool _type();
    bool spOp();
    bool op();
    bool assign();
    bool into();
    bool outto();
    bool loop();
    bool condition_go();
    bool gotoLabel();
    bool E();
    bool T();
    bool F();
    bool relation();
    bool z();
    bool high();
    bool low();
    void error(string str);

public:
    SyntacticalAnalyzer();
    QString run(vector<lex_table> lexems, vector<con_table> con, string lexError, string table);
};

#endif // SYNTACTICALANALYZER_H
