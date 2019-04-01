#ifndef RELATIONTABLE_H
#define RELATIONTABLE_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include "pregrammar.h"

namespace Ui {
class RelationTable;
}

class RelationTable : public QWidget
{
    Q_OBJECT

public:
    explicit RelationTable(QWidget *parent = nullptr);
    ~RelationTable();

    void fillTable(vector<vector<string>> relation);

private:
    Ui::RelationTable *ui;
    PreGrammar *table = new PreGrammar();
};

#endif // RELATIONTABLE_H
