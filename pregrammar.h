#ifndef PREGRAMMAR_H
#define PREGRAMMAR_H

#include "lexicalanalyzer.h"
#define SIZE 27//4//9//27
#define TABLE_SIZE 61//10//17//61
struct rule{
    vector<string> element;
    vector<string> firstPlus;
    vector<string> lastPlus;

    rule(vector<string> el){
        this->element = el;
    }
};

class PreGrammar {
private:
    vector<pair<string, vector<rule>>> grammar;
    vector<vector<string>> relation;
    vector<string> terminals;
    vector<string> first;

    void initializeRelations();
    void setNamedLines();
    void addTerminals();
    void initializeTerminals();

    void findFirstPlus();
    void processFirst(string N, rule object);
    void fillLessTable();
    void setLessMark(size_t leftIndex, vector<string> rightParts);

    void findLastPlus();
    void processLast(string N, rule object);
    void fillGreaterTable();
    void setGreaterMark(size_t rightIndex, vector<string> LeftParts);
    void operateFirstLast(vector<string> leftLast, vector<string> rightFirst);
    void additional();
public:
    PreGrammar();
    void creatingRelations();
    void runTable();
    vector<vector<string>> getRelationTable();
    vector<string> getTerminals();
    vector<pair<string, vector<rule>>> getGrammar();
};

#endif // PREGRAMMAR_H
