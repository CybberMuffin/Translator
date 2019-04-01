#include "codegenerator.h"

CodeGenerator::CodeGenerator()
{

}
QString CodeGenerator::run(string previousText, vector<lex_table> lexTable, vector<idn_table> idTable,
                        vector<con_table> conTable, vector<string> polizVec) {
    this->lexTable = lexTable;
    this->idnTable = idTable;
    this->conTable = conTable;

    finalText = previousText;
    poliz = polizVec;
    polizOriginal = polizVec;

    vector<string> arithm = { "+", "-", "*", "/"};
    size_t iterator = 0;
    while(iterator < poliz.size()) {
        if(poliz[iterator] == "@") {
            if(is_number(poliz[iterator])) {
                poliz.insert(poliz.begin() + iterator + 1, to_string(-to_double(poliz[iterator])));
                poliz.erase(poliz.begin() + iterator - 1, poliz.begin() + iterator);
            }
            else {
                size_t index = findInIdnTable(poliz[iterator - 1]);
                idnTable[index].value = -to_double(poliz[iterator - 1]);
            }
            iterator--;
        }
        else if(contains(poliz[iterator], arithm)) {
            double first = 0;
            double second = 0;
            if(is_number(poliz[iterator - 2]) && is_number(poliz[iterator - 1])) {
                first = to_double(poliz[iterator - 2]);
                second = to_double(poliz[iterator - 1]);
            } else if(is_number(poliz[iterator - 2]) && !is_number(poliz[iterator - 1])) {
                first = to_double(poliz[iterator - 2]);
                second = idnTable[findInIdnTable(poliz[iterator - 1])].value;
            } else if(!is_number(poliz[iterator - 2]) && is_number(poliz[iterator - 1])) {
                first = idnTable[findInIdnTable(poliz[iterator - 2])].value;
                second = to_double(poliz[iterator - 1]);
            } else if(!is_number(poliz[iterator - 2]) && !is_number(poliz[iterator - 1])) {
                first = idnTable[findInIdnTable(poliz[iterator - 2])].value;
                second = idnTable[findInIdnTable(poliz[iterator - 1])].value;
            }

            if(poliz[iterator] == "+")
                poliz.insert(poliz.begin() + iterator + 1, to_string(first + second));
            else if(poliz[iterator] == "-")
                poliz.insert(poliz.begin() + iterator + 1, to_string(first - second));
            else if(poliz[iterator] == "*")
                poliz.insert(poliz.begin() + iterator + 1, to_string(first * second));
            else if(poliz[iterator] == "/")
                poliz.insert(poliz.begin() + iterator + 1, to_string(first / second));

            poliz.erase(poliz.begin() + iterator - 2, poliz.begin() + iterator + 1);
            iterator = iterator - 3;
        }
        else if(poliz[iterator] == "=") {
            size_t index = findInIdnTable(poliz[iterator - 2]);
            if((idnTable[index].type == 1 && is_integer(poliz[iterator - 1])) || idnTable[index].type == 2)
               idnTable[index].value = to_double(poliz[iterator - 1]);
            else
               finalText += "ERROR ---> WRONG ASSIGNING TYPE FOR " + poliz[iterator - 2] + "\n";
        }
        else if(poliz[iterator] == "UPH") {
            bool isTrue;
            double first = 0;
            double second = 0;
            if(is_number(poliz[iterator - 4]) && is_number(poliz[iterator - 3])) {
                first = to_double(poliz[iterator - 4]);
                second = to_double(poliz[iterator - 3]);
            } else if(is_number(poliz[iterator - 4]) && !is_number(poliz[iterator - 3])) {
                first = to_double(poliz[iterator - 4]);
                second = idnTable[findInIdnTable(poliz[iterator - 3])].value;
            } else if(!is_number(poliz[iterator - 4]) && is_number(poliz[iterator - 3])) {
                first = idnTable[findInIdnTable(poliz[iterator - 4])].value;
                second = to_double(poliz[iterator - 3]);
            } else if(!is_number(poliz[iterator - 4]) && !is_number(poliz[iterator - 3])) {
                first = idnTable[findInIdnTable(poliz[iterator - 4])].value;
                second = idnTable[findInIdnTable(poliz[iterator - 3])].value;
            }

            if(poliz[iterator - 2] == ">") isTrue = first > second;
            else if(poliz[iterator - 2] == ">=") isTrue = first >= second;
            else if(poliz[iterator - 2] == "<") isTrue = first < second;
            else if(poliz[iterator - 2] == "<=") isTrue = first <= second;
            else if(poliz[iterator - 2] == "==") isTrue = first == second;

            if(!isTrue) {
                iterator = findIndexInPoliz(poliz[iterator - 1] + ":");
                poliz = polizOriginal;
            }
        }
        else if(poliz[iterator] == "BP") {
            iterator = findIndexInPoliz(poliz[iterator - 1] + ":");
            poliz = polizOriginal;
        }
        else if(poliz[iterator].front() == 'r') {
            if(findInIdnTable(poliz[iterator]) == -1)
                idnTable.emplace_back(2, poliz[iterator], NULL, NULL);
        }
        else if (poliz[iterator] == "<out>") {
            string value = "";
            if(is_integer(to_string(idnTable[findInIdnTable(poliz[iterator - 1])].value)))
                    value = to_string((int) idnTable[findInIdnTable(poliz[iterator - 1])].value);
             else value = to_string(idnTable[findInIdnTable(poliz[iterator - 1])].value);

            finalText += value + " ";
        }
        else if(poliz[iterator] == "<in>") {
            Dialog dialog(poliz[iterator - 1]);
            dialog.setModal(true);
            dialog.exec();

            string num = dialog.getText();
            size_t index = findInIdnTable(poliz[iterator - 1]);
            if(is_number(num) && ((idnTable[index].type == 1 && is_integer(poliz[iterator - 1])) || idnTable[index].type == 2))//notice
               idnTable[index].value = to_double(num);
            else
               finalText += "ERROR ---> WRONG ASSIGNING TYPE FOR " + poliz[iterator - 1] + "\n";
        }

        iterator++;
    }

    QString qText = QString::fromStdString(finalText);
    return qText;
}

bool CodeGenerator::is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it) || *it == '.' || *it == 'e' || *it == '-' || *it == '+')) {
            ++it;
    }
    return !s.empty() && it == s.end();
}

int CodeGenerator::findIndexInPoliz(const std::string& lexem) {
  auto it = std::find(polizOriginal.begin(), polizOriginal.end(), lexem);
  if (it != polizOriginal.end())
      return std::distance(polizOriginal.begin(), it);
  return -1;
}

int CodeGenerator::findInIdnTable(const string& lex) {
    auto it = find_if(idnTable.begin(), idnTable.end(),
                   [&lex](const idn_table& obj) {return obj.lexem == lex;});
    if(it != idnTable.end())
        return std::distance(idnTable.begin(), it);
    return -1;
}

bool CodeGenerator::is_nodot(const string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '.')
            return false;
    }
    return true;
}

bool CodeGenerator::contains(string str, vector<string> vecStr) {
    if (find(vecStr.begin(), vecStr.end(), str) != vecStr.end())
        return true;
    return false;
}

bool CodeGenerator::is_all_upper(const std::string& word)
{
    for(auto& c: word)
        if(!std::isupper(static_cast<unsigned char>(c)) && c != ':')
            return false;
    return true;
}

bool CodeGenerator::is_integer(const string& str) {
    bool dot = false;
    for (size_t i = 0; i < str.length(); i++) {
        if(dot && str[i] != '0')
            return false;
        if (str[i] == '.')
            dot = true;
    }
    return true;
}

double CodeGenerator::to_double(const std::string& symbol) {
    string::size_type sz;
    return stod(symbol, &sz);
}
