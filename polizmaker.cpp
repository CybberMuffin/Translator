#include "polizmaker.h"

PolizMaker::PolizMaker()
{

}
void PolizMaker::logicalExpression(string relation) {
    if(!stack.empty() && stack.back() == "while") {
        --label;
        stack.push_back(relation);
        while(!idns.empty()) {
            reverse(idns.begin(), idns.end());
            poliz.push_back(idns.back());
            idns.pop_back();
            reverse(idns.begin(), idns.end());
        };
    }
    else stack.push_back(relation);

    poliztab.emplace_back(relation, poliztab.back().stack + " " + relation, "");
}

void PolizMaker::endLogExp() {
    poliz.push_back(stack.back());
    stack.pop_back();
    labels.push_back("m" + to_string(++label));
    poliz.push_back(labels.back());
    stack.push_back(poliz.back());
    poliz.push_back("UPH");

    poliztab.back().stack.pop_back();
    poliztab.emplace_back(")", poliztab.back().stack, poliz[poliz.size() - 3] + " "
            + poliz[poliz.size() - 2] + " " + poliz.back());
}

void PolizMaker::endCond() {
    poliz.push_back(labels.back() + ":");
    if(!labels.empty()) labels.pop_back();
    stack.clear(); //consider to change
}

void PolizMaker::endLoop() {
    poliz.push_back("m" + to_string(endLoopMarks.back() - 2));
    poliz.push_back("BP");
    poliz.push_back("m" + to_string(endLoopMarks.back()) + ":");
    endLoopMarks.pop_back();
}

void PolizMaker::jump() {
    poliz.push_back(idns.back());
    poliz.push_back("BP");
    idns.pop_back();
}

void PolizMaker::inOutput(string put) {
     poliz.push_back(idns.back());
     put == "<output>" ?  poliz.push_back("<out>") : poliz.push_back("<in>");
     idns.pop_back();
}

void PolizMaker::assign(vector<string> basis) {
    poliz.push_back(basis[1]);
}

void PolizMaker::arithemticPoliz(vector<string> basis) {
    if(basis[0] == "IDN" || basis[0] == "CON") {
        if(!stack.empty() && stack.back() == "to") afterToInstruction();
        else if(!stack.empty() && stack.back() == "by") afterByInstruction();
        else {
            while(!idns.empty()) {
                reverse(idns.begin(), idns.end());
                poliz.push_back(idns.back());
                idns.pop_back();
                reverse(idns.begin(), idns.end());
            }
        }
    }


    if(!poliztab.empty() && basis.size() > 1) {
        vector<string> arithm = { "+", "-", "*", "/", "=" };
        if(contains(basis[1], arithm))
                 poliztab.emplace_back(basis[1], poliztab.back().stack, basis[1]);
        else if(basis[0] == "-") poliztab.emplace_back(basis[0], poliztab.back().stack, basis[0]);
    }

    if(contains("-", basis)) {
        if(basis[0] == "-") poliz.push_back("@");
        else if(basis[1] == "-") poliz.push_back("-");
    }
    else if(contains("+", basis)) {
        poliz.push_back("+");
    }
    else if(contains("*", basis)) {
        poliz.push_back("*");
    }
    else if(contains("/", basis)) {
        poliz.push_back("/");
    }
    if(contains("=", basis)) {       
        if(!idns.empty()) {
            poliz.push_back(idns.back());   
            idns.pop_back();
        }
        string check = poliz.back();
        poliz.push_back("=");
        if(!stack.empty()) {
            if(stack.back() == "for") {
                poliz.push_back("r" + to_string(++workSec));
                poliz.push_back("1");
                poliz.push_back("=");

                labels.push_back("m" + to_string(++label));
                poliz.push_back(labels.back() + ":");
            }
        }
    }

}

void PolizMaker::afterToInstruction() {
    poliz.push_back("r" + to_string(++workSec + 1));
    poliz.push_back(idns.back());
    string one = idns.back();
    idns.pop_back();
    poliz.push_back("=");

    poliztab.emplace_back(one,
                          poliztab.back().stack, poliz[poliz.size() - 3] +
                            " " + poliz[poliz.size() - 2] + " " + poliz.back(),
            to_string(0), iterationVar);
}

void PolizMaker::afterByInstruction() {
    poliz.push_back("r" + to_string(++workSec - 1));
    poliz.push_back(idns.back());
    string some = idns.back();
    idns.pop_back();
    poliz.push_back("=");

    poliz.push_back("r" + to_string(workSec - 2));
    poliz.push_back("0");
    poliz.push_back("==");
    labels.push_back("m" + to_string(++label));
    poliz.push_back(labels.back());
    poliz.push_back("UPH");


    poliz.push_back(iterationVar);
    poliz.push_back(iterationVar);
    poliz.push_back("r" + to_string(workSec - 1));
    poliz.push_back("+");
    poliz.push_back("=");
    endCond();

    poliz.push_back("r" + to_string(workSec - 2));
    poliz.push_back("0");
    poliz.push_back("=");

    poliz.push_back(iterationVar);
    poliz.push_back("r" + to_string(workSec));
    poliz.push_back("-");
    poliz.push_back("r" + to_string(workSec - 1));
    poliz.push_back("*");
    poliz.push_back("0");
    poliz.push_back("<=");
    labels.push_back("m" + to_string(++label));
    poliz.push_back(labels.back());
    poliz.push_back("UPH");

    endLoopMarks.push_back(label);

    string one = "";
    for(size_t i = poliz.size() - 26; i < poliz.size(); i++) {
        one += " " + poliz[i];
    }

    poliztab.emplace_back(some, poliztab.back().stack, one, to_string(0), iterationVar);

    iterationVar = "";
    stack.clear();
}

double PolizMaker::calculatePoliz() {
    reverse(poliz.begin(), poliz.end());
    vector<double> numbers;
    double finalNumber;
    while(!poliz.empty()) {
        if(is_number(poliz.back())) {
            string::size_type sz;
            double symbol = stod(poliz.back(), &sz);
            numbers.push_back(symbol);
        }
        else if(poliz.back() == "@") {
            finalNumber = -numbers.back();
            numbers.pop_back();
            numbers.push_back(finalNumber);
        }
        else if(poliz.back() == "+") {
                finalNumber = numbers.back();
                numbers.pop_back();
                finalNumber = finalNumber + numbers.back();
                numbers.pop_back();
                numbers.push_back(finalNumber);
        }
        else if(poliz.back() == "-") {
            finalNumber = numbers.back();
            numbers.pop_back();
            finalNumber = numbers.back() - finalNumber;
            numbers.pop_back();
            numbers.push_back(finalNumber);
        }
        else if(poliz.back() == "*") {
            finalNumber = numbers.back();
            numbers.pop_back();
            finalNumber = finalNumber * numbers.back();
            numbers.pop_back();
            numbers.push_back(finalNumber);
        }
        else if(poliz.back() == "/") {
            finalNumber = numbers.back();
            numbers.pop_back();
            finalNumber = numbers.back() / finalNumber;
            numbers.pop_back();
            numbers.push_back(finalNumber);
        }
           poliz.pop_back();
    }

    return finalNumber;
}

string PolizMaker::showCalculation() {
    std::ostringstream strs;
    strs << this->calculatePoliz();
    std::string str = strs.str();

    return str;
}

string PolizMaker::reduceIntoString(vector<string> some){
    string total = "";
    for(auto it : some) total += " " + it;
    return total;
}

bool PolizMaker::is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool PolizMaker::is_all_upper(const std::string& word)
{
    for(auto& c: word)
        if(!std::isupper(static_cast<unsigned char>(c)))
            return false;
    return true;
}

bool PolizMaker::contains(string str, vector<string> vecStr) {
    if (find(vecStr.begin(), vecStr.end(), str) != vecStr.end())
        return true;
    return false;
}

void PolizMaker::addIdns(string idn) {
    this->idns.push_back(idn);
    if(!stack.empty()) {
        if(stack.back() == "for" && iterationVar == "") {
            iterationVar = idns.back();
            poliztab.emplace_back(iterationVar, poliztab.back().stack, iterationVar, to_string(1), iterationVar);
        } else {
            if(stack.back() != "to" && stack.back() != "by")
                poliztab.emplace_back(idn, poliztab.back().stack, idn);
        }
    } else {       
        if(is_all_upper(idn)) poliztab.emplace_back(idn + ":", "", idn + ":");
        else poliztab.emplace_back(idn, "", idn);
    }
}


void PolizMaker::addToStack(string lexem) {
    this->stack.push_back(lexem);
    if(lexem == "for") {        
        poliztab.emplace_back(lexem, "for m" + to_string(label + 1) + " "
                              + "m" + to_string(label + 2) + " "
                              "m" + to_string(label + 3), "", to_string(1));
    } else {
        poliztab.emplace_back(lexem, lexem, "");
    }
}

void PolizMaker::addLabel() {
    poliz.push_back(idns.back() + ":");
    idns.pop_back();
}

void PolizMaker::addToPolizTab(string lex) {
    poliztab.emplace_back(lex,
            !poliztab.empty() ? poliztab.back().stack : "", lex);
}

void PolizMaker::makeTable() {

}

vector<string> PolizMaker::getStack() {
    return this->stack;
}

vector<string> PolizMaker::getPoliz() {
    return this->poliz;
}

vector<string> PolizMaker::getIdns() {
    return this->idns;
}

vector<polizTable> PolizMaker::getTable() {
    return this->poliztab;
}

