#include "autosyntacticalanalyzer.h"

AutoSyntacticalAnalyzer::AutoSyntacticalAnalyzer(){
    this->index = 0;

    ////declaration
    this->ways.emplace_back(1, 2, 1);
    this->ways.emplace_back(1, 2, 2);
    this->ways.emplace_back(2, 3, 100);
    this->ways.emplace_back(3, 2, 14);
    this->ways.emplace_back(3, 1, 13);
    this->ways.emplace_back(3, 4, 11);
    this->ways.emplace_back(4, 10, 13, 5);
    this->ways.emplace_back(5, 6, 12);
    this->ways.emplace_back(6, -1);//exit
    //// end of declaration

    ////list of operations
    this->ways.emplace_back(10, 11, 102);
    this->ways.emplace_back(10, 20, -1, 12);
    this->ways.emplace_back(11, 20, 31, 12);
    this->ways.emplace_back(12, 13, 13);
    this->ways.emplace_back(13, 14, 102);
    //extra exits
    this->ways.emplace_back(13, -1, 12);//exit
    this->ways.emplace_back(13, -1, 7);
    //end
    this->ways.emplace_back(13, 20, -1, 12);//lex -1 should be the last one from the same state
    this->ways.emplace_back(14, 20, 31, 12);
    //// end of list

    ////operations
    this->ways.emplace_back(20, 21, 100);
    this->ways.emplace_back(20, 23, 9);
    this->ways.emplace_back(20, 26, 10);
    this->ways.emplace_back(20, 29, 3);
    this->ways.emplace_back(20, 39, 8);
    this->ways.emplace_back(20, 45, 30);
    //assigning
    this->ways.emplace_back(21, 50, 17, 22);
    this->ways.emplace_back(22, -1);//exit
    //input
    this->ways.emplace_back(23, 24, 15);
    this->ways.emplace_back(24, 25, 100);
    this->ways.emplace_back(25, 24, 15);
    this->ways.emplace_back(25, -1);//exit
    //output
    this->ways.emplace_back(26, 27, 16);
    this->ways.emplace_back(27, 28, 100);
    this->ways.emplace_back(28, 27, 16);
    this->ways.emplace_back(28, -1);//exit
    //loop
    this->ways.emplace_back(29, 30, 100);
    this->ways.emplace_back(30, 50, 17, 31);
    this->ways.emplace_back(31, 50, 4, 32);
    this->ways.emplace_back(32, 50, 5, 33);
    this->ways.emplace_back(33, 34, 6);
    this->ways.emplace_back(34, 60, 28, 35);
    this->ways.emplace_back(35, 36, 29);
    this->ways.emplace_back(36, 10, 13, 37);
    this->ways.emplace_back(37, 38, 7);
    this->ways.emplace_back(38, -1);//exit
    //condition operator
    this->ways.emplace_back(39, 60, 28, 40);
    this->ways.emplace_back(40, 41, 29);
    this->ways.emplace_back(41, 42, 11);
    this->ways.emplace_back(42, 10, 13, 43);
    this->ways.emplace_back(43, 44, 12);
    this->ways.emplace_back(44, -1);//exit

    //goto label
    this->ways.emplace_back(45, 46, 102);
    this->ways.emplace_back(46, -1);//exit

    /////expression
    this->ways.emplace_back(50, 50, 25);
    this->ways.emplace_back(50, 51, 100);
    this->ways.emplace_back(50, 51, 101);
    this->ways.emplace_back(50, 50, 28, 52);
    this->ways.emplace_back(51, 50, 24);
    this->ways.emplace_back(51, 50, 25);
    this->ways.emplace_back(51, 50, 26);
    this->ways.emplace_back(51, 50, 27);
    this->ways.emplace_back(51, -1); //exit
    this->ways.emplace_back(52, 51, 29);

    ////relation
    this->ways.emplace_back(60, 50, -1, 61);
    this->ways.emplace_back(61, 50, 20, 62);
    this->ways.emplace_back(61, 50, 18, 62);
    this->ways.emplace_back(61, 50, 19, 62);
    this->ways.emplace_back(61, 50, 21, 62);
    this->ways.emplace_back(61, 50, 22, 62);
    this->ways.emplace_back(61, 50, 23, 62);
    this->ways.emplace_back(62, -1);//exit

}

QString AutoSyntacticalAnalyzer::run(vector<lex_table> lexems, string lexError) {
    this->table = lexems;
    string text = "Program is running";
    int state = 1;
    while(index < this->table.size()){
        if(state != -10) {
            if(state == -3)
                break;
            if(!main_stack.empty())
                conversion.emplace_back(table[index].lexem, state, main_stack.back());
            else conversion.emplace_back(table[index].lexem,state, 0);
            state = getNextState(state, this->table[index].IDN);
        }
        else {
            text = "Error at line: " + to_string(this->table[index].row) + ", at " + this->table[index].lexem + "\n\n";
            text += lexError;
            break;
        }
    }

    QString qText = QString::fromStdString(text);
    return qText;
}

int AutoSyntacticalAnalyzer::getNextState(int state, int label) {
    for (auto &way : this->ways) {
        if(way.alpha == state && (way.label == label || way.label == -1)){
            if(way.stack != 0){
                this->main_stack.push_back(way.stack);
            }

            if(way.beta != -1) {
                if(way.label != -1)
                    index++;
                return way.beta;
            }
            else {
                if (!this->main_stack.empty()) {
                    int up = this->main_stack.back();
                    this->main_stack.pop_back();
                    return up;
                }
                else{
                    return -3;
                }
            }
        }
    }
    return -10;
}

vector<autom> AutoSyntacticalAnalyzer::getWays(){
    return ways;
}

vector<convers> AutoSyntacticalAnalyzer::getConversion(){
    return conversion;
}
