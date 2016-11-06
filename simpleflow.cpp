#include "simpleflow.h"


using namespace cv;
using namespace std;


SimpleFlow::SimpleFlow(QWidget *parent) : QLabel(parent)
{
    photoTaken = false;
    zeroSpree = 0;
    frameWidth=320;
    frameHeight=240;
    subImageWidth=40;
    subImageHeight=40;
    rect = Rect((frameWidth-subImageWidth)/2,frameHeight/2+(frameHeight/2-subImageHeight)/2,subImageWidth,subImageHeight);
    p1 = Point(rect.x+subImageWidth/2,rect.y+subImageHeight/2);

    n =  0; // Toutes les 5 frames : moyennes des déplacements

    cap = VideoCapture(1); // open the default camera
    //    cout<<"width :"<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<endl;
    //    cout<<"height :"<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,frameWidth);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight);
    if(!cap.isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
    }

    // Get frame1

    cap >> frame1;
    // Mirror effect
    cv::flip(frame1,frame1,1);
    // Extract rect1 and convert to gray
    //cv::cvtColor(Mat(f1,rect),f1,COLOR_BGR2GRAY);
    Mat(frame1,rect).copyTo(frameRect1);
    Mat(frame1,rect).copyTo(oldMatrix);
    // Init output window
    // namedWindow("WebCam",1);
    // oldMatrix(flow);
}


bool SimpleFlow::isFlowCorrect(float u) {
    return !cvIsNaN(u) && (fabs(u) < 1e9);
}


void SimpleFlow::Update(bool gameStarted)
{

    // Get frame2
    cap >> frame2;
    cv::cvtColor(frame2,frame2,CV_BGR2RGB);


    cv::flip(frame2,frame2,1);
    // Extract rect2 and convert to gray
    //cv::cvtColor(Mat(frame2,rect),frameRect2,COLOR_BGR2GRAY);
    Mat(frame2,rect).copyTo(frameRect2);

    // Compute optical flow
    //float start = (float)getTickCount();
    cv::calcOpticalFlowSF(frameRect1, frameRect2,flow,3, 2, 4, 4.1, 25.5, 18, 55.0, 25.5, 0.35, 18, 55.0, 25.5, 10);
    //cv::calcOpticalFlowSF(frameRect1, frameRect2, flow, 3, 2, 4);
    // printf("calcOpticalFlowSF : %lf sec\n", (getTickCount() - start) / getTickFrequency());



    if(isHandApproaching(flow,oldMatrix))
    {
        qDebug()<<"HAND";
        if(gameStarted)
            emit(handApproaching());
    }
    else
    {
        // Compute mean flow
        float um,vm;
        calc_mean(flow,um,vm);
        // printf("um: %lf\t", um);
        //printf("vm: %lf\n", vm);

        // Draw green rectangle and mean vector


        if(n !=5)
        {
            n++;
            xMoy += um;
            yMoy += vm;
        }
        else
        {
            xMoy /= 5;
            yMoy /= 5;
            p3.x=p1.x+4*xMoy;
            p3.y=p1.y+4*yMoy;

            n=0;
        }

        if(photoTaken)
        {
            rectangle(frame2,rect,Scalar( 0, 255, 0),2);
            p2.x=p1.x+4*um;
            p2.y=p1.y+4*vm;
            line(frame2,p1,p2,Scalar(255,255,255),2);
            line(frame2,p1,p3,Scalar(255,0,0),2);
        }

        emit handMoved(xMoy, yMoy);
    }

    QImage img= QImage((const unsigned char*)(frame2.data),frame2.cols,frame2.rows,QImage::Format_RGB888);
    // display on label
    this->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    this->resize(this->pixmap()->size());

    Mat(frame2,rect).copyTo(frameRect1);

    flow.copyTo(oldMatrix);

}



SimpleFlow::~SimpleFlow()
{

    // the camera will be deinitialized automatically in Videoure destructor
}

void SimpleFlow::setPhotoTaken(bool ok)
{
    photoTaken = ok;
}

bool SimpleFlow::isHandApproaching(Mat flow1, Mat oldMatrix)
{
    int counter = 0;
    const int rows = flow1.rows;
    const int cols = flow1.cols;
    float xc = this->width()/2;
    float yc = this->height()/2;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Vec2f flow1_at_point = flow1.at<Vec2f>(y, x);

            float u1 = flow1_at_point[0];
            float v1 = flow1_at_point[1];

            Vec2f flow2_at_point = oldMatrix.at<Vec2f>(y, x);

            float u2 = flow2_at_point[0];
            float v2 = flow2_at_point[1];

            if(sqrt(pow(u2-xc,2) + pow(v2-yc,2))>=sqrt(pow(u1-xc,2) + pow(v1-yc,2)))
            {
                counter++;
            }
        }
    }
    if(counter == 0)
    {
        zeroSpree+=1;
    }
    else
    {
        zeroSpree=0;
    }
    //Si on recoit 3 zeros de suite on considere que la main s'est rapprochée
    if(zeroSpree == 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SimpleFlow::calc_mean(Mat flow1,float &um, float &vm)
{
    float sumu = 0;
    float sumv = 0;
    int counter = 0;
    const int rows = flow1.rows;
    const int cols = flow1.cols;

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x) {
            Vec2f flow1_at_point = flow1.at<Vec2f>(y, x);

            float u1 = flow1_at_point[0];
            float v1 = flow1_at_point[1];

            if (isFlowCorrect(u1) && isFlowCorrect(v1)) {
                sumu += u1;
                sumv += v1;
                counter++;
            }
        }
    }
    um=sumu/(1e-9 + counter);
    vm=sumv/(1e-9 + counter);
}




