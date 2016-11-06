#ifndef LISTMODELRECORD_H
#define LISTMODELRECORD_H

#include <QAbstractListModel>
#include "record.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>

/*
 * Auteur : Sylvain Blanc
 * Classe gérant le modèle de la vue ListRecord
 * Cette classe sauvegarde un veteur de tous les records ainsi que le nom du fichier dans lequel écrire
 */

class ListModelRecord : public QAbstractListModel // Hérite de la classe QAbstractListModel, elle doit donc réimplémenter des fonctions
{
    Q_OBJECT
public:
    //Constructeur avec le fichier en paramètre
    explicit ListModelRecord(QString fileToLoad, QObject *parent = 0);
    //Méthode à implémenter
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    // Méthode à implémenter (Gère l'affichage)
    QVariant data(const QModelIndex &index, int role) const;
    // Ajoute un record et retrie la liste
    void addRecord(Record *r);
    // Retourne le record à un certain index de la liste
    Record* atInd(int i);

private:
    // Vector stockant les records
    std::vector<Record*> *records_;
    // Nom du fichier ou sont sauvegardés les records
    QString fichierRecord;

};


#endif // LISTMODELRECORD_H
