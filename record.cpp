#include "record.h"

Record::Record(QString nom, float temps, int niveau, QString fn, QObject *parent) :
    QObject(parent)
{
    this->level = niveau;
    this->name = nom;
    this->time = temps;
    this->fileName = fn;
}


QString Record::displayInList()
{
    return  + " " + QString::number(time).leftJustified(3, ' ') + "| " + QString::number(level).leftJustified(3, ' ') + "| " + name;
}
