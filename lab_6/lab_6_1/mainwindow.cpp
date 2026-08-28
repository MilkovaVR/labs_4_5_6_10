#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::validateData(const QString &name, const QString &desc,
                              const QString &hunger, const QString &price, const QString &weight)
{
    if (name.trimmed().isEmpty() || desc.trimmed().isEmpty() ||
        hunger.trimmed().isEmpty() || price.trimmed().isEmpty() || weight.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return false;
    }

    bool hungerOk;
    double hungerVal = hunger.toDouble(&hungerOk);
    if (!hungerOk) {
        QMessageBox::warning(this, "Ошибка", "Значение голода может быть только числом!");
        return false;
    }
    if (hungerVal < 0 || hungerVal > 100) {
        QMessageBox::warning(this, "Ошибка", "Значение голода должно быть от 0 до 100!");
        return false;
    }

    bool priceOk;
    double priceVal = price.toDouble(&priceOk);
    if (!priceOk) {
        QMessageBox::warning(this, "Ошибка", "Цена должна быть числом!");
        return false;
    }
    if (priceVal <= 0) {
        QMessageBox::warning(this, "Ошибка", "Цена должна быть больше 0!");
        return false;
    }

    bool weightOk;
    double weightVal = weight.toDouble(&weightOk);
    if (!weightOk) {
        QMessageBox::warning(this, "Ошибка", "Вес должна быть числом!");
        return false;
    }
    if (weightVal <= 0) {
        QMessageBox::warning(this, "Ошибка", "Вес должен быть больше 0!");
        return false;
    }

    return true;
}

void MainWindow::on_btn_loadTxt_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите txt файл", "", "Текстовые файлы (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл!");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Encoding::Utf8);
    in.setAutoDetectUnicode(true);

    QString name = in.readLine();
    QString desc = in.readLine();
    QString hunger = in.readLine();
    QString price = in.readLine();
    QString weight = in.readLine();

    if (name.isNull() || desc.isNull() || hunger.isNull() || price.isNull() || weight.isNull()) {
        QMessageBox::warning(this, "Ошибка", "В файле должно быть ровно 5 строк (или они пустые)!");
        file.close();
        return;
    }

    ui->lineEdit_name->setText(name);
    ui->lineEdit_desc->setText(desc);
    ui->lineEdit_hunger->setText(hunger);
    ui->lineEdit_price->setText(price);
    ui->lineEdit_weight->setText(weight);

    file.close();
    QMessageBox::information(this, "Успех", "Данные из файла успешно заполнены в форме!");
}

void MainWindow::on_btn_saveJson_clicked()
{
    QString name = ui->lineEdit_name->text();
    QString desc = ui->lineEdit_desc->text();
    QString hunger = ui->lineEdit_hunger->text();
    QString price = ui->lineEdit_price->text();
    QString weight = ui->lineEdit_weight->text();

    if (!validateData(name, desc, hunger, price, weight)) {
        return;
    }

    QJsonObject obj;
    obj["название"] = name;
    obj["описание"] = desc;
    obj["голод"] = hunger;
    obj["цена"] = price;
    obj["вес"] = weight;

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как JSON", "pizza.json", "JSON файлы (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
        file.close();
        QMessageBox::information(this, "Готово", "Объект сохранен в JSON!");
    }
}