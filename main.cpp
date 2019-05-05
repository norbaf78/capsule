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

Mat sumTwoMat3Channel(Mat matImage, Mat matMask, Vec3b th){
    int rows = matImage.rows; // is equal in matMask
    int cols = matImage.cols; // is equal in matMask
    int type = matImage.type(); // is equal in matMask
    Mat newMatEnsamble = matImage.clone();
    cout << matMask.type() << endl;
    cout << matImage.type() << endl;
    cout << newMatEnsamble.type() << endl;
    if((matImage.rows == matMask.rows) && (matImage.cols == matMask.cols)){
        if((matImage.channels() == 3) && (matMask.channels() == 3) && (matImage.type() == matMask.type())){
            for(int index_y = 0;index_y < rows; index_y++){
                for(int index_x = 0;index_x < cols; index_x++){
                    Vec3b val = matMask.at<Vec3b>(Point(index_x,index_y));
                    if (val != th && index_y > 0){
                        newMatEnsamble.at<Vec3b>(Point(index_x,index_y)) = Vec3b(0,0,0) + val;
                        //cout << val << " " << Vec3b(0,0,0) << endl ;
                    }
                }
            }
            return newMatEnsamble;
        }
        else{
            return Mat(1,1,0);
        }
    }
    else{
        return Mat(0,0,0);
    }
}

// 10453 x 16885

int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);

    const int images_in_x = 26; //23*47 = 1081 // 26*42 = 1092
    const int images_in_y = 42;
    const int border_pixel = 1;
    const int imageDimension = 401; // dimension images in "C:\Users\Fabio Roncato\Documents\images_rebif\rebif\ridimension" imageDimension x imageDimension
    const int circleRadious = imageDimension/6;
    QDate startingDay(2010, 5, 17);
    QDate endingDay(2013, 5, 4);


    ////////////////////////////////////////////////////////////////////////////////////
    // open file with information of date, injection yes or no, point of injection (if injection has been done)
    ////////////////////////////////////////////////////////////////////////////////////
    QFile file("C:\\Users\\Fabio Roncato\\Documents\\Qt\\09_01_2019_rebif\\date.txt");
    if(!file.open(QFile::ReadOnly|QFile::Text))
        cout << "No file found" << endl;

    ////////////////////////////////////////////////////////////////////////////////////
    // read the data from the file and create three QStringList with those informations
    ////////////////////////////////////////////////////////////////////////////////////
    QStringList data;
    QStringList iniezione;
    QStringList posizione;
    while ( !file.atEnd() ) {
      QString line = file.readLine();   // read the line
      QStringList list = line.split(" "); // split the line
      QString dateString = list[0]; // read the date
      QString yesOrNo = list[1]; // read yes or no (if the injection has been done or not)
      QString position = list[2]; // read the position (in case the injection has not been done the value will been 0)
      data.append(dateString); // the value are append for the data in "data"
      iniezione.append(yesOrNo); // the value are append for the injection in "iniezione"
      posizione.append(position); // the value are append for the injection position in "posizione"
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
    /// create an image containing all the images for all the days (imageDimension is the dimenion of the small images used)
    ///////////////////////////////////////////////////////////////////////////////////////
    int index_x=0, index_y=0;
    Mat big_image_new(2*border_pixel+(images_in_y*imageDimension)+(images_in_y-1)*1, 2*border_pixel+(images_in_x*imageDimension)+(images_in_x-1)*1, CV_8UC3, Scalar(0,0,0));

    ////////////////////////////////////////////////////////////////////////////////////
    /// load the original image will be used ad background
    ///////////////////////////////////////////////////////////////////////////////////////
    QString final_image = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\test_image3_update.jpg";
    Mat test_image = imread(final_image.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat test_image_gray;
    cv::cvtColor(test_image, test_image_gray, CV_BGR2GRAY);
    cv::cvtColor(test_image_gray, test_image, CV_GRAY2BGR);

    // create an image
    QString filenameCurrentImageBody;
    QString dayDate;
    QString position;
    for(int i=0;i< data.count(); i++){ // for all the date (for all the days)
        /////////////////////////////////////////////////////////////////////////
        // generating which image(different type depending if injection done and where) take
        /////////////////////////////////////////////////////////////////////////
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
        //image has been selected and now circle needle and date will be added
        Mat currentImageBody = imread(filenameCurrentImageBody.toStdString(),CV_LOAD_IMAGE_COLOR);
        cv::threshold(currentImageBody,currentImageBody,254,255,cv::THRESH_BINARY);
        if(position.toInt() != 0){
            putText(currentImageBody, dayDate.toStdString(), Point(imageDimension/14,imageDimension-20), FONT_HERSHEY_TRIPLEX , 1.7, CV_RGB(0,0,0), 2.0 );
            circle(currentImageBody, Point((imageDimension-1)/2, (imageDimension-1)/2), circleRadious, CV_RGB(220,220,220), -2 , 8, 0);
        }

/*
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
*/

        /////////////////////////////////////////////////////////////////////////
        // generating an cutting image of the same area in the original image
        /////////////////////////////////////////////////////////////////////////
        Rect roi = cv::Rect(border_pixel+index_x*(imageDimension+1),border_pixel+index_y*(imageDimension+1),imageDimension, imageDimension);
        Mat croppedImage = Mat(test_image_gray, roi);

        //Mat MatTransparent;
        //cv::inRange(currentImageBody, cv::Scalar(0,0,0), cv::Scalar(241,241,241), MatTransparent);
        //croppedImage.copyTo(currentImageBody, 255-MatTransparent);
        currentImageBody.copyTo(big_image_new(cv::Rect(border_pixel+index_x*(imageDimension+1),border_pixel+index_y*(imageDimension+1),imageDimension, imageDimension)));

        //cv::imshow("transparent pixel", MatTransparent);
        //imshow( "currentImageVoid", currentImageVoid );
        imshow( "imageBody", currentImageBody );
        imshow( "croppedImage", croppedImage );
        //waitKey(0);

        // ending one line of the big image we will pass to the next one
        index_x++;
        if(index_x>images_in_x-1){
            index_y++;
            index_x=0;
        }
    }

    // this image is 24 bit image 3 channel
    imwrite("C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\rebif_color.jpg" , big_image_new );
    Mat xx = sumTwoMat3Channel(test_image,big_image_new,Vec3b(255,255,255));
    cout << "xx.rows: " << xx.rows << " xx.cols: " << xx.cols << " xx.channel: " << xx.channels() << endl;
    imwrite("C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\rebif_color_done.jpg" , xx );

    cout << "temporary finish !!!" << endl;
    waitKey(0);

    return a.exec();
}

// cerchi copriago color panna 240,240,240
// immagine piccole del valore medio ed interno del corpo della persona come immagine

