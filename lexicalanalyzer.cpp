#include "lexicalanalyzer.h"

LexicalAnalyzer::LexicalAnalyzer()
{
    HAS_TO_READ = true;
    dot_dot = false;
    declaration_end = false;
    num = 1;
    row = 1;
    lex_empty = true;
    add = false;
}
void LexicalAnalyzer::selectFile(QString file){
    string str = file.toUtf8().constData();
    char cstr[str.size() + 1];
    strcpy(cstr, str.c_str());
    FILE *newfile = fopen(cstr, "r+");
    this->file = newfile;
    //fgets(this->text, SIZE, this->file);
}

void LexicalAnalyzer::error(string str) {
    this->lexError += str + '\n' + '\n';
}

bool LexicalAnalyzer::is_semanticly_allowed(string lex) {
    //auto it = find(idnTable.begin(), idnTable.end(), lex);
    auto it = find_if(idnTable.begin(), idnTable.end(),
                   [&lex](const idn_table& obj) {return obj.lexem == lex;});
    if (!declaration_end)
    {
        if (it != idnTable.end()) {
            error("Variable " + lex + " on row " + to_string(row) + " is already declared");
            return false;
        }
        else {
            size_t i = 0;
            int type = 0;
            while(i < table.size()) {
                if(table[i].row == row) {
                    if(table[i].IDN == 1) type = 1;
                    else if(table[i].IDN == 2) type = 2;
                }
                i++;
            }
            idnTable.emplace_back(type, lex, NULL, this->num);
            return true;
        }
    }
    else {
        if (it != idnTable.end())
            return true;
        else {
            error("Variable " + lex + " on row " + to_string(row) + " is not declared");
            return false;
        }
    }
}

int LexicalAnalyzer::analyze_code(string lex) {
    auto it = lexems.find(lex);
    if (it != lexems.end()) {
        if (it->second == 11)
            declaration_end = true;
        return it->second;
    }
    else {
        if (is_variable(lex)) {
            if (is_semanticly_allowed(lex)) {
                return 100;
            }
        }
        else if (is_nums(lex) || is_allowed_nums(lex)) {
            con_table con;
            con.lex_idn = 101;
            con.lexem = lex;
            if(is_nodot(lex))
                con.type = 1;
            else con.type = 2;
            conTable.push_back(con);
            return 101;
        }
        else if (is_upp(lex)) {
            labTable.push_back(lex);
            return 102;
        }
        else return -1;
    }
}

void LexicalAnalyzer::addLex(string lexName)
{
    lex_table lexem;
    lexem.number = num;
    lexem.lexem = lexName;
    lexem.row = row;
    lexem.IDN = analyze_code(lexName);
    //lexem.row = num;
    //lexem.CON_id = lexem.IDN == 101 ? conTable.size() + 1 : '-';
    table.push_back(lexem);
    num++;
}

char LexicalAnalyzer::nextChar()
{
    return (char)getc(file);
    //return this->text[index++];
}

bool LexicalAnalyzer::not_separator(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
        ch == '/' || ch == ',' || ch == '<' || ch == '>' ||
        ch == '!' || ch == '=' || ch == ':' || ch == '\n') {
        return false;
    }
    else {
        return true;
    }
}

bool LexicalAnalyzer::is_uni_separ(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '(' ||
        ch == ')' || ch == '{' || ch == '}' ||
        ch == '/' || ch == ',' || ch == ':') {
        return true;
    }
    else {
        return false;
    }
}

bool LexicalAnalyzer::is_cont_separ(char ch)
{
    if (ch == '<' || ch == '>' || ch == '=' || ch == '!')
        return true;
    else return false;
}

bool LexicalAnalyzer::is_e(string str)
{
    for (int i = 0; i< str.length(); i++) {
        if (str[i] == 'e')
        {
            if (is_nums(str.substr(0, i))) {
                if (isdigit(str[i + 1]))
                    return true;
                else if (str[i + 1] == '+' || str[i + 1] == '-')
                {
                    if (isdigit(str[i + 2]))
                        return true;
                    else return false;
                }
                else return false;
            }
            else return false;
        }
    }
    return false;
}

bool LexicalAnalyzer::is_upp(string str) {

    for (int i = 0; i< str.length(); i++) {
        if (!isupper(str[i]))
            return false;
    }

    return true;
}

bool LexicalAnalyzer::is_letters_exists(string name)
{
    bool let = false;
    for (int i = 0; i< name.length(); i++) {
        if ((islower(name[i])))
            let = true;
    }

    return let;
}

bool LexicalAnalyzer::is_variable(string name)
{
    for (size_t i = 0; i < name.length(); i++)
    {
        if (!islower(name[i]) && !(isdigit(name[i]) && i != 0))
            return false;
    }
    return true;
}

bool LexicalAnalyzer::is_nodot(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '.')
            return false;
    }
    return true;
}

bool LexicalAnalyzer::is_nums(string str)
{
    bool num = true;
    for (int i = 0; i< str.length(); i++) {
        if (!(isdigit(str[i]) || str[i] == '.' && is_nodot(str.substr(0, i))))
            num = false;
    }

    return num;
}

bool LexicalAnalyzer::is_allowed_nums(string str) {
    bool once = false;
    bool sign = false;
    for (int i = 0; i < str.length(); i++)
    {
        if (!(isdigit(str[i]) || str[i] == '.' && is_nodot(str.substr(0, i)))) {
            if (str[i] == 'e' && once == false)
                once = true;
            else if ((str[i] == '+' || str[i] == '-') && sign == false)
                sign = true;
            else return false;
        }
    }
    return true;
}

bool LexicalAnalyzer::is_correct_name(string name)
{
    char ch = name[0];

    if (is_variable(name) || is_nums(name) || is_upp(name) || is_e(name))
        return true;
    else return false;
}

void LexicalAnalyzer::run_double_separ(char ch, string lex)
{
    bool sep = false;
    if (ch == '<')
    {
        ch = nextChar();
        if (ch == '<' || ch == '=')
            sep = false;
        else sep = true;
    }
    else if (ch == '>')
    {
        ch = nextChar();
        if (ch == '>' || ch == '=')
            sep = false;
        else sep = true;
    }
    else if (ch == '=')
    {
        ch = nextChar();
        if (ch == '=')
            sep = false;
        else sep = true;
    }
    else if (ch == '!')
    {
        ch = nextChar();
        if (ch == '=')
            sep = false;
        else {
            error("Not correct character `!` on row: " + to_string(row));
        }
    }
    else {
        sep = true;
    }

    if (sep == false) {
        lex += ch;
        HAS_TO_READ = true;
    }

    addLex(lex);
}

void LexicalAnalyzer::run_digit(char ch, string lex)
{
    if (ch == '.' && !is_nodot(lex) && is_nums(lex)) {
        addLex(lex);
        dot_dot = true;
    }
    else dot_dot = false;
}

void LexicalAnalyzer::run_delimiter(char ch, string lex)
{
    if (is_cont_separ(ch) || is_uni_separ(ch) || ch == '\n')
    {
        if (!lex.empty() && (ch == '+' || ch == '-') && lex[lex.length() - 1] == 'e') {
            if (is_nums(lex.substr(0, lex.find('e')))) {
                lex_empty = false;
                return;
            }
        }

        if (!lex.empty())
        {
            if (is_correct_name(lex)) {
                addLex(lex);
                lex = "";
            }
            else {
                error("Lexem " + lex + " is not allowed on row: " + to_string(row));
            }
        }

        if (is_cont_separ(ch))
        {
            lex += ch;
            run_double_separ(ch, lex);
        }
        else if (is_uni_separ(ch))
        {
            lex += ch;
            addLex(lex);
        }
        else if (ch == '\n')
        {
            lex += "{enter}";
            addLex(lex);
            row++;
            HAS_TO_READ = true;
        }
    }
    else if (ch == ' ')
    {
        if (is_correct_name(lex) && !lex.empty())
        {
            addLex(lex);
            HAS_TO_READ = true;
        }
        else {
            if (!lex.empty()) {
                error("Lexem " + lex + " is not allowed on row: " + to_string(row));
            }
        }
    }
    else {
        error("Unexpected character " + string(1, ch) + " on row: " + to_string(row));
    }
    lex_empty = true;
}

string LexicalAnalyzer::run_table() {
    string text = "";
    text += string(1, '#') + "\t\t" + "Row" + "\t\t"
        + "Lexem" + "\t\t\t" + "IDN" + '\n';
    for (size_t i = 0; i < table.size(); i++) {
        text += to_string(table[i].number) + "\t\t" + to_string(table[i].row) + "\t\t"
            + table[i].lexem + "\t\t\t" + to_string(table[i].IDN) + '\n';
    }
    //QString qText = QString::fromStdString(text);
    //return qText;
    return text;
}

void LexicalAnalyzer::analyzer()
{
    char ch;
    string lex = "";

    while (!feof(file))
    {

        ch = nextChar();

        if (!isalnum(ch) && not_separator(ch) && ch != '.') {
            if (!feof(file)) {
                error("Unexpected character " + string(1, ch) + " on row: " + to_string(row));
            }
        }

        if (!not_separator(ch)) {
            run_delimiter(ch, lex);
            if (lex_empty)
                lex = "";
            else lex += ch;
        }

        else if (isalpha(ch)) {
            lex += ch;
        }

        else if (isdigit(ch) || ch == '.') {
            lex += ch;
        }
    }
    for(auto it: idnTable) {
        cout << it.lex_idn << " " << it.lexem << " " << it.type << " " << it.value << endl;
    }
}

vector<lex_table> LexicalAnalyzer::get_lextable() {
    return table;
}

vector<con_table> LexicalAnalyzer::get_contable() {
    return conTable;
}

string LexicalAnalyzer::getError(){
    return this->lexError;
}

map<string, int> LexicalAnalyzer::getLexemsMap() {
    return lexems;
}

vector<idn_table> LexicalAnalyzer::get_idntable() {
    return idnTable;
}
