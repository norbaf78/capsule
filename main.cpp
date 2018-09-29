#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QTime>
#include <QString>
#include <iostream>
#include <opencv2/opencv.hpp>

#define DEBUG

using namespace std;
using namespace cv;

int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);

    QTextStream out(stdout);
    Mat image;
    int count = 0;
    int rand_number_from0tothis = 10000;
    const int images_in_x = 40;
    const int images_in_y =25;
    const int border_pixel = 257;
    const int circle_center_x = 30;
    const int circle_center_y = 30;
    QString save_path = "C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\saved_image_\\";
    QDirIterator it("C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\Google Foto", QStringList() << "*_org.jpg", QDir::Files, QDirIterator::Subdirectories);
    //QDirIterator it("C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\test_image_", QStringList() << "*_org.jpg", QDir::Files, QDirIterator::Subdirectories);
    int anno_2015[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_2016[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_2017[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_2018[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_have_to_be[12] = {31,28,31,30,31,30,31,31,30,31,30,31};



    while (it.hasNext()){
        it.next();
        QString year = it.fileName().mid(5,4);
        QString month = it.fileName().mid(10,2);
        QString day = it.fileName().mid(13,2);
        QString hour = it.fileName().mid(16,2);
        QString minute = it.fileName().mid(19,2);
        QString second = it.fileName().mid(22,2);
        out << year << " " << month << " " << day << " " << hour << " " << minute << " " << second << endl;
        image = imread(it.filePath().toStdString(),CV_LOAD_IMAGE_COLOR);
        cout << it.filePath().toStdString() << endl;
        //Size size(image.rows/6, image.cols/6);//the dst image size,e.g.100x100
        Size size(256, 256);//the dst image size,e.g.100x100
        resize(image,image,size);//resize image

        if(year.toInt()==2015)
          anno_2015[month.toInt()-1]++;
        else if(year.toInt()==2016)
          anno_2016[month.toInt()-1]++;
        else if(year.toInt()==2017)
          anno_2017[month.toInt()-1]++;
        else if(year.toInt()==2018)
          anno_2018[month.toInt()-1]++;
        else
            cout << "error" << endl;

        if((hour != "00") || (minute != "00") || (second != "00")){
#ifdef DEBUG
            cvtColor(image,image,CV_BGR2GRAY);
            cvtColor(image,image,CV_GRAY2BGR);
            putText(image, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " " + hour.toStdString() + ":" +
                    minute.toStdString() + ":" + second.toStdString(), Point(70/*100*/, 250), FONT_HERSHEY_COMPLEX_SMALL, /*0.6*/0.7, CV_RGB(255,255,255), /*0.8*/0.9);
            circle(image, Point(circle_center_x,circle_center_y), 8, CV_RGB(255,0,0), -1, 8, 0);
            circle(image, Point(circle_center_x,circle_center_y), 3, CV_RGB(255,0,0), -1, 8, 0);
            circle(image, Point(circle_center_x,circle_center_y), 8, CV_RGB(0,0,0), 1, 8, 0);
#endif

#ifndef DEBUG
            putText(image, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " " + hour.toStdString() + ":" +
                    minute.toStdString() + ":" + second.toStdString(), Point(70/*100*/, 250), FONT_HERSHEY_COMPLEX_SMALL, /*0.6*/0.7, CV_RGB(255,255,255), /*0.8*/0.9);
            circle(image, Point(circle_center_x,circle_center_y), 8, CV_RGB(255,255,255), -1, 8, 0);
            circle(image, Point(circle_center_x,circle_center_y), 3, CV_RGB(0,0,0), -1, 8, 0);
            circle(image, Point(circle_center_x,circle_center_y), 8, CV_RGB(0,0,0), 1, 8, 0);
            cvtColor(image,image,CV_BGR2GRAY);
            cvtColor(image,image,CV_GRAY2BGR);
#endif
        }
        else{
#ifdef DEBUG
            cvtColor(image,image,CV_BGR2GRAY);
            cvtColor(image,image,CV_GRAY2BGR);
#endif
            cout << "no pills" << endl;
            putText(image, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " --:--:--", Point(70, 250), FONT_HERSHEY_COMPLEX_SMALL, 0.7, CV_RGB(255,255,255), 0.9);
#ifndef DEBUG
            cvtColor(image,image,CV_BGR2GRAY);
            cvtColor(image,image,CV_GRAY2BGR);
#endif
        }



///////////////////////////////////////////////////////////////////////////
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());
        int randomValue = qrand() % rand_number_from0tothis;
        QString number = QString::number(randomValue);
        imwrite( (save_path + number  + it.fileName()).toStdString() + ".jpg" , image );
        count++;
    //    namedWindow( it.fileName().toStdString(), WINDOW_AUTOSIZE);// Create a window for display.
    //    imshow( it.fileName().toStdString(), image );
    //    waitKey(0);
    //    destroyWindow(it.fileName().toStdString());

    }
    out << "images available: " << count << endl;
    out << "anno 2015" << endl;
    for(int i =0;i<12;i++)
        out << i+1 << ": " << anno_2015[i] << "  -   " << anno_have_to_be[i] << endl;
    out << "anno 2016" << endl;
    for(int i =0;i<12;i++){
        if(i==1) out << i+1 << ": " << anno_2016[i] << "  -   " << anno_have_to_be[i]+1 << endl;
        else out << i+1 << ": " << anno_2016[i] << "  -   " << anno_have_to_be[i] << endl;
    }
    out << "anno 2017" << endl;
    for(int i =0;i<12;i++)
        out << i+1 << ": " << anno_2017[i] << "  -   " << anno_have_to_be[i] << endl;
    out << "anno 2018" << endl;
    for(int i =0;i<12;i++)
        out << i+1 << ": " << anno_2018[i] << "  -   " << anno_have_to_be[i] << endl;




    // create  big image
    int counter=0;
    bool images_completed = false;
    Mat image_small;
    image = imread(save_path.toStdString() + "test_small_image_\\test_small_image_.jpg",CV_LOAD_IMAGE_COLOR);
    int image_cols = image.cols;
    int image_rows = image.rows;

    out << "image: " << save_path << "test_small_image_\\test_small_image_.jpg" << endl;
    out << "image_cols: " << image_cols << "  -  " << "image_rows: " << image_rows << endl;
    int index_x=0, index_y=0;
    Mat big_image(2*border_pixel+(images_in_y*image_rows)+(images_in_y-1)*1, 2*border_pixel+(images_in_x*image_cols)+(images_in_x-1)*1, CV_8UC3, Scalar(255,255,255));

    // first image è stato tolto C360 alla img originale per togliera dalla iterazione
//    QString first_image = "2732_2015-04-02-08-37-49-922_org.jpg.jpg"; //
//    image_small = imread((save_path + first_image).toStdString() ,CV_LOAD_IMAGE_COLOR);  // non viene trovata ???????
//    image_small.copyTo(big_image(cv::Rect(border_pixel+index_x*(image_cols+1),border_pixel+index_y*(image_rows+1),image_small.cols, image_small.rows)));
//    index_x++;
//    counter++;


    QDirIterator it_bn_images("C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\saved_image_", QStringList() << "*C360*.jpg", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it_bn_images.hasNext() && images_completed==false){
       it_bn_images.next();
       image_small = imread(it_bn_images.filePath().toStdString(),CV_LOAD_IMAGE_COLOR);
       out << "index_x: " << index_x << " index_y: " << index_y << "   -    " << "image_smallcols: " << image_small.cols << "  -  " << "image_smallrows: " << image_small.rows << endl;
       image_small.copyTo(big_image(cv::Rect(border_pixel+index_x*(image_cols+1),border_pixel+index_y*(image_rows+1),image_small.cols, image_small.rows)));
       index_x++;
       if(index_x>images_in_x-1){
           index_y++;
           index_x=0;
       }
       if(index_y>images_in_y-1)
           images_completed = true;
       counter++;
    }

    // last image è stato tolto C360 alla img originale per togliera dalla iterazione
//    QString last_image = "1828_2017-05-10-07-09-46-801_org.jpg.jpg"; // è stato tolto C360 alla img originale per togliera dalla iterazione
//    image_small = imread((save_path + last_image).toStdString() ,CV_LOAD_IMAGE_COLOR);
//    image_small.copyTo(big_image(cv::Rect(border_pixel+index_x*(image_cols+1),border_pixel+index_y*(image_rows+1),image_small.cols, image_small.rows)));
//    index_y++;
//    index_x=0;
//    counter++;

    cout << save_path.toStdString() + "test_small_image_\\big_image_.jpg" << endl;
    imwrite( save_path.toStdString() + "test_small_image_\\big_image_.jpg" , big_image );

    out << "finish !!!" << endl;
    cout << "small images in big image: " << counter << endl;




    double alpha = 0.8; double beta;
    Mat src1 = imread(save_path.toStdString() + "test_small_image_\\big_image_.jpg", CV_LOAD_IMAGE_COLOR);
    Mat src2 = imread(save_path.toStdString() + "test_small_image_\\ms.jpg", CV_LOAD_IMAGE_COLOR);
    Mat src3 = imread(save_path.toStdString() + "test_small_image_\\ms_.jpg", CV_LOAD_IMAGE_COLOR);
    Mat dst;
    beta = ( 1.0 - alpha );
    addWeighted( src1, alpha, src2, beta, 0.0, dst);
    imwrite( save_path.toStdString() + "test_small_image_\\result_.jpg", dst);
    addWeighted( src1, alpha, src3, beta, 0.0, dst);
    imwrite( save_path.toStdString() + "test_small_image_\\result2_.jpg", dst);



    out << "finish !!!" << endl;
    waitKey(0);


    return a.exec();

}



