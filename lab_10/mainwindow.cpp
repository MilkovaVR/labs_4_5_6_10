#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QMouseEvent>
#include <QDate>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet(
        "QWidget { background-color: #A64100; color: white; }"
        "QPushButton { background-color: #006A55; color: white; border-radius: 5px; }"
        "QPushButton#btn_UploadPhoto { background-color: #6A94D4; color: white; border-radius: 5px; }"
        "QLineEdit { background-color: white; color: black; border: 1px solid #ccc; }"
        "QCheckBox { background-color: transparent; color: white; }"
        );

    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);

    ui->lineEdit_Name->installEventFilter(this);
    ui->lineEdit_Surname->installEventFilter(this);
    ui->lineEdit_Nickname->installEventFilter(this);
    ui->lineEdit_ID->installEventFilter(this);
    ui->lineEdit_Email->installEventFilter(this);
    ui->lineEdit_Password->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_UploadPhoto_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите фото", "", "Картинки (*.png *.jpg *.jpeg)");
    if (fileName.isEmpty()) return;

    photoPath = fileName;

    ui->label_Photo->setScaledContents(true);
    ui->label_Photo->setAlignment(Qt::AlignCenter);

    QPixmap pixmap(fileName);
    ui->label_Photo->setPixmap(pixmap);
}

bool MainWindow::validateData()
{
    if (ui->lineEdit_Name->text().isEmpty() ||
        ui->lineEdit_Surname->text().isEmpty() ||
        ui->lineEdit_Nickname->text().isEmpty() ||
        ui->lineEdit_ID->text().isEmpty() ||
        ui->lineEdit_Email->text().isEmpty() ||
        ui->lineEdit_Password->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return false;
    }

    QRegularExpression nameRegex("^[A-Za-zА-Яа-яЁё\\s-]+$");
    if (!nameRegex.match(ui->lineEdit_Name->text()).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "Имя не может содержать цифры или специальные символы!");
        return false;
    }
    if (!nameRegex.match(ui->lineEdit_Surname->text()).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "Фамилия не может содержать цифры или специальные символы!");
        return false;
    }

    QRegularExpression idRegex("^[A-Z][0-9]{3}-[0-9]{2}[A-Z]-[A-Z]$");
    if (!idRegex.match(ui->lineEdit_ID->text()).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "ID-Key должен соответствовать формату A111-11A-A!");
        return false;
    }

    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (!emailRegex.match(ui->lineEdit_Email->text()).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный Email!");
        return false;
    }

    if (ui->lineEdit_Password->text().length() < 6) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен быть не меньше 6 символов!");
        return false;
    }

    if (!ui->checkBox_Europe->isChecked() &&
        !ui->checkBox_Asia->isChecked() &&
        !ui->checkBox_America->isChecked()) {
        QMessageBox::warning(this, "Ошибка", "Выберите хотя бы один сервер!");
        return false;
    }

    if (photoPath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Загрузите фото!");
        return false;
    }

    return true;
}

QString MainWindow::encryptPassword(QString password)
{
    int shift = 3;
    QString encrypted = "";

    for (QChar ch : password) {
        if (ch.isLetter()) {
            int code = ch.unicode();
            code += shift;
            ch = QChar(code);
        }
        else if (ch.isDigit()) {
            int code = ch.unicode();
            code += shift;
            ch = QChar(code);
        }
        encrypted += ch;
    }
    return encrypted;
}

bool MainWindow::isNicknameTaken(QString nickname)
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString accountsPath = desktopPath + "/Accounts";
    return QDir(accountsPath + "/" + nickname).exists();
}

void MainWindow::clearAllFields()
{
    ui->lineEdit_Name->clear();
    ui->lineEdit_Surname->clear();
    ui->lineEdit_Nickname->clear();
    ui->lineEdit_ID->clear();
    ui->lineEdit_Email->clear();
    ui->lineEdit_Password->clear();
    ui->checkBox_Europe->setChecked(false);
    ui->checkBox_Asia->setChecked(false);
    ui->checkBox_America->setChecked(false);
    ui->label_Photo->clear();
    photoPath.clear();
}

void MainWindow::on_btn_Register_clicked()
{
    if (!validateData()) return;

    QString name = ui->lineEdit_Name->text();
    QString surname = ui->lineEdit_Surname->text();
    QString nickname = ui->lineEdit_Nickname->text();
    QString idKey = ui->lineEdit_ID->text();
    QString email = ui->lineEdit_Email->text();
    QString password = ui->lineEdit_Password->text();

    if (isNicknameTaken(nickname)) {
        QMessageBox::warning(this, "Ошибка", "Никнейм " + nickname + " уже занят!");
        return;
    }

    QString encryptedPassword = encryptPassword(password);

    QString desktopPath = QDir::homePath() + "/Desktop";
    QString accountsPath = desktopPath + "/Accounts";
    QString userFolderPath = accountsPath + "/" + nickname;

    QDir().mkpath(userFolderPath);

    if (!photoPath.isEmpty()) {
        QImage image(photoPath);
        if (!image.isNull()) {
            image.save(userFolderPath + "/photo.jpg");
        } else {
            QMessageBox::warning(this, "Ошибка", "Фото не удалось сохранить (файл не является картинкой)!");
        }
    }

    QJsonObject jsonObj;
    jsonObj["имя"] = name;
    jsonObj["фамилия"] = surname;
    jsonObj["никнейм"] = nickname;
    jsonObj["id_key"] = idKey;
    jsonObj["email"] = email;
    jsonObj["сервер"] = ui->checkBox_Europe->isChecked() ? "Европа" :
                            (ui->checkBox_Asia->isChecked() ? "Азия" : "Америка");

    QFile jsonFile(userFolderPath + "/user.json");
    if (jsonFile.open(QIODevice::WriteOnly)) {
        jsonFile.write(QJsonDocument(jsonObj).toJson());
        jsonFile.close();
    }

    QFile txtFile(userFolderPath + "/password.txt");
    if (txtFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&txtFile);
        out.setEncoding(QStringConverter::Encoding::Utf8);
        out << email << "/" << encryptedPassword;
        txtFile.close();
    }

    QMessageBox::information(this, "Регистрация", "Аккаунт успешно создан!\nДанные сохранены в папке: " + userFolderPath);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (obj == ui->lineEdit_Name) ui->lineEdit_Name->clear();
        if (obj == ui->lineEdit_Surname) ui->lineEdit_Surname->clear();
        if (obj == ui->lineEdit_Nickname) ui->lineEdit_Nickname->clear();
        if (obj == ui->lineEdit_ID) ui->lineEdit_ID->clear();
        if (obj == ui->lineEdit_Email) ui->lineEdit_Email->clear();
        if (obj == ui->lineEdit_Password) ui->lineEdit_Password->clear();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}