#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "simpleflow.h"
#include <QMainWindow>
#include <QTimer>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QTime>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSound>
#include <QDateTime>
#include <QGraphicsPixmapItem>
#include <QPixmap>

/*
 * Auteur : Stefano Alder - Sylvain Blanc
 * Classe principale permettant de gérer les actions sur l'interface utilisateur
 * Cette classe defini les connections entre l'interface et les actions engendrées
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Constructeur de la classe
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Slot connecté a un bouton qui permet de démarrer/redemarrer une partie en prenant en compte le niveau
    void restart();
    //Slot connecté a un timer qui permet de mettre a jour l'image camera et le deplacement du bras toutes les 20 ms
    void myUpdate();
    //Slot connecté a un timer qui permet de mettre a jour les temps (total et boule actuelle)
    void onTimeChanged();
    //Slot connecté a un signal provenant de la clase myglwidget qui permet de passer a la manche suivante (si il y en a une)
    void nextRound(bool);
    //Slot connecte au double clique sur la liste qui permet d'appeler la fonction displayRecPic
    void on_listView_doubleClicked(const QModelIndex &index);

private:
    float vx;
    float vy;
    //Le timer qui permet d'avoir un time out toutes les 1 s pour incrementer les temps (total et boule)
    QTimer* timeGame;
    //L'interface qt
    Ui::MainWindow *ui;
    //Le timer qui permet d'avoir un time out toutes les 20 ms pour l'affichage de la camera
    QTimer *timer;
    //Variable stockant le nombre de balles restantes
    int ballLeft;
    //Variable permettant de savoir si on est en cours de partie
    bool gameStarted;
    //Variable representant le temps total ( de la partie)
    float time;
    //Variable representant le temps de la manche actuelle (pour une boule)
    float timeBall;
    //Variable representant le niveau selectionné par les check box
    int levelSelected;
    //Variable qui permet de sauvegarder le nom de la photo prise en debut de partie
    QString lastImgSaved;
    //Variable permettant de savoir si l'animation de fin est en train d'être joué
    bool winAnimation;
    //Scene permettant d'afficher la photo du gagnant
    QGraphicsScene *scenePic;
    //View permettant d'afficher la photo du gagnant
    QGraphicsView *viewPic;
    //Variable pour rajouter un element dans la view
    QGraphicsPixmapItem *myPic;
    //Methode qui permet d'afficher la photo du gagnant
    void displayRecPic(QString name);

};

#endif // MAINWINDOW_H
