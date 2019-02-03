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
    Mat big_image_new_gray, big_image_new_gray_inverse;

    int circleRadious = imageDimension/6;
    // create an image
    QString filenameCurrentImageBody;
    QString dayDate;
    QString position;
    for(int i=0;i< data.count(); i++){
        // selectiong which image(different type depending if injection done and where) take
        dayDate = data[i];
        if(iniezione[i].compare("yes") == 0){ // iniezione effettuata
            position = posizione[i];
            if(position.toInt() != 0 && position.toInt()< 19) // effettuata frontalmente
                filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension\\injectionSite_front_" + position + ".png";
            else if(position.toInt() != 0 && position.toInt()>= 19) // effettuata posteriormente
                filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension\\injectionSite_back_" +  position + ".png";
        }
        else{ // iniezione non effettuata
            position="0";
            filenameCurrentImageBody = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\rebif\\ridimension\\injectionSite_0.png";
        }
        //cout << "path: " << filenameCurrentImageBody.toStdString() << endl;
        // image has been selected and now circle needle and date will be added
        Mat currentImageBody = imread(filenameCurrentImageBody.toStdString(),CV_LOAD_IMAGE_COLOR);
        if(position.toInt() != 0){
            putText(currentImageBody, dayDate.toStdString(), Point(imageDimension/12,imageDimension-60), FONT_HERSHEY_TRIPLEX , 1.7, CV_RGB(0,0,0), 1.0 );
            // draw circle in center
            circle(currentImageBody, Point((imageDimension-1)/2, (imageDimension-1)/2), circleRadious, CV_RGB(0,0,0), -2 , 8, 0);
        }

        // new imahe created with circle needle and date is added to compose the big one
        currentImageBody.copyTo(big_image_new(cv::Rect(border_pixel+index_x*(imageDimension+1),border_pixel+index_y*(imageDimension+1),imageDimension, imageDimension)));
        imshow( "imageBody", currentImageBody );

        // ending one line of the big image we will pass to the next one
        index_x++;
        if(index_x>images_in_x-1){
            index_y++;
            index_x=0;
        }
    }

    // create the gray image of he one just created
    cv::cvtColor(big_image_new, big_image_new_gray, CV_BGR2GRAY);
    big_image_new_gray_inverse = 255 - big_image_new_gray;
    cv::threshold(big_image_new_gray_inverse,big_image_new_gray_inverse,1,255,cv::THRESH_BINARY);
    big_image_new_gray = 255-big_image_new_gray_inverse;

    imwrite("C:\\Users\\Fabio Roncato\\Documents\\Qt\\09_01_2019_rebif\\rebif_color.jpg" , big_image_new );
    imwrite("C:\\Users\\Fabio Roncato\\Documents\\Qt\\09_01_2019_rebif\\rebif_gray.jpg" , big_image_new_gray );

    QString final_image = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\09_01_2019_rebif\\test_image.jpg";
    Mat test_image = imread(final_image.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat outputMat;
    test_image.copyTo(outputMat, big_image_new_gray);
    imwrite("C:\\Users\\Fabio Roncato\\Documents\\Qt\\09_01_2019_rebif\\outputMat.jpg" , outputMat );


    cout << "temporary finish !!!" << endl;
    waitKey(0);

    return a.exec();
}



