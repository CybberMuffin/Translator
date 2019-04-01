#ifndef PREGRAMMARSYNTAXANALYZER_H
#define PREGRAMMARSYNTAXANALYZER_H
#include "lexicalanalyzer.h"
#include "pregrammar.h"
#include <algorithm>
#include <sstream>
#include <polizmaker.h>

struct analys {
    string stack;
    string relation;
    string table;
    string poliz;
    analys(string stack, string relation, string table, string poliz){
        this->stack = stack;
        this->relation = relation;
        this->table = table;
        this->poliz = poliz;
    }
};

class PregrammarSyntaxAnalyzer
{
private:
    vector<pair<string, vector<rule>>> grammar;
    vector<lex_table> table;
    vector<vector<string>> relationTable;
    vector<string> terminals;
    map <string, int> lexems;
    PolizMaker *polizMaker = new PolizMaker;

    vector<string> stack;
    vector<analys> analysisTable;
    string text;
    bool declarationFinished = false;
    vector<string> relations;


    string getRelation(string leftSymbol, string rightSymbol);
    string lexemsMapConverter(int IDN);
    void pushIntoAnalysisTable(string relation);
    void inStackOperation();
    bool contains(string str, vector<string> vecStr);
    string reduceIntoString(vector<string> some);
    string getElementForRule(vector<string> buffer);

    void polizChecker(vector<string> buffer);
    void checkPolizTail();
    void checkPolizKeyWords();
public:
    PregrammarSyntaxAnalyzer();
    string run(vector<vector<string>> relationTable, vector<lex_table> table, vector<string> terminals,
                map <string, int> lexems, vector<pair<string, vector<rule>>> grammar);
    vector<analys> getAnalysisTable();
    vector<polizTable> getPolizTable();
    vector<string> getPoliz();
};

#endif // PREGRAMMARSYNTAXANALYZER_H

