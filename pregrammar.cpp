#include "pregrammar.h"

PreGrammar::PreGrammar() {
    this->grammar = {
            {"<prog>", {rule({"<spOg1>", "{", "<spOp1>", "}"})}},
            {"<spOg>", {rule({"<og>"}), rule({"<spOg>", "{enter}", "<og>"})}},
            {"<spOg1>", {rule({"<spOg>"})}},
            {"<og>", {rule({"<type>", "<spId1>"})}},
            {"<type>", {rule({"int"}), rule({"float"})}},
            {"<spId>", {rule({"IDN"}), rule({"<spId>", ",", "IDN"})}},
            {"<spId1>", {rule({"<spId>"})}},
            {"<spOp>", {rule({"<op>", "{enter}"}), rule({"<spOp>", "<op>", "{enter}"})}},
            {"<spOp1>", {rule({"<spOp>"})}},
            {"<op>", {rule({"<assign>"}), rule({"<input>"}), rule({"<output>"}),  rule({"LBL", ":"}),
                      rule({"<loop>"}), rule({"<cond>"}), rule({"<gotoLabel>"})}},
            {"<input>", {rule({"cin", "<<", "IDN"}), rule({"<input>", "<<", "IDN"})}},
            {"<output>", {rule({"cout", ">>", "IDN"}), rule({"<output>", ">>", "IDN"})}},
            {"<loop>", {rule({"for", "<assign>", "to", "<E1>", "by", "<E1>", "while",
                              "<relBrackets>", "<spOp1>", "rof"})}},
            {"<cond>", {rule({"if", "<relBrackets>", "{", "<spOp1>", "}"})}},
            {"<gotoLabel>", {rule({"goto", "LBL"})}},
            {"<assign>", {rule({"IDN", "=", "<E1>"})}},
            {"<E>", {rule({"<T1>"}), rule({"<E>", "+", "<T1>"}), rule({"<E>", "-", "<T1>"})}},
            {"<E1>", {rule({"<E>"})}},
            {"<E2>", {rule({"<E1>"})}},
            {"<T>", {rule({"<F1>"}), rule({"<T>", "*", "<F1>"}), rule({"<T>", "/", "<F1>"})}},
            {"<T1>", {rule({"<T>"}), rule({"-", "<T1>"})}},
            {"<F>", {rule({"IDN"}), rule({"CON"}), rule({"(", "<E2>", ")"})}},
            {"<F1>", {rule({"<F>"})}},
            {"<relation>", {rule({"<E1>", "<z>", "<E1>"})}},
            {"<relBrackets>", {rule({"(", "<relation>", ")"})}},
            {"<z>", {rule({"<"}), rule({"<="}), rule({">"}), rule({">="}), rule({"=="}), rule({"!="})}},
    };

    /*this->grammar = {
        {"<prog>", {rule({"<spOg1>", "{", "<spOp1>", "}"})}},
        {"<spOg1>", {rule({"<spOg>"})}},
        {"<spOg>", {rule({"int", "<spId1>"}), rule({"int", "<spId1>", ";", "<spOg>"})}},
        {"<spId1>", {rule({"<spId>"})}},
        {"<spId>", {rule({"id"}), rule({"<spId>", ",", "id"})}},
        {"<spOp1>", {rule({"<spOp>"})}},
        {"<spOp>", {rule({"<spOp>", "<op>", ";"}), rule({"<op>", ";"})}},
        {"<op>", {rule({"id", "=", "id"}), rule({"cin", "id"})}}
    };*/
    /*this->grammar = {
        {"<op>", {rule({"read", "(", "<spId1>", ")", ";"})}},
        {"<spId1>", {rule({"<spId>"})}},
        {"<spId>", {rule({"id"}), rule({"<spId>", ",", "id"})}}
    };*/

    initializeTerminals();

    initializeRelations();

    setNamedLines();
}

void PreGrammar::initializeTerminals(){
    /*terminals = {"{", "}", "enter", "int", "float", "id", "con", ",", "lab", ":", "cin",
                 "<<", "cout", ">>", "for", "to", "by", "while", "(", ")", "rof", "if",
                 "goto", "=", "*", "/", "+", "-", "<", ">", "<=", ">=", "==", "!="
                 };*/
    terminals = {"{", "}", "{enter}", "int", "float", "IDN", "CON", ",", "LBL", ":", "cin",
                 "<<", "cout", ">>", "for", "to", "by", "while", "(", ")", "rof", "if",
                 "goto", "=", "*", "/", "+", "-", "<", ">", "<=", ">=", "==", "!="
                 };
    //terminals = {"{", "}", "id", ";", ",", "int", "=", "cin"};

    //terminals = {"read", "(", ")", "id", ",", ";"};
}



void PreGrammar::creatingRelations() {
    for (auto const& it : this->grammar){
        for(auto ruleObject : it.second){
            for(size_t j = 0; j < ruleObject.element.size()-1; j++){
                for(size_t k = 1; k < TABLE_SIZE; k++) {
                    if (relation[k][0] == ruleObject.element[j]) {
                        for (size_t l = 1; l < TABLE_SIZE; l++) {
                            if(relation[0][l] == ruleObject.element[j+1]){
                                this->relation[k][l] = "=";
                            }
                        }
                    }
                }
            }
        }
    }

    findFirstPlus();
    fillLessTable();

    findLastPlus();
    fillGreaterTable();
}

void PreGrammar::findFirstPlus(){
    for(auto &it : grammar){
        for(auto &object : it.second){
            processFirst(it.first, object);
            object.firstPlus = first;
            first.clear();
        }
    }
}

void PreGrammar::processFirst(string N, rule object){
    string firstSymbol = object.element[0];
    if(find(first.begin(), first.end(), firstSymbol) == first.end() && N != firstSymbol){
        first.push_back(firstSymbol);
        if(find(terminals.begin(), terminals.end(), firstSymbol) == terminals.end()){
            for(auto &it : grammar){
                if(it.first == firstSymbol) {
                    for(auto &that : it.second) {
                         processFirst(it.first, that);
                    }
                }
            }
        }
    }
}

void PreGrammar::fillLessTable(){
    for(size_t i = 1; i < relation.size(); i++){
        for(size_t j = 1; j < SIZE; j++){
            if(relation[i][j] == "="){
                for(auto &it : grammar){
                    if(it.first == relation[0][j]) {
                        vector<string> firsts = {};
                        for (auto &object : it.second) {
                            firsts.insert(firsts.end(), object.firstPlus.begin(), object.firstPlus.end());
                        }
                        setLessMark(i, firsts);
                    }
                }
            }
        }
    }
}

void PreGrammar::setLessMark(size_t leftIndex, vector<string> rightParts){
    for (size_t j = 1; j < TABLE_SIZE; j++) {
        for(auto &str : rightParts){
            if(relation[0][j] == str){
                relation[leftIndex][j] = "<";
            }
        }
    }
}


void PreGrammar::findLastPlus() {
    for(auto &it : grammar){
        for(auto &object : it.second){
            processLast(it.first, object);
            object.lastPlus = first;
            first.clear();
        }
    }
}

void PreGrammar::processLast(string N, rule object) {
    string lastSymbol = object.element[object.element.size() - 1];
    if(find(first.begin(), first.end(), lastSymbol) == first.end() && N != lastSymbol){
        first.push_back(lastSymbol);
        if(find(terminals.begin(), terminals.end(), lastSymbol) == terminals.end()){
            for(auto &it : grammar){
                if(it.first == lastSymbol) {
                    for(auto &that : it.second) {
                        processLast(it.first, that);
                    }
                }
            }
        }
    }
}

void PreGrammar::fillGreaterTable() {
    for(size_t i = 1; i < SIZE; i++){
        for(size_t j = 1; j < relation[i].size(); j++){
            if(relation[i][j] == "="){
                for(auto &it : grammar){
                    if(it.first == relation[i][0]) {
                        vector<string> lasts = {};
                        for (auto &object : it.second) {
                            lasts.insert(lasts.end(), object.lastPlus.begin(), object.lastPlus.end());
                        }
                        setGreaterMark(j, lasts);
                        if(find(terminals.begin(), terminals.end(), relation[0][j]) == terminals.end()){
                            for(auto &that : grammar){
                                if(that.first == relation[0][j]){
                                    vector<string> rightFirst = {};
                                    for(auto &some : that.second){
                                        rightFirst.insert(rightFirst.begin(), some.firstPlus.begin(), some.firstPlus.end());
                                    }
                                    operateFirstLast(lasts, rightFirst);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void PreGrammar::setGreaterMark(size_t rightIndex, vector<string> leftParts) {
    for(size_t i = 1; i < TABLE_SIZE; i++){
        for(auto &str : leftParts){
            if(relation[i][0] == str){
                relation[i][rightIndex] = ">";
            }
        }
    }
}

void PreGrammar::operateFirstLast(vector<string> leftLast, vector<string> rightFirst){
    for(size_t i = 1; i < TABLE_SIZE; i++) {
        for(size_t j = 1; j < TABLE_SIZE; j++) {
            for(auto &last : leftLast){
                for(auto &first : rightFirst){
                    if(relation[i][0] == last && relation[0][j] == first){
                        relation[i][j] = ">";
                    }
                }
            }
        }
    }
}

void PreGrammar::additional(){
    for(auto &it : grammar){
        vector<string> lasts = {};
        vector<string> firsts = {};
        for(auto &object : it.second){
            lasts.insert(lasts.end(), object.lastPlus.begin(), object.lastPlus.end());
            firsts.insert(firsts.end(), object.firstPlus.begin(), object.firstPlus.end());
        }
        for(auto &last : lasts){
            for(auto &first : firsts){
                for (size_t i = 1; i < TABLE_SIZE; i++) {
                    if (relation[i][0] == last) {
                        for (size_t j = 1; j < TABLE_SIZE; j++) {
                            if (relation[0][j] == first) {
                                this->relation[i][j] = ">";
                            }
                        }
                    }
                }
            }
        }
    }

}





void PreGrammar::initializeRelations() {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        vector<string> relative;
        for (size_t j = 0; j < TABLE_SIZE; j++) {
            relative.emplace_back(" ");
        }
        this->relation.push_back(relative);
    }
}

void PreGrammar::setNamedLines() {
    auto it = this->grammar.begin();
    for (size_t i = 1; i < SIZE; i++) {
        this->relation[i][0] = it->first;
        this->relation[0][i] = it->first;
        it++;
    }
    addTerminals();
}

void PreGrammar::addTerminals() {
    initializeTerminals();
    size_t j = 0;
    for (size_t i = SIZE; i < TABLE_SIZE; i++) {
       relation[i][0] = terminals[j];
       relation[0][i] = terminals[j];
       j++;
    }
}

void PreGrammar::runTable() {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        for (size_t j = 0; j < TABLE_SIZE; j++) {
            cout << this->relation[i][j] << "\t";
        }
        cout << "\n\n";
    }
}

vector<vector<string>> PreGrammar::getRelationTable() {
    return relation;
}

vector<string> PreGrammar::getTerminals() {
    return terminals;
}

vector<pair<string, vector<rule>>> PreGrammar::getGrammar(){
    return grammar;
}



