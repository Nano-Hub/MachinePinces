#ifndef SIMPLEFLOW_H
#define SIMPLEFLOW_H

#include <QLabel>
#include <cstdio>
#include <iostream>
#include <qdebug.h>
#include <QDate>

#include    <gl\gl.h>                         // Header File For The OpenGL32 Library
#include    <gl\glu.h>                            // Header File For The GLu32 Library
#include    <gl\glaux.h>                          // Header File For The GLaux Library

#include "opencv2/opencv.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;
using namespace std;

/*
 * Auteurs : Stefano Alder et Sylvain Blanc
 *
 */

class SimpleFlow : public QLabel // Cette classe hérite d'un QLabel afin d'afficher les images de la caméra
{
    Q_OBJECT
public:
    // Constructeur héritant du parent et permettant d'initialiser les différentes variables
    explicit SimpleFlow(QWidget *parent = 0);
    // Destructeur
    virtual ~SimpleFlow();
    // Recupere une nouvelle image via la Webcam et effectue les différetns calculs, puis envoie des signals selon les résultats obtenus
    void Update(bool);
    //Methode permettant de changer la valeur permettant de savoir si la photo a ete prise
    void setPhotoTaken(bool);

signals:
    // Envoie un signal avec les infos du vecteur vitesse pour effectuer un mouvement
    void handMoved(float x, float y);
    // Envoie un signal si la pince doit descendre
    void handApproaching();

private:
    // taille de l'image dépendant de la webcam (largeur)
    int frameWidth;
    // taille de l'image dépendant de la webcam (longueur)
    int frameHeight;
    // taille du rectangle de detection, il ne peut pas etre trop grand ou cela cause des ralentissements (largeur)
    int subImageWidth;
    // taille du rectangle de detection, il ne peut pas etre trop grand ou cela cause des ralentissements (longueur)
    int subImageHeight;
    // Rectangle de detection
    Rect rect;
    // points des différents vecteurs vitesse de la main
    Point p1, p2, p3;
    // Différentes matrices stockant des images à différents stades du traitement
    Mat frame1,frame2,flow,frameRect1,frameRect2,oldMatrix;
    // instance de gestion de la webcam
    VideoCapture cap;
    // Calcul le nombre de 0 à la suite (detection de l'approche)
    int zeroSpree;
    // nombre d'itération sur lequel faire la moyenne
    int n;
    // moyenne de la vitesse sur n itérations
    float xMoy, yMoy;
    //Variable permettant de savoir si la photo a ete prise
    bool photoTaken;

private:
    // Vérifie la validité du nombre
    bool isFlowCorrect(float u);
    // Calcul de la mediane
    void calc_mean(Mat flow1,float &um, float &vm);
    // test la detection de l'approche de la main en recevant deux matrices (avant et apres) et renvoie si l'approche a lieu ou pas
    bool isHandApproaching(Mat, Mat);
};


#endif // SIMPLEFLOW_WEBCAM_H

