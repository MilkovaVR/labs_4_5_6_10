#include "addform.h"

AddForm::AddForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddForm)
{
    ui->setupUi(this);
}

AddForm::~AddForm()
{
    delete ui;
}

void AddForm::setPersonData(Person* p)
{
    currentPerson = p;
    ui->label_Info->setText(p->Craft());

    ui->label_Photo->setPixmap(QPixmap("/Users/vasilinamilkova/lab5_New/photo_1.jpeg").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void AddForm::on_btn_Cancel_clicked()
{
    reject();
}

void AddForm::on_btn_Print_clicked()
{
    done(1);
}