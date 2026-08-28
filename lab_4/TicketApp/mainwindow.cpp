#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ticket.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Билет");

    ui->fromLineEdit->setPlaceholderText("Москва");
    ui->toLineEdit->setPlaceholderText("Минск");
    ui->dateTimeLineEdit->setPlaceholderText("дд.мм.гггг/чч:мм");
    ui->seatLineEdit->setPlaceholderText("1-5: A-D, 6-30: A-F");

    baggageGroup = new QButtonGroup(this);
    baggageGroup->addButton(ui->baggageYesRadioButton);
    baggageGroup->addButton(ui->baggageNoRadioButton);

    ui->baggageYesRadioButton->setChecked(false);
    ui->baggageNoRadioButton->setChecked(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::validateFromTo(const QString& text, QString& errorMsg)
{
    QRegularExpression regex("^[A-ZА-ЯЁ][a-zа-яё]*$");

    if (text.isEmpty())
    {
        errorMsg = "Поле не может быть пустым";
        return false;
    }

    if (!regex.match(text).hasMatch()) {
        errorMsg = "Только буквы, первая буква заглавная";
        return false;
    }

    return true;
}

bool MainWindow::validateDateTime(const QString& text, QString& errorMsg)
{
    QRegularExpression regex("^\\d{2}\\.\\d{2}\\.\\d{4}/\\d{2}:\\d{2}$");

    if (text.isEmpty()) {
        errorMsg = "Поле не может быть пустым";
        return false;
    }

    if (!regex.match(text).hasMatch()) {
        errorMsg = "Формат: дд.мм.гггг/чч:мм (например: 11.11.2026/12:00)";
        return false;
    }

    int day   = text.mid(0, 2).toInt();
    int month = text.mid(3, 2).toInt();
    int year  = text.mid(6, 4).toInt();
    int hour  = text.mid(11, 2).toInt();
    int minute = text.mid(14, 2).toInt();

    if (month < 1 || month > 12) {
        errorMsg = "Месяц должен быть от 1 до 12";
        return false;
    }
    if (year < 2024 || year > 2030) {
        errorMsg = "Год должен быть от 2024 до 2030";
        return false;
    }
    if (hour < 0 || hour > 23) {
        errorMsg = "Часы должны быть от 0 до 23";
        return false;
    }
    if (minute < 0 || minute > 59) {
        errorMsg = "Минуты должны быть от 0 до 59";
        return false;
    }

    auto isValidDate = [](int d, int m, int y) -> bool {
        if (d < 1) return false;

        int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};

        bool isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        if (m == 2 && isLeap) daysInMonth[1] = 29;

        return d <= daysInMonth[m - 1];
    };

    if (!isValidDate(day, month, year)) {
        errorMsg = "Такой даты не существует";
        return false;
    }

    return true;
}

bool MainWindow::validateSeat(const QString& text, QString& errorMsg)
{
    if (text.isEmpty()) {
        errorMsg = "Поле не может быть пустым";
        return false;
    }

    QRegularExpression regex1("^[1-5][A-D]$");
    QRegularExpression regex2("^([1-2][0-9]|30)[A-F]$");

    if (regex1.match(text).hasMatch() || regex2.match(text).hasMatch()) {
        return true;
    }

    errorMsg = "Формат места: 1-5A-D, 6-30A-F (например: 12F или 5A)";
    return false;
}

void MainWindow::on_resetButton_clicked()
{
    ui->fromLineEdit->clear();
    ui->toLineEdit->clear();
    ui->dateTimeLineEdit->clear();
    ui->seatLineEdit->clear();

    baggageGroup->removeButton(ui->baggageYesRadioButton);
    baggageGroup->removeButton(ui->baggageNoRadioButton);

    ui->baggageYesRadioButton->setChecked(false);
    ui->baggageNoRadioButton->setChecked(false);

    baggageGroup->addButton(ui->baggageYesRadioButton);
    baggageGroup->addButton(ui->baggageNoRadioButton);

    ui->foodCheckBox->setChecked(false);
    ui->taxiCheckBox->setChecked(false);
    ui->vipCheckBox->setChecked(false);

    QString normalStyle = "";
    ui->fromLineEdit->setStyleSheet(normalStyle);
    ui->toLineEdit->setStyleSheet(normalStyle);
    ui->dateTimeLineEdit->setStyleSheet(normalStyle);
    ui->seatLineEdit->setStyleSheet(normalStyle);

    qDebug() << "Форма сброшена";
}

void MainWindow::on_saveButton_clicked()
{
    QString errorMsg;


    QString from = ui->fromLineEdit->text().trimmed();
    if (!validateFromTo(from, errorMsg)) {
        QMessageBox::warning(this, "Ошибка валидации", "Поле 'Откуда': " + errorMsg);
        ui->fromLineEdit->setStyleSheet("background-color: #ffcccc;");
        return;
    }
    ui->fromLineEdit->setStyleSheet("");


    QString to = ui->toLineEdit->text().trimmed();
    if (!validateFromTo(to, errorMsg)) {
        QMessageBox::warning(this, "Ошибка валидации", "Поле 'Куда': " + errorMsg);
        ui->toLineEdit->setStyleSheet("background-color: #ffcccc;");
        return;
    }
    ui->toLineEdit->setStyleSheet("");


    QString dateTime = ui->dateTimeLineEdit->text().trimmed();
    if (!validateDateTime(dateTime, errorMsg)) {
        QMessageBox::warning(this, "Ошибка валидации", "Поле 'Дата и время': " + errorMsg);
        ui->dateTimeLineEdit->setStyleSheet("background-color: #ffcccc;");
        return;
    }
    ui->dateTimeLineEdit->setStyleSheet("");


    QString seat = ui->seatLineEdit->text().trimmed().toUpper();
    if (!validateSeat(seat, errorMsg)) {
        QMessageBox::warning(this, "Ошибка валидации", "Поле 'Место': " + errorMsg);
        ui->seatLineEdit->setStyleSheet("background-color: #ffcccc;");
        return;
    }
    ui->seatLineEdit->setStyleSheet("");


    if (!ui->baggageYesRadioButton->isChecked() && !ui->baggageNoRadioButton->isChecked()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите 'Да' или 'Нет' для дополнительного багажа");
        return;
    }
    bool extraBaggage = ui->baggageYesRadioButton->isChecked();


    bool food     = ui->foodCheckBox->isChecked();
    bool taxi     = ui->taxiCheckBox->isChecked();
    bool vipLounge = ui->vipCheckBox->isChecked();

    Ticket ticket(from, to, dateTime, seat, extraBaggage, food, taxi, vipLounge);

    if (ticket.saveToFile("result.txt")) {
        QMessageBox::information(this, "Успех", "Билет успешно сохранён в файл result.txt на рабочем столе");
        qDebug() << "Билет сохранён!";
        qDebug() << "Откуда:" << from << "Куда:" << to << "Место:" << seat;
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл");
    }
}