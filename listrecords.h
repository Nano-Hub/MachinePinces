#ifndef LISTRECORDS_H
#define LISTRECORDS_H

#include <QListView>
#include "listmodelrecord.h"

/*
 * Auteur : Sylvain Blanc
 * Class héritant de QListViex et appliquand un model ListModelRecord
 */

class ListRecords : public QListView
{
    Q_OBJECT

public:
    // Constructeur, set le modele
    explicit ListRecords(QWidget *parent = 0);
};

#endif // LISTRECORDS_H
