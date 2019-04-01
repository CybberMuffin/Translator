#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

//#define SIZE 100000
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <vector>
#include <utility>
#include <exception>
#include <QString>

using namespace std;

struct lex_table{
    int number;
    string lexem;
    int row;
    int IDN;
};

struct con_table{
    int type;
    string lexem;
    int lex_idn;
};

struct idn_table{
    int type;
    string lexem;
    double value;
    int lex_idn;

    idn_table(int type, string lexem, double value, int lex_idn) {
        this->type = type;
        this->lexem = lexem;
        this->value = value;
        this->lex_idn = lex_idn;
    }
};

class LexicalAnalyzer
{
private:
    bool HAS_TO_READ;
    bool dot_dot;
    bool declaration_end;
    FILE *file;
    int num;
    int row;
    bool lex_empty;
    bool add;
    string lexError = "";
    vector<lex_table> table;
    vector<idn_table> idnTable;
    vector <con_table> conTable;
    vector<string> labTable;
    map <string, int> lexems = {
            { "int", 1 },
            { "float", 2 },
            { "for", 3 },
            { "to", 4 },
            { "by", 5 },
            { "while", 6 },
            { "rof", 7 },
            { "if", 8 },
            { "cin", 9 },
            { "cout", 10 },
            { "{", 11 },
            { "}", 12 },
            { "{enter}", 13 }, // \n <Enter>
            { ",", 14 },
            { "<<", 15 },
            { ">>", 16 },
            { "=", 17 },
            { "<", 18 },
            { ">", 19 },
            { "<=", 20 },
            { ">=", 21 },
            { "==", 22 },
            { "!=", 23 },
            { "+", 24 },
            { "-", 25 },
            { "*", 26 },
            { "/", 27 },
            { "(", 28 },
            { ")", 29 },
            { "goto", 30 },
            { ":", 31 },
            { "IDN", 100 },
            { "CON", 101 },
            { "LBL", 102 }
        };

    bool is_semanticly_allowed(string lex);
    int analyze_code(string lex);
    void addLex(string lexName);
    char nextChar();
    bool not_separator(char ch);
    bool is_uni_separ(char ch);
    bool is_cont_separ(char ch);
    bool is_e(string str);
    bool is_upp(string str);
    bool is_letters_exists(string name);
    bool is_variable(string name);
    bool is_nodot(string str);
    bool is_nums(string str);
    bool is_allowed_nums(string str);
    bool is_correct_name(string name);
    void run_double_separ(char ch, string lex);
    void run_digit(char ch, string lex);
    void run_delimiter(char ch, string lex);
public:
    LexicalAnalyzer();
    void error(string str);
    string getError();
    void selectFile(QString file);
    string run_table();
    void analyzer();
    vector<lex_table> get_lextable();
    vector<con_table> get_contable();
    vector<idn_table> get_idntable();
    map<string, int> getLexemsMap();
};

#endif // LEXICALANALYZER_H
