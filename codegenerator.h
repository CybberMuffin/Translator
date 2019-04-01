#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H
#include "lexicalanalyzer.h"
#include "polizmaker.h"
#include "dialog.h"

class CodeGenerator
{
private:
    vector<string> poliz;
    vector<string> polizOriginal;
    vector<lex_table> lexTable;
    vector<idn_table> idnTable;
    vector<con_table> conTable;
    string finalText = "";

    bool is_number(const std::string& s);
    bool is_all_upper(const std::string& word);
    bool is_nodot(const string& str);
    bool is_integer(const string& str);
    bool contains(string str, vector<string> vecStr);
    int findIndexInPoliz(const std::string& lexem);
    int findInIdnTable(const string& lex);
    double to_double(const std::string& symbol);
public:
    CodeGenerator();
    QString run(string previousText, vector<lex_table> lexTable, vector<idn_table> idTable,
             vector<con_table> conTable, vector<string> poliz);
};

#endif // CODEGENERATOR_H
