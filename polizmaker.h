#ifndef POLIZMAKER_H
#define POLIZMAKER_H
#include "lexicalanalyzer.h"
#include "pregrammar.h"
#include <algorithm>
#include <sstream>


struct polizTable{
    string in;
    string stack;
    string out;
    string OC;
    string ZPC;

    polizTable(string in, string stack, string out, string OC = "", string ZPC = "") {
        this->in = in;
        this->stack = stack;
        this->out = out;
        this->OC = OC;
        this->ZPC = ZPC;
    }
};

class PolizMaker
{
private:
    vector<string> poliz;
    vector<string> store;
    vector<string> idns;

    vector<string> stack;
    vector<string> labels;
    vector<int> endLoopMarks;
    vector<polizTable> poliztab;
    int label = 0;
    int workSec = 0;
    string iterationVar = "";

    void afterToInstruction();
    void afterByInstruction();
public:
    PolizMaker();
    void arithemticPoliz(vector<string> basis);
    void logicalExpression(string relation);
    void endLogExp();
    void endCond();
    void endLoop();

    void jump();
    void inOutput(string put);

    void assign(vector<string> basis);
    double calculatePoliz();
    string showCalculation();
    string reduceIntoString(vector<string> some);
    bool is_number(const std::string& s);
    bool contains(string str, vector<string> vecStr);
    bool is_all_upper(const std::string& word);

    void addIdns(string idn);
    void addToStack(string lexem);
    void addLabel();
    void addToPolizTab(string lex);
    void makeTable();

    vector<string> getStack();
    vector<string> getPoliz();
    vector<string> getIdns();
    vector<polizTable> getTable();
};

#endif // POLIZMAKER_H
