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
#include <QDate>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);

    const int boxInX = 19;
    const int boxInY = 57;
    int imageDimension = 401;
    QDate startingDay(2010, 5, 17);
    QDate endingDay(2013, 5, 4);

    QString imgPath = "C:\\Users\\Fabio Roncato\\OneDrive\\Immagini\\Importazioni fotocamera\\2016-04-19\\20150103_134557.jpg";
    Mat image = imread(imgPath.toStdString(),CV_LOAD_IMAGE_COLOR);
    int dimBoxInX = image.cols/boxInX;
    int dimBoxInY = image.rows/boxInY;
    for(int lineX = 1; lineX < boxInX; lineX++)
        line(image, Point(lineX*dimBoxInX,0), Point(lineX*dimBoxInX,image.rows), CV_RGB(255,255,255), 1, 8, 0);
    for(int lineY = 1; lineY < boxInY; lineY++)
        line(image, Point(0,lineY*dimBoxInY), Point(image.cols, lineY*dimBoxInY), CV_RGB(255,255,255), 1, 8, 0);

    Mat newImage(image.rows, image.cols, image.type(), Scalar(0,0,0));

// // cycle on all the small square
//    for(int lineX = 0; lineX < boxInX; lineX++){
//        for(int lineY = 0; lineY < boxInY; lineY++){
//
//        }
//    }

// // this part of the code create the list of days between two dates (this data are now available inside date.txt file)
//    int daysBetweenDate = endingDay.toJulianDay() - startingDay.toJulianDay();
//    QDate currentDay = startingDay;
//    for(int i=0;i<daysBetweenDate;i++){
//        cout << currentDay.toString(Qt::ISODate).toStdString() << " yes" << endl;
//        currentDay = currentDay.addDays(1);
//    }
//    cout << "Days between date: " << daysBetweenDate << endl;

    // open file with information of date, injection yes or no, point of injection (if done)
    QFile file("C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\date.txt");
    if(!file.open(QFile::ReadOnly|QFile::Text))
        cout << "No file found" << endl;

    // get the data from the files and create three QStringList with those informations
    QStringList data;
    QStringList iniezione;
    QStringList posizione;
    while ( !file.atEnd() ) {
      QString line = file.readLine();
      QStringList list = line.split(" ");
      QString dateString = list[0];
      QString yesOrNo = list[1];
      QString position = list[2];
      data.append(dateString);
      iniezione.append(yesOrNo);
      posizione.append(position);
      cout << dateString.toStdString() << " - " << yesOrNo.toStdString() << " - " << position.toStdString() << endl;
    }
    cout << endl;
    cout << "data elements: " << data.count() << endl;
    cout << "iniezione elements: " << iniezione.count() << endl;
    cout << "posizione elements: " << posizione.count() << endl;

    // read number of days in the period and number of injection have been done
    int countInjectionDone=0;
    for(int i=0;i< data.count(); i++){
        if(iniezione[i].compare("yes") == 0)
            countInjectionDone++;
    }
    cout << "Injection done: " << countInjectionDone << endl;
    cout << "Total days: " << data.count() << endl;

    int circleRadious = imageDimension/6;
    // create an image
    QString filenameCurrentImageBody;
    QString dayDate;
    QString position;
    for(int i=0;i< data.count(); i++){
        dayDate = data[i];
        if(iniezione[i].compare("yes") == 0){ // iniezione effettuata
            position = posizione[i];
            if(position.toInt() != 0 && position.toInt()< 19)
                filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension\\injectionSite_front_" + position + ".png";
            else if(position.toInt() != 0 && position.toInt()>= 19)
                filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension\\injectionSite_back_" +  position + ".png";
        }
        else{
            position="0";
            filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension\\injectionSite_0.png";
        }
        cout << "path: " << filenameCurrentImageBody.toStdString() << endl;
        Mat currentImageBody = imread(filenameCurrentImageBody.toStdString(),CV_LOAD_IMAGE_COLOR);
        if(position.toInt() != 0){
            putText(currentImageBody, dayDate.toStdString(), Point(imageDimension/4,imageDimension-60), FONT_HERSHEY_COMPLEX_SMALL, 1.4, CV_RGB(0,0,0), 0.9 );
            // draw circle in center
            circle(currentImageBody, Point((imageDimension-1)/2, (imageDimension-1)/2), circleRadious, CV_RGB(0,0,0), 2 , 8, 0);
        }

        imshow( "imageBody", currentImageBody );
        waitKey(0);
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

    cout << "temporary finish !!!" << endl;
    waitKey(0);

    return a.exec();
}



