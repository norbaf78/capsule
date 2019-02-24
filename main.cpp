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

    const int images_in_x = 26; //23*47 = 1081 // 26*42 = 1092
    const int images_in_y = 42;
    const int border_pixel = 1;
    const int imageDimension = 401;
    QDate startingDay(2010, 5, 17);
    QDate endingDay(2013, 5, 4);


    ////////////////////////////////////////////////////////////////////////////////////
    // open file with information of date, injection yes or no, point of injection (if done)
    ////////////////////////////////////////////////////////////////////////////////////
    QFile file("C:\\Users\\Fabio Roncato\\Documents\\Qt\\09_01_2019_rebif\\date.txt");
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
     // cout << dateString.toStdString() << " - " << yesOrNo.toStdString() << " - " << position.toStdString() << endl;
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


    ////////////////////////////////////////////////////////////////////////////////////
    /// create an image containing a number of days images (imageDimension is the dimenion of the images)
    ///////////////////////////////////////////////////////////////////////////////////////
    int index_x=0, index_y=0;
    Mat big_image_new(2*border_pixel+(images_in_y*imageDimension)+(images_in_y-1)*1, 2*border_pixel+(images_in_x*imageDimension)+(images_in_x-1)*1, CV_8UC3, Scalar(0,0,0));

    ////////////////////////////////////////////////////////////////////////////////////
    /// load the original image
    ///////////////////////////////////////////////////////////////////////////////////////
    QString final_image = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\test_image.jpg";
    Mat test_image = imread(final_image.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat test_image_gray;
    cv::cvtColor(test_image, test_image_gray, CV_BGR2GRAY);
    cv::cvtColor(test_image_gray, test_image, CV_GRAY2BGR);

    int circleRadious = imageDimension/6;
    // create an image
    QString filenameCurrentImageBody;
    QString dayDate;
    QString position;
    for(int i=0;i< data.count(); i++){
        /////////////////////////////////////////////////////////////////////////
        // generating which image(different type depending if injection done and where) take
        /////////////////////////////////////////////////////////////////////////
        dayDate = data[i];
        if(iniezione[i].compare("yes") == 0){ // iniezione effettuata
            position = posizione[i];
            if(position.toInt() != 0 && position.toInt()< 19) // effettuata frontalmente
                filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension2\\injectionSite_front_" + position + ".png";
            else if(position.toInt() != 0 && position.toInt()>= 19) // effettuata posteriormente
                filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension2\\injectionSite_back_" +  position + ".png";
        }
        else{ // iniezione non effettuata
            position="0";
            filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension2\\injectionSite_0.png";
        }
        //image has been selected and now circle needle and date will be added
        Mat currentImageBody = imread(filenameCurrentImageBody.toStdString(),CV_LOAD_IMAGE_COLOR);
        if(position.toInt() != 0){
            putText(currentImageBody, dayDate.toStdString(), Point(imageDimension/12,imageDimension-60), FONT_HERSHEY_TRIPLEX , 1.7, CV_RGB(0,0,0), 2.0 );
            circle(currentImageBody, Point((imageDimension-1)/2, (imageDimension-1)/2), circleRadious, CV_RGB(240,240,240), -2 , 8, 0);
        }


        /////////////////////////////////////////////////////////////////////////
        // generating an image where the color is the mean color of the same area in the original image
        /////////////////////////////////////////////////////////////////////////
        QString filenameCurrentImageVoid = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension2\\void_image.png";
        Mat currentImageVoid = imread(filenameCurrentImageVoid.toStdString(),CV_LOAD_IMAGE_COLOR);
        Rect roi = cv::Rect(border_pixel+index_x*(imageDimension+1),border_pixel+index_y*(imageDimension+1),imageDimension, imageDimension);
        Mat cropped = Mat(test_image_gray, roi);
        Scalar mean, stdv;
        cv::meanStdDev(cropped,mean,stdv);
        currentImageVoid.setTo((int)mean[0]);


        /////////////////////////////////////////////////////////////////////////
        // generating an cutting image of the same area in the original image
        /////////////////////////////////////////////////////////////////////////
        roi = cv::Rect(border_pixel+index_x*(imageDimension+1),border_pixel+index_y*(imageDimension+1),imageDimension, imageDimension);
        Mat croppedImage = Mat(test_image_gray, roi);


        imshow( "imageBody", currentImageBody );
        imshow( "currentImageVoid", currentImageVoid );
        imshow( "croppedImage", croppedImage );
        waitKey(0);

        // ending one line of the big image we will pass to the next one
        index_x++;
        if(index_x>images_in_x-1){
            index_y++;
            index_x=0;
        }
    }

    cout << "temporary finish !!!" << endl;
    waitKey(0);

    return a.exec();
}

// cerchi copriago color panna 240,240,240
// immagine piccole del valore medio ed interno del corpo della persona come immagine

