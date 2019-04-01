#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include "lexicalanalyzer.h"
#include "syntacticalanalyzer.h"
#include "autosyntacticalanalyzer.h"
#include "relationtable.h"
#include "pregrammarsyntaxanalyzer.h"
#include "codegenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionRun_triggered();

    void on_actionSave_triggered();

    void fillLexTable(vector<lex_table> table);

    void fillStatesTable(vector<autom> states);

    void fillConversionTable(vector<convers> conversion);

    void fillAnalysisTable(vector<analys> analysis);

    void fillGrammarTable(vector<pair<string, vector<rule>>> grammar);

    void fillPolizTable(vector<polizTable> polizTab);

    void on_actionRun_Relation_Table_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    LexicalAnalyzer *lex = new LexicalAnalyzer();
    SyntacticalAnalyzer *syntax = new SyntacticalAnalyzer();
    AutoSyntacticalAnalyzer *autoSyntax = new AutoSyntacticalAnalyzer();
    PregrammarSyntaxAnalyzer *preSyntax = new PregrammarSyntaxAnalyzer();
    PreGrammar *preGrammar = new PreGrammar();
    CodeGenerator *codeGenerator = new CodeGenerator();
};

#endif // MAINWINDOW_H
