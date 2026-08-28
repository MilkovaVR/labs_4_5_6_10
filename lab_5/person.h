#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QDate>

class Person {
protected:
    QString surname;
    QString name;
    QDate birthDate;
public:
    Person(QString s, QString n, QDate d) : surname(s), name(n), birthDate(d) {}
    virtual ~Person() {}

    virtual QString Craft() const = 0;

    QString getSurname() const { return surname; }
    QString getName() const { return name; }
    QDate getDate() const { return birthDate; }
};

class RuP : public Person {
private:
    QString patronymic;
public:
    RuP(QString s, QString n, QString p, QDate d) : Person(s, n, d), patronymic(p) {}
    QString getPatronymic() const { return patronymic; }
    QString Craft() const override {
        QString p = patronymic.isEmpty() ? "-" : patronymic;
        return "Фамилия: " + surname + "\nИмя: " + name + "\nОтчество: " + p + "\nДата: " + birthDate.toString("dd.MM.yyyy");
    }
};

class AmP : public Person {
private:
    QString secondName;
public:
    AmP(QString s, QString n, QString s2, QDate d) : Person(s, n, d), secondName(s2) {}
    QString Craft() const override {
        return "Имя: " + name + "\nВторое имя: " + secondName + "\nФамилия: " + surname + "\nДата: " + birthDate.toString("MM-dd-yyyy");
    }
};

#endif
