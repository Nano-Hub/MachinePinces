// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include "GL/glu.h"
#include <QThread>
#include <QSound>
#include "confetti.h"

//Permet de definir les limites en X pour le deplacement de la pince
#define BOUNDX 0.15
//Permet de definir les limites en Y pour le deplacement de la pince
#define BOUNDY 0.15
//Permet de definir la taille de la sphere pour le niveau 1
#define SIZE_LEVEL_1 0.1f
//Permet de definir la taille de la sphere pour le niveau 2
#define SIZE_LEVEL_2 0.07f
//Permet de definir la taille de la sphere pour le niveau 3
#define SIZE_LEVEL_3 0.05f
//Permet de definir la valeur de la gravité pour la sphere
#define GRAVITY 0.0018f;

/*
 * Auteur : Stefano Alder - Sylvain Blanc
 * Classe permettant de gérer toutes ce qui concerne la partie 3d
 */

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    //Constructeur du widget
    explicit MyGLWidget(QWidget *parent = 0);
    //Destructer du widget
    ~MyGLWidget();
    //Methode permettant d'ajouter les confettis dans la liste pour ensuite les afficher
    void addConfetti();
    //Methode permettant d'ajouter la balle dans le terrain selon le niveau
    void addBall(int);

protected:
    //Methode permettant l'initialisation d'open GL
    void initializeGL();
    //Methode permettant de dessiner l'objet
    void paintGL();
    //Methode permettant de centrer le viewport
    void resizeGL(int width, int height);
    //Methode qui retourne la taille minimum
    QSize minimumSizeHint() const;
    //Methode qui retourne la taille Qsize
    QSize sizeHint() const;
    //Methode permettant de traiter les clics de la souris
    void mousePressEvent(QMouseEvent *event);
    //Methode permettant de traiter le deplacement de la souris
    void mouseMoveEvent(QMouseEvent *event);
    //Methode permettant de traiter les evenements de la molette (pour le zoom)
    void wheelEvent(QWheelEvent *event);
    //Methode permettant de d'animer le bras dans le cas ou le joueur a reussi a attraper la sphere
    void animate();
    //Methode permettant de d'animer le bras dans le cas ou le joueur n'a pas reussi a attraper la sphere
    void animateFailed();
    //Methode permettant de charger une texture
    GLuint loadBMP_custom(const char * imagepath);


public slots:
    //Slot permettant de regarder si le joueur a reussi a etre au dessus de la sphere ou non
    void startFall();
    //Slot permettant de definir la rotation en X lors du deplacement dans la souris
    void setXRotation(int angle);
    //Slot permettant de definir la rotation en Y lors du deplacement dans la souris
    void setYRotation(int angle);
    //Slot permettant de definir la rotation en Z lors du deplacement dans la souris
    void setZRotation(int angle);
    //Slot permettant de deplacer le grapin lorsque la main a provoqué un deplacement
    void moveClaw(float, float);

signals:
    //Signal permettant de dire que la rotation en X doit etre changé suite au deplacement de la souris
    void xRotationChanged(int angle);
    //Signal permettant de dire que la rotation en Y doit etre changé suite au deplacement de la souris
    void yRotationChanged(int angle);
    //Signal permettant de dire que la rotation en Z doit etre cahngé suite au deplacement de la souris
    void zRotationChanged(int angle);
    //Signal permettant de signaler a MainWindow que l'animation a est terminé
    void endAnimation(bool);

private:
    //Methode permettant de dessiner presque la totalité de notre attrape-sphère
    void draw();
    //Methode pemettant de dessiner les vitres
    void drawGlass();
    //Methode permettant de dessiner les doigts de la pince
    void drawFinger(float, int);
    //Methode permettant de dessiner la sphere
    void drawBall();
    //Variable représentant le niveau de zoom
    float zoom;
    //Variable représentant le niveau de rotation par rapport a l'axe X
    int xRot;
    //Variable représentant le niveau de rotation par rapport a l'axe Y
    int yRot;
    //Variable représentant le niveau de rotation par rapport a l'axe Z
    int zRot;
    //Variable representant l'angle d'ecartement des doigts
    int ecartement;
    //Variable representant la position sur l'axe X de la sphere
    float posXBall;
    //Variable representant la position sur l'axe Y de la sphere
    float posYBall;
    //Variable representant la position sur l'axe Z de la sphere
    float posZBall;
    //Variable representant la taille de la sphere (selon le niveau)
    float sizeBall;
    //Variable contenant l'image de la texture de TSE
    GLuint imageTSE;
    //Liste contenant les confettis a afficher
    std::list<Confetti> listConfetti;
    //Variable representant la position sur l'axe X de la pince
    float x;
    //Variable representant la position sur l'axe Y de la pince
    float y;
    //Variable representant la position sur l'axe Z de la pince
    float z;
    //Variable permettant de savoir si la pince peut bouger (dans le cas d'une animation)
    bool active;
    //Variable permettant de dessiner la sphere de la pince
    GLUquadricObj *sphere;
    //Variable permettant de dessiner la sphere des doigts
    GLUquadricObj *sphereDoigt[3];
    //Variable permettant de dessiner les cylindres des doigts
    GLUquadricObj *cylinder[3];
    //Variable permettant de dessiner les cones des doigts
    GLUquadricObj *cone[3];
    //Variable permettant de dessiner les bords de la boite
    GLUquadricObj *cylinderBox[12];
    //Variable permettant de dessiner le lien le bras et la boite
    GLUquadricObj *cylinderAttache;
    //Variable permettant de dessiner la sphere dans la boite
    GLUquadricObj *ball;
    //Variable permettant de connaitre la derniere postion de la souris
    QPoint lastPos;
};

#endif // MYGLWIDGET_H

