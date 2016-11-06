#ifndef RECORD_H
#define RECORD_H

#include <QObject>
/*
 * Auteur : Sylvain Blanc
 * Objet record contenant le nom du détenteur du record, le niveau, le temps et le chemin de l'image.
 */
class Record : public QObject
{
    Q_OBJECT
public:
    // Constructeur
    explicit Record(QString nom, float temps, int niveau, QString fn, QObject *parent = 0);
    // retourne un affichage en une ligne d'un record
    QString displayInList();
    //Compare 2 records
    static bool cmpRec(const Record *r1, const Record *r2)
    {
        return r1->time<r2->time;
    }

    // temps du record établi
    float time;
    // niveau sous lequel le record a été effectué
    int level;
    // Nom du détenteur du record
    QString name;
    // Nom du fichier stockant l'image
    QString fileName;
};

#endif // RECORD_H
