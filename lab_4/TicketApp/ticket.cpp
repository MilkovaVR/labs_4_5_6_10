#include "ticket.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>


Ticket::Ticket(QString from, QString to, QString dateTime, QString seat,
               bool extraBaggage, bool food, bool taxi, bool vipLounge)
{
    this->from = from;
    this->to = to;
    this->dateTime = dateTime;
    this->seat = seat;
    this->extraBaggage = extraBaggage;
    this->food = food;
    this->taxi = taxi;
    this->vipLounge = vipLounge;
}

bool Ticket::saveToFile(const QString& filename)
{

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fullPath = desktopPath + "/" + filename;

    QFile file(fullPath);

    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Не удалось открыть файл:" << fullPath;
        return false;
    }

    QTextStream out(&file);

    out << "========================================\n";
    out << "Билет:\n";
    out << "  Откуда: " << from << "\n";
    out << "  Куда: " << to << "\n";
    out << "  Дата и время: " << dateTime << "\n";
    out << "  Место: " << seat << "\n";
    out << "  Доп. багаж: " << (extraBaggage ? "Да" : "Нет") << "\n";
    out << "  Услуги:\n";
    out << "    - Питание: " << (food ? "Да" : "Нет") << "\n";
    out << "    - Такси: " << (taxi ? "Да" : "Нет") << "\n";
    out << "    - VIP зал: " << (vipLounge ? "Да" : "Нет") << "\n";
    out << "========================================\n\n";

    file.close();

    qDebug() << "Файл сохранен на рабочий стол:" << fullPath;
    return true;
}
