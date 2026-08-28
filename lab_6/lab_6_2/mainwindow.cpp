#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QBrush>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget_Good->setColumnCount(5);
    ui->tableWidget_Good->setHorizontalHeaderLabels({"Название", "Описание", "Голод", "Цена", "Вес"});
    ui->tableWidget_Good->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget_Bad->setColumnCount(5);
    ui->tableWidget_Bad->setHorizontalHeaderLabels({"Название", "Описание", "Голод", "Цена", "Вес"});
    ui->tableWidget_Bad->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isValidPizza(const QJsonObject &obj)
{
    QString name = obj.value("название").toString();
    if (name.isEmpty()) return false;

    QString desc = obj.value("описание").toString();
    if (desc.isEmpty()) return false;

    bool hungerOk, priceOk, weightOk;

    double hunger = obj.value("голод").toString().toDouble(&hungerOk);
    double price = obj.value("цена").toString().toDouble(&priceOk);
    double weight = obj.value("вес").toString().toDouble(&weightOk);

    if (!hungerOk || !priceOk || !weightOk) return false;

    if (hunger < 0 || hunger > 100) return false;

    if (price <= 0) return false;

    if (weight <= 0) return false;

    return true;
}

Pizza MainWindow::parsePizza(const QJsonObject &obj)
{
    Pizza p;
    p.name = obj.value("название").toString();
    p.desc = obj.value("описание").toString();
    p.hunger = obj.value("голод").toString().toDouble();
    p.price = obj.value("цена").toString().toDouble();
    p.weight = obj.value("вес").toString().toDouble();
    return p;
}

void MainWindow::on_btn_load_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите JSON", "", "JSON файлы (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray array;
    if (doc.isArray()) {
        array = doc.array();
    } else if (doc.isObject()) {
        array.append(doc.object());
    } else {
        QMessageBox::warning(this, "Ошибка", "Файл пуст или не является JSON!");
        return;
    }

    ui->tableWidget_Good->setRowCount(0);
    ui->tableWidget_Bad->setRowCount(0);

    QVector<Pizza> goodPizzas;
    QJsonArray badArray;

    for (const QJsonValue &val : array) {
        QJsonObject obj = val.toObject();

        QString name = obj.value("название").toString();
        QString desc = obj.value("описание").toString();
        QString hunger = obj.value("голод").toString();
        QString price = obj.value("цена").toString();
        QString weight = obj.value("вес").toString();

        if (!isValidPizza(obj)) {
            int row = ui->tableWidget_Bad->rowCount();
            ui->tableWidget_Bad->insertRow(row);
            ui->tableWidget_Bad->setItem(row, 0, new QTableWidgetItem(name));
            ui->tableWidget_Bad->setItem(row, 1, new QTableWidgetItem(desc));
            ui->tableWidget_Bad->setItem(row, 2, new QTableWidgetItem(hunger));
            ui->tableWidget_Bad->setItem(row, 3, new QTableWidgetItem(price));
            ui->tableWidget_Bad->setItem(row, 4, new QTableWidgetItem(weight));

            QBrush redBrush(QColor(255, 170, 170));

            if (name.isEmpty()) {
                ui->tableWidget_Bad->item(row, 0)->setBackground(redBrush);
            }
            if (desc.isEmpty()) {
                ui->tableWidget_Bad->item(row, 1)->setBackground(redBrush);
            }
            bool hungerOk;
            double hungerVal = hunger.toDouble(&hungerOk);
            if (!hungerOk || hungerVal < 0 || hungerVal > 100) {
                ui->tableWidget_Bad->item(row, 2)->setBackground(redBrush);
            }
            bool priceOk;
            double priceVal = price.toDouble(&priceOk);
            if (!priceOk || priceVal <= 0) {
                ui->tableWidget_Bad->item(row, 3)->setBackground(redBrush);
            }
            bool weightOk;
            double weightVal = weight.toDouble(&weightOk);
            if (!weightOk || weightVal <= 0) {
                ui->tableWidget_Bad->item(row, 4)->setBackground(redBrush);
            }

            badArray.append(obj);
        } else {
            Pizza p = parsePizza(obj);
            goodPizzas.push_back(p);
        }
    }

    std::sort(goodPizzas.begin(), goodPizzas.end(), [](const Pizza &a, const Pizza &b) {
        return a.name > b.name;
    });

    for (const Pizza &p : goodPizzas) {
        int row = ui->tableWidget_Good->rowCount();
        ui->tableWidget_Good->insertRow(row);
        ui->tableWidget_Good->setItem(row, 0, new QTableWidgetItem(p.name));
        ui->tableWidget_Good->setItem(row, 1, new QTableWidgetItem(p.desc));
        ui->tableWidget_Good->setItem(row, 2, new QTableWidgetItem(QString::number(p.hunger)));
        ui->tableWidget_Good->setItem(row, 3, new QTableWidgetItem(QString::number(p.price)));
        ui->tableWidget_Good->setItem(row, 4, new QTableWidgetItem(QString::number(p.weight)));
    }

    if (!goodPizzas.isEmpty()) {
        QFile goodFile("good.json");
        if (goodFile.open(QIODevice::WriteOnly)) {
            QJsonArray goodArray;
            for (const Pizza &p : goodPizzas) {
                QJsonObject obj;
                obj["название"] = p.name;
                obj["описание"] = p.desc;
                obj["голод"] = QString::number(p.hunger);
                obj["цена"] = QString::number(p.price);
                obj["вес"] = QString::number(p.weight);
                goodArray.append(obj);
            }
            goodFile.write(QJsonDocument(goodArray).toJson());
            goodFile.close();
        }
    }
    if (!badArray.isEmpty()) {
        QFile badFile("bad.json");
        if (badFile.open(QIODevice::WriteOnly)) {
            badFile.write(QJsonDocument(badArray).toJson());
            badFile.close();
        }
    }

    QMessageBox::information(this, "Готово", "Корректные объекты сохранены в good.json\nБитые объекты сохранены в bad.json");
}