#include "confetti.h"

Confetti::Confetti(float p_x, float p_y, float p_z, float p_speed, float p_r, float p_g, float p_b)
{
    x = p_x;
    y = p_y;
    z = p_z;
    speed = p_speed;
    r = p_r;
    g = p_g;
    b = p_b;
    TTL = 7000;
}

void Confetti::decreaseTTL()
{
    TTL-=20;
}

int Confetti::getTTL()
{
    return TTL;
}

void Confetti::draw()
{
    z -= speed*0.1f;
    glPushMatrix();
    glColor4f(r,g,b,1);
    glScalef(2,2,2);
    glBegin(GL_QUADS);
    glVertex3f(x,y,z);
    glVertex3f(x,y+0.025f,z);
    glVertex3f(x,y+0.025f,z+0.025f);
    glVertex3f(x,y,z+0.025f);
    glEnd();
    glPopMatrix();


}
