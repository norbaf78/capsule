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
    cout << "------- sumTwoMat3Channel ------- start";
    int rows = matImage.rows; // is equal in matMask
    int cols = matImage.cols; // is equal in matMask
    int type = matImage.type(); // is equal in matMask
    Mat newMatEnsamble = matImage.clone();
    cout << matMask.type() << endl;
    cout << matImage.type() << endl;
    cout << newMatEnsamble.type() << endl;
    if((matImage.rows == matMask.rows) && (matImage.cols == matMask.cols)){
        cout << "qui si 2" << endl;
        if((matImage.channels() == 3) && (matMask.channels() == 3) && (matImage.type() == matMask.type())){
            cout << "qui si 3" << endl;
            for(int index_y = 0;index_y < rows; index_y++){
                for(int index_x = 0;index_x < cols; index_x++){
                    Vec3b val = matMask.at<Vec3b>(Point(index_x,index_y));
                    if (val != th && index_y > 0){
                        newMatEnsamble.at<Vec3b>(Point(index_x,index_y)) = Vec3b(0,0,0) + val;
                    }
                }
            }
            cout << "------- sumTwoMat3Channel ------- end";
            return newMatEnsamble;
        }
        else{
            cout << "------- sumTwoMat3Channel ------- end";
            return Mat(1,1,0);
        }
    }
    else{
        cout << "------- sumTwoMat3Channel ------- end";
        return Mat(0,0,0);
    }
}

Mat sumTwoMat1Channel(Mat matImage, Mat matMask, Vec3b th){
    int rows = matImage.rows; // is equal in matMask
    int cols = matImage.cols; // is equal in matMask
    int type = matImage.type();
    Mat newMatEnsamble = matImage.clone();
    cout << matMask.type() << endl;
    cout << matImage.type() << endl;
    cout << newMatEnsamble.type() << endl;
    if((matImage.rows == matMask.rows) && (matImage.cols == matMask.cols)){
        if(matImage.channels() == 3) {
            for(int index_y = 0;index_y < rows; index_y++){
                for(int index_x = 0;index_x < cols; index_x++){
                    Vec3b val = matMask.at<Vec3b>(Point(index_x,index_y));
                    if (val != th){
                        newMatEnsamble.at<Vec3b>(Point(index_x,index_y)) = Vec3b(0,0,0);
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

// 16 in x sono 90 cm
// 23 in y sono quindi 130

int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);
    const int images_in_x = 23;
    const int images_in_y = 16;
    const int imageDimension_y = 915; // dimension images in "C:\Users\Fabio Roncato\Documents\images_rebif\new_rebif" imageDimension x imageDimension
    const int imageDimension_x = 526;
    const int border_pixel = 2; //imageDimension_x/2;
    QDate startingDay(2010, 5, 17);
    QDate endingDay(2011, 5, 18);
    QFile file("C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\date_primo_anno.txt");
    QString pathImageFrontInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_front_";
    QString pathImageBackInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_back_";
    QString imageNoInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_0.png";
    QString savePathBigInjectionImage = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\rebif_color_done.jpg";
    QString imageSentence = "C:\\Users\\Fabio Roncato\\Documents\\rebif\\ScrittaDaPreparare_6.png";
    QString imageSentencePlusBigImageDone = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\out.jpg";
    QString imagePhotoToAdd = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\IMG_20190531_161202.jpg";
    QString imageFinal = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\out2.jpg";



    ////////////////////////////////////////////////////////////////////////////////////
    // open file with information of date, injection yes or no, point of injection (if injection has been done)
    // and read the data from the file and create three QStringList with those informations
    ////////////////////////////////////////////////////////////////////////////////////    
    if(!file.open(QFile::ReadOnly|QFile::Text))
        cout << "No file found" << endl;

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
    Mat big_image_new(2*border_pixel+(images_in_y*imageDimension_y)+(images_in_y-1)*1, 2*border_pixel+(images_in_x*imageDimension_x)+(images_in_x-1)*1, CV_8UC3, Scalar(0,0,0));
    cv::waitKey(0);

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
                filenameCurrentImageBody = pathImageFrontInjection + position + ".png";
            else if(position.toInt() != 0 && position.toInt()>= 19) // effettuata posteriormente
                filenameCurrentImageBody = pathImageBackInjection + position + ".png";
        }
        else{ // iniezione non effettuata
            position="0";
            filenameCurrentImageBody = imageNoInjection;
        }
        //image has been selected and date injection will be added
        Mat currentImageBody = imread(filenameCurrentImageBody.toStdString(),CV_LOAD_IMAGE_COLOR);
        cv::threshold(currentImageBody,currentImageBody,254,255,cv::THRESH_BINARY);
        if(position.toInt() != 0){
            putText(currentImageBody, dayDate.toStdString(), Point(60,imageDimension_y-110), FONT_HERSHEY_TRIPLEX , 2.0, CV_RGB(0,0,0), 4.0 );
        }

        /////////////////////////////////////////////////////////////////////////
        // insert the small image into the big one created before
        /////////////////////////////////////////////////////////////////////////
        currentImageBody.copyTo(big_image_new(cv::Rect(border_pixel+index_x*(imageDimension_x+1),border_pixel+index_y*(imageDimension_y+1),imageDimension_x, imageDimension_y)));
        // ending one line of the big image we will pass to the next one
        index_x++;
        if(index_x>images_in_x-1){
            index_y++;
            index_x=0;
        }
    }

    imwrite(savePathBigInjectionImage.toStdString(), big_image_new);
    Mat matMask = imread(imageSentence.toStdString());
    // add the two image ( big image injection + sentence image) and save it
    Mat out = sumTwoMat1Channel(big_image_new, matMask, Vec3b(255,255,255));
    imwrite(imageSentencePlusBigImageDone.toStdString() , out );



    // add the previous image and the photo image( big image injection + sentence image  + photo image) and save it
    Mat image = imread(imagePhotoToAdd.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat out2 = sumTwoMat3Channel(image, out, Vec3b(255,255,255));
    imwrite(imageFinal , out2 );


    cout << "temporary finish !!!" << endl;
    waitKey(0);

    return a.exec();
}


