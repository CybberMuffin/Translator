#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(std::string title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Enter value for variable " + QString(QString::fromStdString(title)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_okButton_clicked()
{
    this->text = ui->cinText->toPlainText();
    close();
}

std::string Dialog::getText() {
   return text.toUtf8().constData();
}
