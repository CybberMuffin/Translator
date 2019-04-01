#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillStatesTable(autoSyntax->getWays());
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->text->setText(QString());
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->text->setText(text);
    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName;
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->text->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->text->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->text->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->text->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->text->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->text->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->text->redo();
}

void MainWindow::on_actionRun_triggered()
{
    lex = new LexicalAnalyzer();
    syntax = new SyntacticalAnalyzer();
    autoSyntax = new AutoSyntacticalAnalyzer();
    preGrammar = new PreGrammar();
    preSyntax = new PregrammarSyntaxAnalyzer();
    codeGenerator = new CodeGenerator();
    on_actionSave_triggered();

    lex->selectFile(currentFile);
    lex->analyzer();

    fillLexTable(lex->get_lextable());

    ui->textBrowser->clear();
    //ui->textBrowser->setText(syntax->run(lex->get_lextable(), lex->get_contable(), lex->getError(), lex->run_table()));
    //ui->textBrowser->setText(autoSyntax->run(lex->get_lextable(), lex->getError()));
    preGrammar->creatingRelations();
    string action = preSyntax->run(preGrammar->getRelationTable(), lex->get_lextable(), preGrammar->getTerminals(),
                                              lex->getLexemsMap(), preGrammar->getGrammar());
    ui->textBrowser->setText(codeGenerator->run(action, lex->get_lextable(), lex->get_idntable(), lex->get_contable(),
                                                preSyntax->getPoliz()));

    fillConversionTable(autoSyntax->getConversion());
    fillAnalysisTable(preSyntax->getAnalysisTable());
    fillGrammarTable(preGrammar->getGrammar());
    fillPolizTable(preSyntax->getPolizTable());
}


void MainWindow::fillLexTable(vector<lex_table> table) {
    QStandardItemModel *model = new QStandardItemModel(table.size(), 3);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Row"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Lexem"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Idn"));

    for (int row = 0; row < table.size(); ++row) {
        QStandardItem *t_row = new QStandardItem(QString("%0").arg(table[row].row));
        QStandardItem *lexem = new QStandardItem(QString(QString::fromStdString(table[row].lexem)));
        QStandardItem *idn = new QStandardItem(QString("%0").arg(table[row].IDN));
        model->setItem(row, 0, t_row);
        model->setItem(row, 1, lexem);
        model->setItem(row, 2, idn);
    }

    ui->lexTable->setModel(model);
}

void MainWindow::fillConversionTable(vector<convers> conversion) {
    QStandardItemModel *model = new QStandardItemModel(conversion.size(), 3);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Lexem"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("State"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Stack"));

    for (int row = 0; row < conversion.size(); ++row) {
        QStandardItem *lexem =  new QStandardItem(QString(QString::fromStdString(conversion[row].lexem)));
        QStandardItem *state;
        QStandardItem *stack;
        if(conversion[row].state != -1)
            state = new QStandardItem(QString("%0").arg(conversion[row].state));
        else state = new QStandardItem(QString("%0").arg(conversion[row].stack));

        if(conversion[row].stack != 0)
            stack = new QStandardItem(QString("%0").arg(conversion[row].stack));
        else stack = new QStandardItem(QString("empty"));

        model->setItem(row, 0, lexem);
        model->setItem(row, 1, state);
        model->setItem(row, 2, stack);
    }

    ui->conversionTable->setModel(model);
}

void MainWindow::fillStatesTable(vector<autom> states) {
    QStandardItemModel *model = new QStandardItemModel(states.size(), 4);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Alpha"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Lexem"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Beta"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Stack"));

    for (int row = 0; row < states.size(); ++row) {
        QStandardItem *alpha = new QStandardItem(QString("%0").arg(states[row].alpha));
        QStandardItem *label = new QStandardItem(QString("%0").arg(states[row].label));
        QStandardItem *beta;
        if(states[row].beta != -1)
            beta = new QStandardItem(QString("%0").arg(states[row].beta));
        else beta = new QStandardItem(QString("exit"));
        QStandardItem *stack = new QStandardItem(QString("%0").arg(states[row].stack));
        model->setItem(row, 0, alpha);
        model->setItem(row, 1, label);
        model->setItem(row, 2, beta);
        model->setItem(row, 3, stack);
    }

    ui->statesTable->setModel(model);
    ui->statesTable->setColumnWidth(0, 70);
    ui->statesTable->setColumnWidth(2, 70);

}

void MainWindow::fillAnalysisTable(vector<analys> analysis) {
    QStandardItemModel *model = new QStandardItemModel(analysis.size(), 4);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Stack"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Relation"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Lexems"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Poliz"));

    for (size_t row = 0; row < analysis.size(); ++row) {
        QStandardItem *stack =  new QStandardItem(QString(QString::fromStdString(analysis[row].stack)));
        QStandardItem *relation =  new QStandardItem(QString(QString::fromStdString(analysis[row].relation)));
        QStandardItem *lexems =  new QStandardItem(QString(QString::fromStdString(analysis[row].table)));
        QStandardItem *poliz =  new QStandardItem(QString(QString::fromStdString(analysis[row].poliz)));

        model->setItem(row, 0, stack);
        model->setItem(row, 1, relation);
        model->setItem(row, 2, lexems);
        model->setItem(row, 3, poliz);
    }

    ui->analysisTable->setModel(model);
    ui->analysisTable->setColumnWidth(0, 140);
    ui->analysisTable->setColumnWidth(1, 60);
    ui->analysisTable->setColumnWidth(2, 100);
    ui->analysisTable->setColumnWidth(3, 100);
}

void MainWindow::fillGrammarTable(vector<pair<string, vector<rule>>> grammar) {
    QStandardItemModel *model = new QStandardItemModel(grammar.size(), 2);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Left part"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Right part"));

    for (size_t row = 0; row < grammar.size(); ++row) {
        QStandardItem *left =  new QStandardItem(QString(QString::fromStdString(grammar[row].first)));
        string rightString = "";
        for(auto rightPart : grammar[row].second) {
            for(auto lex : rightPart.element) {
                rightString += lex;
            }
            rightString +=  " | ";
        }

        QStandardItem *right =  new QStandardItem(QString(QString::fromStdString(rightString)));

        model->setItem(row, 0, left);
        model->setItem(row, 1, right);
    }

    ui->grammarTable->setModel(model);
    ui->grammarTable->setColumnWidth(0, 100);
    ui->grammarTable->setColumnWidth(1, 320);
}

void MainWindow::fillPolizTable(vector<polizTable> polizTab) {
    QStandardItemModel *model = new QStandardItemModel(polizTab.size(), 5);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("In"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Stack"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Out"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("OC"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ZPC"));

    for (size_t row = 0; row < polizTab.size(); ++row) {
        QStandardItem *in =  new QStandardItem(QString(QString::fromStdString(polizTab[row].in)));
        QStandardItem *stack =  new QStandardItem(QString(QString::fromStdString(polizTab[row].stack)));
        QStandardItem *out =  new QStandardItem(QString(QString::fromStdString(polizTab[row].out)));
        QStandardItem *OC =  new QStandardItem(QString(QString::fromStdString(polizTab[row].OC)));
        QStandardItem *ZPC =  new QStandardItem(QString(QString::fromStdString(polizTab[row].ZPC)));

        model->setItem(row, 0, in);
        model->setItem(row, 1, stack);
        model->setItem(row, 2, out);
        model->setItem(row, 3, OC);
        model->setItem(row, 4, ZPC);
    }

    ui->polizTable->setModel(model);
    ui->polizTable->setColumnWidth(0, 70);
    ui->polizTable->setColumnWidth(3, 30);
    ui->polizTable->setColumnWidth(4, 40);
}

void MainWindow::on_actionRun_Relation_Table_triggered()
{

}
