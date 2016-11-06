#ifndef CONFETTI
#define CONFETTI

#include <QtOpenGL>

/*
 * Auteur : Stefano Alder
 * Classe representant un objet confetti
 * Cette classe defini les caracteristiques d'un confetti et ces methode d'affichage
 */


//Permet de definir le nombre de confetti affiché
#define NOMBRECONFETTI 1500

class Confetti
{
public:
    //Constructeur de l'objet confetti qui prend en paramètre chaque axe (x,y,z), la vitesse de deplacement du confetti
    // et la couleur du confetti(r,g,b)
    explicit Confetti(float p_x, float p_y, float p_z, float p_speed, float p_r, float p_g, float p_b);
    //Methode qui permet de dessiner l'objet confetti et de permettre son deplacement par l'intermediraire de l'incrementation du parametre z
    void draw();
    //Methode qui permet diminuer la durée de vie de l'objet, ce qui permet de le detruire a un certain moment
    void decreaseTTL();
    //(Getter) Methode qui permet d'obtenir la valeur de ttl qui est private
    int getTTL();

private:
    //Coordonnées du confetti
    float x, y, z;
    //Vitesse du confetti
    float speed;
    //Parametre rouge/vert/bleu du confetti
    float r,g,b;
    //Temps de vie de confetti (temps d'affichage)
    float TTL;

};

#endif // CONFETTI

