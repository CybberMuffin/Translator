#include "relationtable.h"
#include "ui_relationtable.h"

RelationTable::RelationTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelationTable)
{
    ui->setupUi(this);
    table->creatingRelations();
    fillTable(table->getRelationTable());
}


void RelationTable::fillTable(vector<vector<string>> relation){
    QStandardItemModel *model = new QStandardItemModel(relation.size(), relation.size());

//    for(size_t i = 0; i < relation.size(); i++){
//        model->setHeaderData(i, Qt::Horizontal, QObject::tr(QString::fromStdString(relation[0][i])));
//        model->setHeaderData(i, Qt::Vertical, QObject::tr(relation[0][i]));
//    }

    for (int row = 0; row < relation.size(); ++row) {
        for(int col = 0; col < relation[row].size(); ++col){
            model->setItem(row, col, new QStandardItem(QString(QString::fromStdString(relation[row][col]))));
        }
    }

    ui->tableView->setModel(model);
}

RelationTable::~RelationTable()
{
    delete ui;
}
