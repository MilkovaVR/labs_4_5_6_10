#ifndef TICKET_H
#define TICKET_H

#include <QString>
#include <QTextStream>

// Класс для хранения данных билета
class Ticket
{
private:
    QString from;      // Откуда
    QString to;        // Куда
    QString dateTime;  // Дата и время
    QString seat;      // Место
    bool extraBaggage; // Доп. багаж (true - Да, false - Нет)
    bool food;         // Питание
    bool taxi;         // Такси
    bool vipLounge;    // VIP зал

public:
    // Конструктор для заполнения всех полей
    Ticket(QString from, QString to, QString dateTime, QString seat,
           bool extraBaggage, bool food, bool taxi, bool vipLounge);

    // Метод для сохранения в файл
    bool saveToFile(const QString& filename);
};

#endif // TICKET_H
