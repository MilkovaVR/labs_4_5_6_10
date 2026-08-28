#ifndef TICKET_H
#define TICKET_H

#include <QString>
#include <QTextStream>

class Ticket
{
private:
    QString from;      
    QString to;        
    QString dateTime;  
    QString seat;      
    bool extraBaggage;
    bool food;         
    bool taxi;         
    bool vipLounge;    

public:
    Ticket(QString from, QString to, QString dateTime, QString seat,
           bool extraBaggage, bool food, bool taxi, bool vipLounge);

    bool saveToFile(const QString& filename);
};

#endif // TICKET_H
