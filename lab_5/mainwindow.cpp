#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addform.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"ФИО", "Дата рождения", "Тип"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    for(Person* p : people) delete p;
    delete ui;
}

void MainWindow::on_btnLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл с данными", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл!");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Encoding::Utf8);
    in.setAutoDetectUnicode(true);

    ui->tableWidget->setRowCount(0);
    for(Person* p : people) delete p;
    people.clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() < 7) continue;

        int code = parts[0].trimmed().toInt();
        QString str1 = parts[1].trimmed();
        QString str2 = parts[2].trimmed();
        QString str3 = parts[3].trimmed();

        bool ok1, ok2, ok3;
        int day = parts[4].trimmed().toInt(&ok1);
        int month = parts[5].trimmed().toInt(&ok2);
        int year = parts[6].trimmed().toInt(&ok3);

        if (!ok1 || !ok2 || !ok3) continue;

        QDate today = QDate::currentDate();
        QDate date(year, month, day);

        if (!date.isValid()) continue;
        if (date > today) continue;

        if (code == 1) {
            RuP* r = new RuP(str1, str2, str3, date);
            people.push_back(r);
            addToTable(r);
        }
        else if (code == 2) {
            AmP* a = new AmP(str3, str1, str2, date);
            people.push_back(a);
            addToTable(a);
        }
    }
    file.close();
}

void MainWindow::addToTable(Person* p)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QString fullName = p->getSurname() + " " + p->getName();
    if (dynamic_cast<RuP*>(p)) {
        RuP* r = dynamic_cast<RuP*>(p);
        if (!r->getPatronymic().isEmpty()) fullName += " " + r->getPatronymic();
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem("RuP"));
    } else {
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem("AmP"));
    }

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(fullName));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(p->getDate().toString("dd.MM.yyyy")));
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    int row = index.row();
    if (row < 0 || row >= people.size()) return;

    Person* selectedPerson = people[row];

    AddForm form(this);
    form.setPersonData(selectedPerson);

    int result = form.exec();

    if (result == 1) {
        ui->tableWidget->removeRow(row);
        delete people[row];
        people.remove(row);
    }
}