
// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>


#include "myglwidget.h"



MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    posZBall = 0;
    posXBall = 0;
    posYBall = 0;
    sizeBall = 0;
    ecartement = 60;
    xRot = -90*16;
    yRot = 0;
    zRot = -90*16;
    zoom = -3;

    active = false;

    x = 0.5;
    y = 0.5;
    z = 0.3;

    for(int  i = 0; i < 3; i++)
    {
        cone[i] = gluNewQuadric();
        cylinder[i] = gluNewQuadric();
        sphereDoigt[i] = gluNewQuadric();
    }

    for(int  i = 0; i <12; i++)
    {
        cylinderBox[i] = gluNewQuadric();
    }

    sphere = gluNewQuadric();
    cylinderAttache = gluNewQuadric();
    ball = gluNewQuadric();

}

MyGLWidget::~MyGLWidget()
{
}


void MyGLWidget::addBall(int level)
{
    //Random position in the box

    //x >= BOUNDX && x <= 1 - BOUNDX

    qsrand(QTime::currentTime().msec());
    posXBall = 0;
    posYBall = 0;

    posXBall = qrand()%100;
    posXBall *= 0.006;
    posXBall += BOUNDX;

    posYBall = qrand()%100;
    posYBall *= 0.006;
    posYBall += BOUNDY;

    if(posYBall <= 0.40f)
    {
        if(posXBall >= 0.60f)
        {
            posYBall += 0.30f;
        }
    }

    posZBall = 0;

    if(level == 1)
    {
        sizeBall = SIZE_LEVEL_1;
    }
    else if(level == 2)
    {
        sizeBall = SIZE_LEVEL_2;
    }
    else if(level == 3)
    {
        sizeBall = SIZE_LEVEL_3;
    }

    active = true;
}


QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::gray);

    //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glPolygonMode (GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH);

    //Lumière pour les confettis
    glEnable(GL_LIGHT0);
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {5.0f, 0.5f, 0.5f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    //Lumière pour l'attrape sphère
    glEnable(GL_LIGHT1);
    GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos1[] = {0.5f, -0.5f, 0.5f,1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);


    for(int  i = 0; i < 3; i++)
    {
        gluQuadricDrawStyle(cone[i], GLU_FILL);
        gluQuadricDrawStyle(cylinder[i], GLU_FILL);
        gluQuadricDrawStyle(sphereDoigt[i], GLU_FILL);
    }
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricDrawStyle(cylinderAttache, GLU_FILL);
    gluQuadricDrawStyle(ball, GLU_FILL);

    for(int  i = 0; i < 12; i++)
    {
        gluQuadricDrawStyle(cylinderBox[i], GLU_FILL);
    }

    imageTSE = loadBMP_custom("picture/tse.bmp");
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-0.5,-0.5, zoom);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    std::list<Confetti>::iterator p = listConfetti.begin();
    if(p->getTTL() != 0)
    {
        while(p != listConfetti.end())
        {
            p->draw();
            p->decreaseTTL();
            p++;
        }
    }
    else
    {
        listConfetti.clear();
    }
    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (float)width / height,0.01, 200);

    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    //Fonction permettant de se deplacer sur l'espace 3D

    //lastPos = event->pos();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    //Fonction Permettant de zoomer sur l'attrape sphere

    /*if(event->delta()>0)
    {
        zoom+=0.2;
    }
    else
    {
        zoom-=0.2;
    }

    this->updateGL();
    event->accept();*/
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //Fonction permettant de se deplacer dans l'espace 3D


   /* int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();*/

}

void MyGLWidget::addConfetti()
{

    qsrand(QTime::currentTime().msec());

    for(int i = 0; i < NOMBRECONFETTI; i++)
    {
        listConfetti.push_back(Confetti((qrand()%200+50)/100.0f,(rand()%200-50)/100.0f,1.5f,(qrand()%200 + 50)/100.0f,(qrand()%100)/100.0f,(qrand()%100)/100.0f,(qrand()%100)/100.0f));
    }
}

void MyGLWidget::moveClaw(float p_y, float p_x)
{
    qDebug()<<active;
    if(active)
    {
        if(x >= BOUNDX && x <= 1 - BOUNDX)
        {
            if(y >= BOUNDY && y <= 1 - BOUNDY)
            {
                x = x + p_x/400;
                y = y + p_y/400;

            }
            else if(y < BOUNDY)
            {
                y = BOUNDY+0.01f;
            }
            else
            {
                y = 1 - BOUNDY-0.01f;
            }
        }
        else if(x < BOUNDX)
        {
            x = BOUNDX+0.01f;
        }
        else
        {
            x = 1 - BOUNDX -0.01f;
        }

        updateGL();
    }



}

void MyGLWidget::startFall()
{
    float seuil = 0.2;
    if(x < posXBall + seuil && x > posXBall - seuil && y < posYBall + seuil && y > posYBall - seuil )
    {
        animate();
    }
    else
    {
        animateFailed();
    }
}


void MyGLWidget::animateFailed()
{
    active = false;


    while(z < 0.75)
    {
        z += 0.05;
        this->updateGL();
        QThread::msleep(100);
    }


    while(ecartement > 30)
    {
        ecartement -= 1;
        this->updateGL();
        QThread::msleep(100);
    }

    while(z > 0.3)
    {
        z -= 0.05;
        this->updateGL();
        QThread::msleep(100);
    }


    while(x < 0.9f)
    {
        x+=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }
    while(y > 0.1f)
    {
        y-=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }

    while(ecartement < 60)
    {
        if(ecartement < 60)
        {
            ecartement += 1;
        }

        this->updateGL();
        QThread::msleep(10);
    }
    this->updateGL();



    while(x >= 0.5f)
    {

        x-=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }
    while(y <= 0.5f)
    {
        y+=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }


    active = true;
    emit(endAnimation(false));
}


void MyGLWidget::animate()
{
    active = false;
    x = posXBall;
    y = posYBall;

    int ecartementMax = 0;

    while(z < 0.75)
    {
        z += 0.05;
        this->updateGL();
        QThread::msleep(100);
    }
    if(sizeBall == SIZE_LEVEL_1)
    {
        ecartementMax = 58;
    }
    else if(sizeBall == SIZE_LEVEL_2)
    {
        ecartementMax = 46;
    }
    else if(sizeBall == SIZE_LEVEL_3)
    {
        ecartementMax = 40;
    }

    while(ecartement > ecartementMax)
    {
        ecartement -= 1;
        this->updateGL();
        QThread::msleep(100);
    }
    while(z > 0.3)
    {
        z -= 0.05;
        posZBall -= 0.05;
        this->updateGL();
        QThread::msleep(100);
    }


    while(x < 0.85f)
    {
        x+=0.05f;
        posXBall +=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }
    while(y > 0.15f)
    {
        y-=0.05f;
        posYBall -=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }

    float vz = 0;
    while(ecartement < 60 || posZBall < 0.20f)
    {
        if(ecartement < 60)
        {
            ecartement += 1;
        }
        if(posZBall < 0.20f)
        {
            vz += GRAVITY;
            posZBall+=vz;
        }
        this->updateGL();
        QThread::msleep(10);
    }
    while(posZBall < 0.25f)
    {
        posZBall+=0.001;
        this->updateGL();
        QThread::msleep(10);
    }


    while(x >= 0.5f)
    {

        x-=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }
    while(y <= 0.5f)
    {
        y+=0.05f;
        this->updateGL();
        QThread::msleep(100);
    }


    active = true;
    emit(endAnimation(true));


}

void MyGLWidget::drawBall()
{
    if(sizeBall != 0)
    {
        glPushMatrix();
        glColor4f(0,0,1,1);
        glTranslatef(posXBall,posYBall,sizeBall - posZBall);
        gluSphere(ball, sizeBall, 36, 18);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        glColor4f(0,0,0,0);
        glTranslatef(posXBall,posYBall,sizeBall - posZBall);
        gluSphere(ball, 0.001, 36, 18);
        glPopMatrix();
    }

}

void MyGLWidget::drawGlass()
{
    glPushMatrix();

    glEnable(GL_BLEND);
    glDepthMask (GL_FALSE);	// should be used with transparency

    glColor4f(1.1, 1, 1.1, 0.1);

    for(int i =0; i < 10; i++)
    {
        for(int j =0; j < 10; j++)
        {

            glPushMatrix();
            glTranslatef(0, j*0.1f, i*0.1f);
            glBegin(GL_QUADS);
            glColor4f(1.1, 1, 1.1, 0.1);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, 0.1);
            glVertex3f(0, 0.1, 0.1);
            glVertex3f(0, 0.1, 0);

            glEnd();
            glPopMatrix();

        }
    }


    glDepthMask (GL_TRUE);	// should be used with transparency
    glDisable(GL_BLEND);

    glPopMatrix();
}
void MyGLWidget::drawFinger(float ecartementDoigt, int num)
{

    glPushMatrix();
    glColor3f(0,1,0);
    glRotatef(ecartementDoigt, 1, 0, 0);
    glTranslatef(0,0,-0.8);

    glPushMatrix();
    glScalef(0.1,0.1,0.8);
    glTranslatef(0,0,-0.5);
    ////volume cylinder 10 true true
    gluCylinder(cylinder[num],1,1,1,36,18);
    glPopMatrix();

    glColor3f(1,0,0);
    glPushMatrix();
    glTranslatef(0,0,-0.3);
    glScalef(0.25,0.25,0.25);
    gluSphere(sphereDoigt[num],1,36,18);



    glPopMatrix();

    glColor3f(0,1,0);
    glPushMatrix();

    glRotatef(50, -1,0,0);
    glTranslatef(0,0.25,-0.3734);
    glScalef(.15,.15,-1.1);

    gluCylinder(cone[num],1,0,1,36,18);
    ////volume cone 20 true
    glPopMatrix();

    glPopMatrix();
}


GLuint MyGLWidget::loadBMP_custom(const char * imagepath)

{

    GLuint texture;

    /* int width, height;

    unsigned char * data;

    FILE * file;

    file = fopen( imagepath, "rb" );

    if ( file == NULL ) return 0;
    width = 1024;
    height = 512;
    data = (unsigned char *)malloc( width * height * 3 );
    //int size = fseek(file,);
    fread( data, width * height * 3, 1, file );
    fclose( file );

    for(int i = 0; i < width * height ; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];

        data[index] = R;
        data[index+2] = B;

    }*/
    texture = 1;

    QImage maTexture(imagepath);
    maTexture = QGLWidget::convertToGLFormat(maTexture);


    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, maTexture.width(),maTexture.height(),0,GL_RGBA,GL_UNSIGNED_BYTE, maTexture.bits());
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    //free( data );

    return texture;

}


void MyGLWidget::draw()
{

    drawBall();
    glPushMatrix();
    glPushMatrix();
    glTranslatef(0.75, 0, 0);

    glScalef(0.25, 0.25, 0.25);
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor4f(0,1,0,1);
    glVertex3f( 0,0,-1);
    glVertex3f( 0,1,-1);
    glVertex3f( 0,1,1);
    glVertex3f( 0,0,1);
    glEnd();
    glPopMatrix();
    glRotatef( -90,0,0,1);
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor4f(0,1,0,1);
    glVertex3f( 0,0,-1);
    glVertex3f( 0,1,-1);
    glVertex3f( 0,1,0);
    glVertex3f( 0,0,0);
    glEnd();
    glPopMatrix();
    glTranslatef( -1,0,0);
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor4f(0,1,0,1);
    glVertex3f( 0,0,-1);
    glVertex3f( 0,1,-1);
    glVertex3f( 0,1,1);
    glVertex3f( 0,0,1);
    glEnd();

    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0,0,-0.25);
    glBegin(GL_QUADS);
    glColor4f(0,1,0,1);
    glVertex3f( 1,0,0);
    glVertex3f( 1,0.25,0);
    glVertex3f( 0.75,0.25,0);
    glVertex3f( 0.75,0,0);
    glEnd();

    glPopMatrix();

    glPushMatrix();


    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, imageTSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask (GL_FALSE);
    glDisable(GL_LIGHTING);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0);//texture 0,0
    glVertex3f( 0,0,0);
    glTexCoord2d(0.25,0);//texture 0.25,0
    glVertex3f( 0,0.25,0);
    glTexCoord2d(0.25,0.75);//texture 0.25,0.75
    glVertex3f( 0.75,0.25,0);
    glTexCoord2d(0,0.75);//texture 0,0.75
    glVertex3f( 0.75,0,0);

    glTexCoord2d(0.25,0);//texture 0.25,0
    glVertex3f( 0,0.25,0);
    glTexCoord2d(1,0);//texture 1,0
    glVertex3f( 0,1,0);
    glTexCoord2d(1,1);//texture 1,1
    glVertex3f( 1,1,0);
    glTexCoord2d(0.25,1);//texture 0.25,1
    glVertex3f( 1,0.25,0);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);



    glPopMatrix();

    drawGlass();


    glRotatef( -90,0,0,1);
    drawGlass();

    glTranslatef( -1,0,0);

    drawGlass();

    glPopMatrix();
    glPushMatrix();

    glTranslatef( 1,0,0);
    drawGlass();

    glPopMatrix();

    glPushMatrix();

    glColor4f(1,1,1,0);


    glBegin(GL_QUADS);
    glTexCoord2d(0,1);
    glVertex3f( 0,0,1);
    glTexCoord2d(1,1);
    glVertex3f( 0,1,1);
    glTexCoord2d(1,0);
    glVertex3f( 1,1,1);
    glTexCoord2d(0,0);
    glVertex3f( 1,0,1);
    glEnd();

    glPopMatrix();


    glColor3f(0,0,1);

    glPushMatrix();
    glTranslatef( 0,0,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[0],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 1,1,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[1],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0,1,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[2],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 1,0,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[3],1,1,1,36,18);

    glPopMatrix();


    glPushMatrix();
    glTranslatef( 1,1,0);
    glRotatef( 90,1,0,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[4],1,1,1,36,18);

    glPopMatrix();


    glPushMatrix();
    glTranslatef( 0,1,1);
    glRotatef( 90,1,0,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[5],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0,1,1);
    glRotatef( 90,0,1,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[6],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0,1,0);
    glRotatef( 90,0,1,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[7],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0,1,0);
    glRotatef( 90,1,0,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[8],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 1,1,1);
    glRotatef( 90,1,0,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[9],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0,0,1);
    glRotatef( 90,0,1,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[10],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0,0,0);
    glRotatef( 90,0,1,0);
    glScalef( 0.01,0.01,1);
    gluCylinder(cylinderBox[11],1,1,1,36,18);

    glPopMatrix();

    glPushMatrix();
    glPushMatrix();
    glColor3f(1,0,0);
    /*
    glPushMatrix();
    glTranslatef(x, 0,1);
    glRotatef( 90,1,0,0);
    glScalef( 0.01,0.01,1);
      gluCylinder(cylinderBox[1],1,1,1,36,18);
    glPopMatrix();

    glColor3f(0,1,0);

    glPushMatrix();
    glTranslatef( 0.5, y, 1);
    glRotatef( 90,0,1,0);
    glScalef( 0.01,0.01,1);
      gluCylinder(cylinderBox[1],1,1,1,36,18);
    glPopMatrix();*/

    glColor3f(0,0,1);

    glPushMatrix();
    glTranslatef( x, y, 1-z);
    glRotatef( 90,0,0,1);
    glScalef( 0.01,0.01,z);
    gluCylinder(cylinderAttache, 1,1,1,36,18);

    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef( x, y, 1-z);
    glScalef(0.1,0.1,0.1);

    gluSphere(sphere, 0.5, 36, 18);

    //volume sphere 3

    glColor3f(1,1,0);
    drawFinger(ecartement,0);
    glRotatef( 120,0,0,1);
    drawFinger(ecartement,1);
    glRotatef( 120,0,0,1);
    drawFinger(ecartement,2);

    glPopMatrix();

    glPopMatrix();



}
