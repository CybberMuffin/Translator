#include "syntacticalanalyzer.h"

SyntacticalAnalyzer::SyntacticalAnalyzer()
{
    this->i = 0;
}

bool SyntacticalAnalyzer::prog() {
    if (spOg()) {
        if (table[i].IDN == 11) {
            i++;
            if (table[i].IDN == 13) {
                i++;
                if (spOp()) {
                    if (table[i].IDN == 12) {
                        i++;
                        return true;
                    }
                    else {
                        error("There is no closing tag '}' at row " + to_string(table[i].row));
                        return false;
                    }
                }
                else {
                    error("Error in operation list at row " + to_string(table[i].row));
                    return false;
                }
            }
            else {
                error("Missing <Enter> after opening tag '{' at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("There is no opening tag '{' at row " + to_string(table[i].row) + " " + to_string(i));
            return false;
        }
    }
    else {
        error("Error in declaration list at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::_type() {
    if (table[i].IDN == 1) {
        i++;
        return true;
    }
    else {
        if (table[i].IDN == 2) {
            i++;
            return true;
        }
        else {
            error("Undefined type at row " + to_string(table[i].row));
            return false;
        }
    }
}

bool SyntacticalAnalyzer::spOg() {
    if (og()) {
        while (table.size() > i && table[i].IDN == 13) {
            i++;
            if (og()) {
                continue;
            }
            else {
                error("Missing declaration at row " + to_string(table[i].row));
                return false;
            }
        }
        return true;
    }
    else {
        error("Missing first declaration at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::spOp() {
    if (op()) {
        if (table[i].IDN == 13) {
            i++;
            while (table.size() > i && table[i].IDN != 12 && table[i].IDN != 7) {
                if (op()) {
                    if (table[i].IDN == 13) {
                        i++;
                        continue;
                    }
                    else {
                        error("Missing <Enter> at row " + to_string(table[i].row));
                        return false;
                    }
                }
                else if (table[i].IDN == 102) {
                    i++;
                    if (table[i].IDN == 31) {
                        i++;
                        continue;
                    }
                    else {
                        error("Missing ':' at row " + to_string(table[i].row));
                        return false;
                    }
                }
                else {
                    error("Missing operation or end of the list at row " + to_string(table[i].row));
                    return false;
                }
            }
            return true;
        }
        else {
            error("Missing <Enter> after first operation at row " + to_string(table[i].row));
            return false;
        }
    }
    else if (table[i].IDN == 102) {
        i++;
        if (table[i].IDN == 31) {
            i++;
            while (table.size() > i && table[i].IDN != 12 && table[i].IDN != 7) {
                if (op()) {
                    if (table[i].IDN == 13) {
                        i++;
                        continue;
                    }
                    else {
                        error("Missing <Enter> at row " + to_string(table[i].row));
                        return false;
                    }
                }
                else if (table[i].IDN == 102) {
                    i++;
                    if (table[i].IDN == 31) {
                        i++;
                        continue;
                    }
                    else {
                        error("Missing ':' at row " + to_string(table[i].row));
                        return false;
                    }
                }
                else {
                    error("Missing operation or end of the list at row " + to_string(table[i].row));
                    return false;
                }
            }
            return true;
        }
        else {
            error("':' expected after Label at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        error("Missing first operation at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::og() {
    if (_type()) {
        if (table[i].IDN == 100) {
            i++;
            while (table.size() > i && table[i].IDN == 14) {
                i++;
                if (table[i].IDN == 100) {
                    i++;
                    continue;
                }
                else {
                    error("Missing variable after coma at row " + to_string(table[i].row));
                    return false;
                }
            }
            return true;
        }
        else {
            error("Missing first variable at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        error("Missing type of variable(s) at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::op() {
    return assign() || into() || outto() || loop() || condition_go() || gotoLabel();
}

bool SyntacticalAnalyzer::assign() {
    if (table[i].IDN == 100) {
        i++;
        if (table[i].IDN == 17) {
            i++;
            if (E()) {
                return true;
            }
            else {
                error("Missing expresion after '=' at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("Missing '=' after variable at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        //error("Missing variable in assignment at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::into() {
    if (table[i].IDN == 9) {
        i++;
        if (table[i].IDN == 15) {
            i++;
            if (table[i].IDN == 100) {
                i++;
                while (table[i].IDN == 15) {
                    i++;
                    if (table[i].IDN == 100) {
                        i++;
                        continue;
                        //return true;
                    }
                    else {
                        error("Missing variable after '<<' at row " + to_string(table[i].row));
                        return false;
                    }
                }
                return true;
            }
            else {
                error("Missing variable after '<<' at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("'<<' expected after 'cin' at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        return false;
    }
}

bool SyntacticalAnalyzer::outto() {
    if (table[i].IDN == 10) {
        i++;
        if (table[i].IDN == 16) {
            i++;
            if (table[i].IDN == 100) {
                i++;
                while (table[i].IDN == 16) {
                    i++;
                    if (table[i].IDN == 100) {
                        i++;
                        continue;
                        //return true;
                    }
                    else {
                        error("Missing variable after '>>' at row " + to_string(table[i].row));
                        return false;
                    }
                }
                return true;
            }
            else {
                error("Missing variable after '>>' at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("'>>' expected after 'cout' at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        return false;
    }
}

bool SyntacticalAnalyzer::loop() {
    if (table[i].IDN == 3) {
        i++;
        if (table[i].IDN == 100) {
            i++;
            if (table[i].IDN == 17) {
                i++;
                if (E()) {
                    if (table[i].IDN == 4) {
                        i++;
                        if (E()) {
                            if (table[i].IDN == 5) {
                                i++;
                                if (E()) {
                                    if (table[i].IDN == 6) {
                                        i++;
                                        if (table[i].IDN == 28) {
                                            i++;
                                            if (relation()) {
                                                if (table[i].IDN == 29) {
                                                    i++;
                                                    if (table[i].IDN == 13) {
                                                        i++;
                                                        if (spOp()) {
                                                            if (table[i].IDN == 7) {
                                                                i++;
                                                                return true;
                                                            }
                                                            else {
                                                                error("Missing closing tag 'rof' for loop at row " + to_string(table[i].row));
                                                                return false;
                                                            }
                                                        }
                                                        else {
                                                            error("Missing operation list in loop body at row " + to_string(table[i].row));
                                                            return false;
                                                        }
                                                    }
                                                    else {
                                                        error("Missing <Enter> after loop declaration at row " + to_string(table[i].row));
                                                        return false;
                                                    }
                                                }
                                                else {
                                                    error("Missing closing tag ')' after relation in while body at row " + to_string(table[i].row));
                                                    return false;
                                                }
                                            }
                                            else {
                                                error("Missing relation in while body at row " + to_string(table[i].row));
                                                return false;
                                            }
                                        }
                                        else {
                                            error("Missing opening tag '(' after 'while' at row " + to_string(table[i].row));
                                            return false;
                                        }
                                    }
                                    else {
                                        error("Missing 'while' in loop declaration at row " + to_string(table[i].row));
                                        return false;
                                    }
                                }
                                else {
                                    error("Missing expresion after 'by' at row " + to_string(table[i].row));
                                    return false;
                                }
                            }
                            else {
                                error("Missig 'by' in loop declaration at row " + to_string(table[i].row));
                                return false;
                            }
                        }
                        else {
                            error("Missing expresion after 'to' at row " + to_string(table[i].row));
                            return false;
                        }
                    }
                    else {
                        error("Missig 'to' in loop declaration at row " + to_string(table[i].row));
                        return false;
                    }
                }
                else {
                    error("Missing expresion after '=' at row " + to_string(table[i].row));
                    return false;
                }
            }
            else {
                error("Missing '=' after variable in loop declaration at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("Missing variable after 'for' at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        return false;
    }
}

bool SyntacticalAnalyzer::condition_go() {
    if (table[i].IDN == 8) {
        i++;
        if (table[i].IDN == 28) {
            i++;
            if (relation()) {
                if (table[i].IDN == 29) {
                    i++;
                    if (table[i].IDN == 11) {
                        i++;
                        if (table[i].IDN == 13) {
                            i++;
                            if (spOp()) {
                                if (table[i].IDN == 12) {
                                    i++;
                                    return true;
                                }
                                else {
                                    error("Missing closing tag '}' at row " + to_string(table[i].row));
                                    return false;
                                }
                            }
                            else {
                                error("Missing operation list at row " + to_string(table[i].row));
                                return false;
                            }
                        }
                        else {
                            error("Missing <Enter> after opening tag '{' at row " + to_string(table[i].row));
                            return false;
                        }
                    }
                    else {
                        error("Missing opening tag '{' at row " + to_string(table[i].row));
                        return false;
                    }
                }
                else {
                    error("Missing closing tag ')' at row " + to_string(table[i].row));
                    return false;
                }
            }
            else {
                error("Missing relation in if body at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("Missing opening tag '(' at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        return false;
    }
}

bool SyntacticalAnalyzer::gotoLabel() {
    if (table[i].IDN == 30) {
        i++;
        if (table[i].IDN == 102) {
            i++;
            return true;
        }
        else {
            error("Missing label after 'goto' at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        return false;
    }
}

bool SyntacticalAnalyzer::E() {
    if (T()) {
        while (table[i].IDN == 26 || table[i].IDN == 27) {
            i++;
            if (T()) {
                continue;
            }
            else {
                error("Missing expresion after sign at row " + to_string(table[i].row));
                return false;
            }
        }
        return true;
    }
    else {
        error("Missing expresion at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::T() {
    if (F()) {
        while (table[i].IDN == 24 || table[i].IDN == 25) {
            i++;
            if (F()) {
                continue;
            }
            else {
                error("Missing expression after sign at row " + to_string(table[i].row));
                return false;
            }
        }
        return true;
    }
    else {
        error("Missing expresion at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::F() {
    if (table[i].IDN == 100 || table[i].IDN == 101) {
        i++;
        return true;
    }
    else if (table[i].IDN == 28) {
        i++;
        if (E()) {
            if (table[i].IDN == 29) {
                i++;
                return true;
            }
            else {
                error("Missing closing tag ')' at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("Missing expression at row " + to_string(table[i].row));
            return false;
        }
    }
    else if (table[i].IDN == 25) {
        i++;
        if (F()) {
            return true;
        }
        else {
            error("Missing expresion after '-' at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        error("Missing expresion at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::relation() {
    if (E()) {
        if (z()) {
            if (E()) {
                return true;
            }
            else {
                error("Missing second expresion after relation sign at row " + to_string(table[i].row));
                return false;
            }
        }
        else {
            error("Missing relation sign after expresion at row " + to_string(table[i].row));
            return false;
        }
    }
    else {
        error("Missing expresion at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::z() {
    if (table[i].IDN >= 18 || table[i].IDN <= 23) {
        i++;
        return true;
    }
    else {
        error("Incorrect relation sign at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::high() {
    if (table[i].IDN == 26 || table[i].IDN == 27) {
        i++;
        return true;
    }
    else {
        error("Incorrect arithmetic sign at row " + to_string(table[i].row));
        return false;
    }
}

bool SyntacticalAnalyzer::low() {
    if (table[i].IDN == 24 || table[i].IDN == 25) {
        i++;
        return true;
    }
    else {
        error("Incorrect arithmetic sign at row " + to_string(table[i].row));
        return false;
    }
}


void SyntacticalAnalyzer::error(string str) {
    errors.push_back(str + '\n' + '\n');
    //cerr << str << "\n\n";
    //system("pause");
}

QString SyntacticalAnalyzer::run(vector<lex_table> lexems, vector<con_table> con, string lexError, string table) {
    this->table = lexems;
    this->conTable = con;
    string text = "";
    text += table;
    if (prog()) {
        text += "Program is running\n";
    }
    else {
        text += lexError;
        for (size_t i = 0; i < this->errors.size(); i++) {
            text += this->errors[i];
        }
    }

    QString qText = QString::fromStdString(text);
    return qText;
}
