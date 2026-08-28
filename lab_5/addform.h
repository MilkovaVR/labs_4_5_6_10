#ifndef ADDFORM_H
#define ADDFORM_H

#include <QDialog>
#include "person.h"

namespace Ui {
class AddForm;
}

#include "ui_addform.h"

class AddForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddForm(QWidget *parent = nullptr);
    ~AddForm();

    void setPersonData(Person* p);

private slots:
    void on_btn_Cancel_clicked();
    void on_btn_Print_clicked();

private:
    Ui::AddForm *ui;
    Person* currentPerson;
};

#endif // ADDFORM_H