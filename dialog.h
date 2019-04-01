#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <iostream>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    std::string getText();
    explicit Dialog(std::string title, QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_okButton_clicked();

private:
    Ui::Dialog *ui;
    QString text;
};

#endif // DIALOG_H
