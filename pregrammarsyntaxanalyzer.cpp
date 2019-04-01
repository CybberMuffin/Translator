#include "pregrammarsyntaxanalyzer.h"


PregrammarSyntaxAnalyzer::PregrammarSyntaxAnalyzer()
{
}

string PregrammarSyntaxAnalyzer::run(vector<vector<string>> relationTable, vector<lex_table> tab, vector<string> terminals,
                                      map<string, int> lexems, vector<pair<string, vector<rule>>> grammar)
{
    this->grammar = grammar;
    this->table = tab;
    reverse(this->table.begin(), this->table.end());
    this->relationTable = relationTable;
    this->terminals = terminals;
    this->lexems = lexems;


    string currentRelation;
    stack.push_back(lexemsMapConverter(table.back().IDN));
    table.pop_back();
    do {
        if(stack.back() == "<spOg1>") declarationFinished = true;

        currentRelation = getRelation(stack.back(), lexemsMapConverter(table.back().IDN));
        pushIntoAnalysisTable(currentRelation);

        checkPolizTail();

        if(currentRelation == "=" || currentRelation == "<") {
            checkPolizKeyWords();

            stack.push_back(lexemsMapConverter(table.back().IDN));
            table.pop_back();
        } else if(currentRelation == ">")
            inStackOperation();
        else {
            text = "Error at line: " + to_string(table.back().row) + ", at " + table.back().lexem + "\n\n";
            break;
        }

        if(table.empty()) analysisTable.emplace_back(reduceIntoString(stack), "", "", "");

    } while(!table.empty());

    while(stack.size() > 1) inStackOperation();


    for(auto it : polizMaker->getPoliz()) {
      //text += it + " ";
      cout << it + " ";
    }

    //if(table.empty()) text += this->polizMaker->showCalculation();

    table.empty() ? text += "\nProgram is running\n" : text += "\nFailure\n";
    //QString qText = QString::fromStdString(text);
    return text;
}

string PregrammarSyntaxAnalyzer::getRelation(string leftSymbol, string rightSymbol) {
    for(size_t i = 1; i < relationTable.size(); i++){
        if(relationTable[i][0] == leftSymbol) {
            for(size_t j = 1; j < relationTable[i].size(); j++){
                if(relationTable[0][j] == rightSymbol){
                    return relationTable[i][j];
                }
            }
        }
    }
    return "Error";
}

string PregrammarSyntaxAnalyzer::lexemsMapConverter(int IDN){
    for (auto it = lexems.begin(); it != lexems.end(); ++it )
        if (it->second == IDN)
            return it->first;

    return "Error";
}

string PregrammarSyntaxAnalyzer::getElementForRule(vector<string> buffer) {
    for (auto const& it : this->grammar){
        for(auto ruleObject : it.second){
            if(ruleObject.element == buffer){
                if(it.first == "<spId>" && declarationFinished) continue;
                return it.first;
            }
        }
    }
    return "Error";
}

void PregrammarSyntaxAnalyzer::inStackOperation() {
    vector<string> buffer;
    string stackRelation;

    do {
        stack.size() < 2 ? stackRelation = "<" : stackRelation = getRelation(stack[stack.size() - 2], stack.back());
        buffer.push_back(stack.back());
        stack.pop_back();
    } while(stackRelation != "<");

    reverse(buffer.begin(), buffer.end());

    stack.push_back(getElementForRule(buffer));

    polizChecker(buffer);
}

void PregrammarSyntaxAnalyzer::polizChecker(vector<string> buffer) {
    this->polizMaker->arithemticPoliz(buffer);
    if(stack.back() == "<z>") relations.push_back(buffer[0]);
    if(stack.back() == "<relation>") {
        this->polizMaker->logicalExpression(relations.back());
        relations.pop_back();
    }

    if(stack.back() == "<op>" && buffer[0] == "LBL") this->polizMaker->addLabel();
    if(stack.back() == "<gotoLabel>") this->polizMaker->jump();
    if(stack.back() == "<output>" || stack.back() == "<input>") this->polizMaker->inOutput(stack.back());
}

void PregrammarSyntaxAnalyzer::checkPolizTail() {
    if(stack.back() == "<relBrackets>") this->polizMaker->endLogExp();

    if(stack.back() == "<cond>") this->polizMaker->endCond();

    if(stack.back() == "<loop>") this->polizMaker->endLoop();
}

void PregrammarSyntaxAnalyzer::checkPolizKeyWords() {
    if((table.back().IDN == 100 || table.back().IDN == 101 || table.back().IDN == 102) && declarationFinished) {
        this->polizMaker->addIdns(table.back().lexem);
    }

    if(table.back().IDN == 8 || (table.back().IDN > 2 && table.back().IDN < 7))
        this->polizMaker->addToStack(table.back().lexem);
}

void PregrammarSyntaxAnalyzer::pushIntoAnalysisTable(string relation) {
    string stackStr = "", tableStr = "", polizStr = "";
    for(auto str: stack) stackStr += " " + str;
    if(!table.empty()){
        for(int i = table.size() - 1; i > -1; i--)
            tableStr += " " + table[i].lexem;
    }
    if(!polizMaker->getPoliz().empty()){
        for(auto str: polizMaker->getPoliz()) polizStr += " " + str;
    }

    analysisTable.emplace_back(stackStr, relation, tableStr, polizStr);
}

bool PregrammarSyntaxAnalyzer::contains(string str, vector<string> vecStr) {
    if (find(vecStr.begin(), vecStr.end(), str) != vecStr.end())
        return true;
    return false;
}

string PregrammarSyntaxAnalyzer::reduceIntoString(vector<string> some){
    string total = "";
    for(auto it : some) total += " " + it;
    return total;
}

vector<analys> PregrammarSyntaxAnalyzer::getAnalysisTable() {
    return analysisTable;
}

vector<polizTable> PregrammarSyntaxAnalyzer::getPolizTable() {
    return polizMaker->getTable();
}

vector<string> PregrammarSyntaxAnalyzer::getPoliz() {
    return polizMaker->getPoliz();
}
