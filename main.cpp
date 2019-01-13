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

    const int boxInX = 40;
    const int boxInY = 20;
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

    // cycle on all the small square
    for(int lineX = 0; lineX < boxInX; lineX++){
        for(int lineY = 0; lineY < boxInY; lineY++){

        }
    }

/*    int daysBetweenDate = endingDay.toJulianDay() - startingDay.toJulianDay();
    QDate currentDay = startingDay;
    for(int i=0;i<daysBetweenDate;i++){
        cout << currentDay.toString(Qt::ISODate).toStdString() << " yes" << endl;
        currentDay = currentDay.addDays(1);
    }
    cout << "Days between date: " << daysBetweenDate << endl;
*/
    cout << QDir::currentPath().toStdString() << endl;

    QFile file("C:\\Users\\Fabio Roncato\\Documents\\Qt\\09_01_2019_rebif\\date.txt");
    if(!file.open(QFile::ReadOnly|QFile::Text))
        cout << "No file found" << endl;

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
      //cout << dateString.toStdString() << " - " << yesOrNo.toStdString() << " - " << position.toStdString();
    }

    cout << endl;
    cout << "data elements: " << data.count() << endl;
    cout << "iniezione elements: " << iniezione.count() << endl;
    cout << "posizione elements: " << posizione.count() << endl;

    int countInjection=0;
    for(int i=0;i< data.count(); i++){
        //cout << iniezione[i].toStdString() << " " << countInjection << endl;
        if(iniezione[i].compare("yes") == 0)
            countInjection++;
    }
    cout << "Injection done: " << countInjection << endl;

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



