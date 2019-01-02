#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QTime>
#include <QString>
#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <QFile>

using namespace std;
using namespace cv;


int mcm(int a,int b){ // a > b
   while(a!=b){
    if(a>b)a-=b;
    else b-=a;
   }
   return a;
}

int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);

    const int boxInX = 10;
    const int boxInY = 10;

    QString imgPath = "C:\\Users\\Fabio Roncato\\OneDrive\\Immagini\\Importazioni fotocamera\\2016-04-19\\20150103_134557.jpg";
    Mat image = imread(imgPath.toStdString(),CV_LOAD_IMAGE_COLOR);

    int dimBoxInX = image.cols/boxInX;
    int dimBoxInY = image.rows/boxInY;


    for(int lineX = 1; lineX < boxInX; lineX++)
        line(image, Point(lineX*dimBoxInX,0), Point(lineX*dimBoxInX,image.rows), CV_RGB(0,255,0), 1, 8, 0);
    for(int lineY = 1; lineY < boxInY; lineY++)
        line(image, Point(0,lineY*dimBoxInY), Point(image.cols, lineY*dimBoxInY), CV_RGB(0,255,0), 1, 8, 0);

    Mat newImage(image.rows, image.cols, image.type(), Scalar(0,0,0));

    // cycle on all the small square
    for(int lineX = 1; lineX < boxInX; lineX++){
        for(int lineY = 1; lineY < boxInY; lineY++){
            // cycle on all the pixels inside the small square
            for(int newX = 1; newX < dimBoxInX; newX++){ // inside the small box
                for(int newY = 1; newY < dimBoxInY; newY++){ // inside the small box
                    if(lineX == 2 && lineY == 3){
                        Vec3b colorPixel = image.at<Vec3b>(Point(lineX*dimBoxInX+newX,lineY*dimBoxInY+newY));
                        newImage.at<Vec3b>(Point(lineX*dimBoxInX+newX,lineY*dimBoxInY+newY))[0] = 255;
                    }
                }
            }
        }
    }



    Size size(image.cols/2, image.rows/2);
    Mat imageResized;
    resize(image,imageResized,size);//resize image
    namedWindow( "resize", WINDOW_AUTOSIZE );
    imshow( "resize", imageResized );

    Mat newImageResized;
    resize(newImage,newImageResized,size);//resize image
    namedWindow( "newResize", WINDOW_AUTOSIZE );
    imshow( "newResize", newImageResized );

    cout << "mcm(" << image.cols << "," << image.rows << "): " << mcm(image.cols,image.rows) << endl;
    cout << "temporary finish !!!" << endl;
    waitKey(0);

    return a.exec();
}



