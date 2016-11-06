#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vx = 0;
    vy = 0;
    gameStarted = false;
    timer = new QTimer(this);
    timeGame = new QTimer(this);
    time = 0;
    ballLeft = 0;
    timeBall=0;
    levelSelected = 1;
    winAnimation = false;
    scenePic = new QGraphicsScene();
    viewPic = new QGraphicsView(scenePic);

    connect(timer, SIGNAL(timeout()),this,SLOT(myUpdate()));
    timer->start(20);

    connect(timeGame, SIGNAL(timeout()),this,SLOT(onTimeChanged()));
    timeGame->start(1000);

    connect(ui->label,SIGNAL(handMoved(float,float)),ui->widget, SLOT(moveClaw(float,float)));

    connect(ui->pushButton_restart,SIGNAL(clicked()),this,SLOT(restart()));

    connect(ui->label, SIGNAL(handApproaching()), ui->widget, SLOT(startFall()));

    connect(ui->widget, SIGNAL(endAnimation(bool)), this, SLOT(nextRound(bool)));

}

void MainWindow::myUpdate()
{
    if(gameStarted)
    {
        ui->widget->moveClaw(vx, vy);
    }
    if(winAnimation)
    {
        ui->widget->updateGL();
    }
    ui->label->Update(gameStarted);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::displayRecPic(QString name)
{
    myPic = scenePic->addPixmap(QPixmap(name));
    viewPic->setWindowTitle("Photographie du meilleur joueur !");
    viewPic->show();
}

void MainWindow::restart()
{
    ui->label->setPhotoTaken(false);
    gameStarted = true;
    ui->label->Update(gameStarted);
    winAnimation = false;
    ballLeft = 3;
    time = 0;
    timeBall = 0;
    ui->label_tempsTotal->setText("Temps total: " + QString::number(time));
    ui->label_currentsphere->setText("Temps manche: " + QString::number(timeBall)+ "s");
    ui->label_spheresRestantes->setText("Spheres restantes: " +QString::number(ballLeft));
    lastImgSaved = QString::number(QDateTime::currentMSecsSinceEpoch());
    ui->label->grab().save("picture/" +lastImgSaved+ ".png","PNG");
    ui->label->setPhotoTaken(true);

    if(ui->radioButton_Level1->isChecked())
    {
        levelSelected = 1;
    }
    else if(ui->radioButton_Level2->isChecked())
    {
        levelSelected = 2;
    }
    else if(ui->radioButton_Level3->isChecked())
    {
        levelSelected = 3;
    }
    ui->widget->addBall(levelSelected);
}

void MainWindow::nextRound(bool isNotFailed)
{
    if(isNotFailed)
    {
        if(ballLeft != 1)
        {
            QSound::play("musics/ApplauseSound.wav");
            ballLeft--;
            timeBall = 0;
            ui->label_currentsphere->setText("Temps manche: " + QString::number(timeBall)+ "s");
            ui->label_spheresRestantes->setText("Spheres restantes: " +QString::number(ballLeft));
            ui->widget->addBall(levelSelected);
        }
        else
        {
            gameStarted = false;
            winAnimation = true;
            ui->widget->addConfetti();
            ballLeft = 0;
            ui->label_spheresRestantes->setText("Spheres restantes: " +QString::number(ballLeft));
            QSound::play("musics/JuraSound.wav");

            //animation victoire
            bool ok;
            QString text = QInputDialog::getText(this, tr("Nouveau record !"),tr("Entrez votre pseudonyme:"), QLineEdit::Normal,"", &ok);
            if (ok && !text.isEmpty())
            {
                //Variable texte avec le pseudo du joueur
                ((ListModelRecord*)this->ui->listView->model())->addRecord(new Record(text,this->time, this->levelSelected, "picture/" + this->lastImgSaved+ ".png"));
            }
        }
    }
}

void MainWindow::onTimeChanged()
{
    if(gameStarted)
    {
        time +=1;
        timeBall+=1;
        ui->label_tempsTotal->setText("Temps total: " + QString::number(time) + "s");
        ui->label_currentsphere->setText("Temps manche: " + QString::number(timeBall)+ "s");
    }
}


void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    displayRecPic(((ListModelRecord *) ui->listView->model())->atInd(index.row())->fileName);
}
