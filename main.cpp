#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QTime>
#include <QString>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <QFile>

//#define DEBUG



#define IMAGES_ALREADY_AVAILABLE

using namespace std;
using namespace cv;

Point capturePoint;// point coordinates, global variable;
bool continue_procedure = false;
void CallBackFunc(int event, int x, int y, int d, void *ptr)
{
    if( event == EVENT_LBUTTONDOWN ){
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        capturePoint = Point(x,y);
        continue_procedure = true;
    }
}

int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);

    QTextStream out(stdout);
    Mat image;
    Mat image_new;
    Mat image_small_clipped;
    Rect roi;
    int count = 0;
    int rand_number_from0tothis = 10000;
    const int images_in_x = 40;
    const int images_in_y =25;
    const int border_pixel = 257;    //   257:44(mm) = 170:x(mm) -> 30mm
    const int border_pixel_new = 170;  // 170:30(mm) = 12:x(mm) -> 2.1mm
    const int circle_center_x = 58;
    const int circle_center_y = 58;
    const int circle_center_x_new = 30;
    const int circle_center_y_new = 30;
    const int dimension_x =(int)150;
    const int dimension_y =(int)150;
    const QString last_year = "2017";
    const QString last_month = "12";
    const QString last_day = "27";
    QString save_path = "C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\saved_image_\\";
    QString save_path2 = "C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\saved_image_new\\";
    QDirIterator it("C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\Google Foto", QStringList() << "*_org.jpg", QDir::Files, QDirIterator::Subdirectories);
    //QDirIterator it("C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\test_image_", QStringList() << "*_org.jpg", QDir::Files, QDirIterator::Subdirectories);
    int anno_2015[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_2016[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_2017[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_2018[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int anno_have_to_be[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

#ifndef IMAGES_ALREADY_AVAILABLE
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

/////////////////NEW/////NEW////////////////
/*        image_small_clipped = image.clone();
        namedWindow( "image_small_clipped", WINDOW_AUTOSIZE );
        //set the callback function for any mouse event
        setMouseCallback("image_small_clipped", CallBackFunc, NULL);
        imshow( "image_small_clipped", image_small_clipped );
        cv::waitKey(0);
        roi.x = capturePoint.x - dimension_x/2;
        roi.y = capturePoint.y - dimension_y/2;
        roi.width = dimension_x;
        roi.height = dimension_y;
        cout << "Point: "  << roi.x << ":" << roi.y << " width: " << roi.width << " height: " << roi.height << endl;
        if((roi.x + roi.width)>image_small_clipped.cols-1) roi.x = image_small_clipped.cols - roi.width;
        if((roi.y + roi.height)>image_small_clipped.rows-1) roi.y = image_small_clipped.rows - roi.height;
        if(roi.x <0) roi.x = 0;
        if(roi.y <0) roi.y = 0;
        cv::Mat crop = image_small_clipped(roi); /* Crop the original image to the defined ROI */
////////////////////////////////////////////

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
            circle(image, Point(circle_center_x,circle_center_y), 7, CV_RGB(0,0,255), -1, 8, 0);
            circle(image, Point(circle_center_x,circle_center_y), 4, CV_RGB(255,255,255), -1, 8, 0);
            circle(image, Point(circle_center_x,circle_center_y), 7, CV_RGB(0,0,0), 1, 8, 0);
            line(image, Point(circle_center_x-3,circle_center_y), Point(circle_center_x+3,circle_center_y), CV_RGB(0,0,0), 1, 8, 0);
            line(image, Point(circle_center_x,circle_center_y-3), Point(circle_center_x,circle_center_y+3), CV_RGB(0,0,0), 1, 8, 0);

      /*      cvtColor(crop,crop,CV_BGR2GRAY);
            cvtColor(crop,crop,CV_GRAY2BGR);
            putText(crop, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " " + hour.toStdString() + ":" +
                    minute.toStdString() + ":" + second.toStdString(), Point(11, 145), FONT_HERSHEY_COMPLEX_SMALL, 0.5, CV_RGB(255,255,255), 0.9);
            circle(crop, Point(circle_center_x_new,circle_center_y_new), 7, CV_RGB(0,0,255), -1, 8, 0);
            circle(crop, Point(circle_center_x_new,circle_center_y_new), 4, CV_RGB(255,255,255), -1, 8, 0);
            circle(crop, Point(circle_center_x_new,circle_center_y_new), 7, CV_RGB(0,0,0), 1, 8, 0);
            line(crop, Point(circle_center_x_new-3,circle_center_y_new), Point(circle_center_x_new+3,circle_center_y_new), CV_RGB(0,0,0), 1, 8, 0);
            line(crop, Point(circle_center_x_new,circle_center_y_new-3), Point(circle_center_x_new,circle_center_y_new+3), CV_RGB(0,0,0), 1, 8, 0);
*/
#endif

#ifndef DEBUG
            putText(image, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " " + hour.toStdString() + ":" +
                    minute.toStdString() + ":" + second.toStdString(), Point(70/*100*/, 250), FONT_HERSHEY_COMPLEX_SMALL, /*0.6*/0.7, CV_RGB(255,255,255), /*0.8*/0.9);
            circle(image, Point(circle_center_x,circle_center_y), 6, CV_RGB(255,255,255), -1, 8, 0);
            //circle(image, Point(circle_center_x,circle_center_y), 4, CV_RGB(0,0,0), -1, 8, 0);
            //circle(image, Point(circle_center_x,circle_center_y), 6, CV_RGB(0,0,0), 1, 8, 0);
            line(image, Point(circle_center_x-5,circle_center_y), Point(circle_center_x+5,circle_center_y), CV_RGB(0,0,0), 1, 8, 0);
            line(image, Point(circle_center_x,circle_center_y-5), Point(circle_center_x,circle_center_y+5), CV_RGB(0,0,0), 1, 8, 0);
            cvtColor(image,image,CV_BGR2GRAY);
            cvtColor(image,image,CV_GRAY2BGR);

      /*      putText(crop, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " " + hour.toStdString() + ":" + minute.toStdString() + ":" + second.toStdString(), Point(10, 145), FONT_HERSHEY_COMPLEX_SMALL, 0.5, CV_RGB(255,255,255), 0.9);
            circle(crop, Point(circle_center_x_new,circle_center_y_new), 6, CV_RGB(255,255,255), -1, 8, 0);
            circle(crop, Point(circle_center_x_new,circle_center_y_new), 4, CV_RGB(0,0,0), -1, 8, 0);
            circle(crop, Point(circle_center_x_new,circle_center_y_new), 6, CV_RGB(0,0,0), 1, 8, 0);
            line(crop, Point(circle_center_x_new-5,circle_center_y_new), Point(circle_center_x_new+5,circle_center_y_new), CV_RGB(0,0,0), 1, 8, 0);
            line(crop, Point(circle_center_x_new,circle_center_y_new-5), Point(circle_center_x_new,circle_center_y_new+5), CV_RGB(0,0,0), 1, 8, 0);
            cvtColor(crop,crop,CV_BGR2GRAY);
            cvtColor(crop,crop,CV_GRAY2BGR);*/
#endif
        }
        else{
#ifdef DEBUG
            cvtColor(image,image,CV_BGR2GRAY);
            cvtColor(image,image,CV_GRAY2BGR);
            //cvtColor(crop,crop,CV_BGR2GRAY);
            //cvtColor(crop,crop,CV_GRAY2BGR);
#endif
            cout << "no pills" << endl;
            putText(image, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " --:--:--", Point(70, 250), FONT_HERSHEY_COMPLEX_SMALL, 0.7, CV_RGB(255,255,255), 0.9);
            //putText(crop, year.toStdString() + "-" + month.toStdString() + "-" + day.toStdString() + " --:--:--", Point(10, 145), FONT_HERSHEY_COMPLEX_SMALL, 0.5, CV_RGB(255,255,255), 0.9);
#ifndef DEBUG
            cvtColor(image,image,CV_BGR2GRAY);
            cvtColor(image,image,CV_GRAY2BGR);
            //cvtColor(crop,crop,CV_BGR2GRAY);
            //cvtColor(crop,crop,CV_GRAY2BGR);
#endif
        }



///////////////////////////////////////////////////////////////////////////
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());
        int randomValue = qrand() % rand_number_from0tothis;
        if(count>1000) randomValue += 20000;
        QString number = QString::number(randomValue);        
        imwrite( (save_path + number  + it.fileName()).toStdString() + ".jpg" , image );
        //imwrite( (save_path2 + number  + it.fileName()).toStdString() + ".jpg" , crop );


        count++;
        cout << "images: " << count << endl;
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

#endif


    // create  big image
    int counter=0;
    bool images_completed = false;
    Mat image_small_new;
    image_new = imread(save_path2.toStdString() + "test_small_image_new\\test_small_image_.jpg",CV_LOAD_IMAGE_COLOR);
    cout << save_path2.toStdString() << "test_small_image_new\\test_small_image_.jpg" << endl;
    int image_cols_new = image_new.cols;
    int image_rows_new = image_new.rows;
    out << "image_cols_new: " << image_cols_new << "  -  " << "image_rows_new: " << image_rows_new << endl;

    int index_x=0, index_y=0;
    Mat big_image_new(2*border_pixel_new+(images_in_y*image_rows_new)+(images_in_y-1)*1, 2*border_pixel_new+(images_in_x*image_cols_new)+(images_in_x-1)*1, CV_8UC3, Scalar(255,255,255));


    // first image è stato tolto C360 alla img originale per togliera dalla iterazione
    QString first_image_new = "2082----_2015-04-02-08-37-49-922_org.jpg.jpg"; //
    cout << (save_path2 + first_image_new).toStdString() << endl;
    image_small_new = imread((save_path2 + first_image_new).toStdString() ,CV_LOAD_IMAGE_COLOR);
    image_small_new.copyTo(big_image_new(cv::Rect(border_pixel_new+index_x*(image_cols_new+1),border_pixel_new+index_y*(image_rows_new+1),image_small_new.cols, image_small_new.rows)));
    index_x++;
    counter++;

    QString filename_images_available = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\20_09_2018_pastiglie\\images.txt";
    QFile file(filename_images_available);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);

    QDirIterator it_bn_images_new("C:\\Users\\Fabio Roncato\\Documents\\Photo\\Cell\\Takeout\\saved_image_new", QStringList() << "*C360*.jpg", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it_bn_images_new.hasNext() && images_completed==false){
        it_bn_images_new.next();
        int index = it_bn_images_new.fileName().indexOf('_');
        QString year = it_bn_images_new.fileName().mid(index+1,4);
        QString month = it_bn_images_new.fileName().mid(index+6,2);
        QString day = it_bn_images_new.fileName().mid(index+9,2);
        out << year << " " << month << " " << day << endl;
        stream << endl << year << "-" << month << "-" << day << ",  " << it_bn_images_new.fileName();

        QDate last_day, current_image_day;
      // here we have to set the two QDateTime object to compare them (see below)
        last_day.setDate(2017,12,26);
        current_image_day.setDate(year.toInt(),month.toInt(),day.toInt());

        if(current_image_day < last_day){ // here I want verify the actual image is part of the first 1000 images I am using
            stream << "*";
            cout << "---" << it_bn_images_new.fileName().toStdString() << "  " << endl;
            image_small_new = imread(it_bn_images_new.filePath().toStdString(),CV_LOAD_IMAGE_COLOR);
            out << "index_x: " << index_x << " index_y: " << index_y << "   -    " << "image_smallcols: " << image_small_new.cols << "  -  " << "image_smallrows: " << image_small_new.rows << endl;
            image_small_new.copyTo(big_image_new(cv::Rect(border_pixel_new+index_x*(image_cols_new+1),border_pixel_new+index_y*(image_rows_new+1),image_small_new.cols, image_small_new.rows)));

            index_x++;
            if(index_x>images_in_x-1){
                index_y++;
                index_x=0;
            }
            if(index_y>images_in_y-1)
                images_completed = true;
            counter++;
        }
        cout << "index_x: " << index_x << " - index_y: " << index_y << endl;
    }
    file.close();

    // a last image è stato tolto C360 alla img originale per togliera dalla iterazione
    QString last_image_new = "3212----_2017-12-27-07-08-29-853_org.jpg.jpg"; // è stato tolto C360 alla img originale per togliera dalla iterazione
    image_small_new = imread((save_path2 + last_image_new).toStdString() ,CV_LOAD_IMAGE_COLOR);
    image_small_new.copyTo(big_image_new(cv::Rect(border_pixel_new+index_x*(image_cols_new+1),border_pixel_new+index_y*(image_rows_new+1),image_small_new.cols, image_small_new.rows)));
    index_y++;
    index_x=0;
    counter++;


    cout << save_path2.toStdString() + "test_small_image_new\\big_image_.jpg" << endl;
    imwrite(save_path2.toStdString() + "test_small_image_new\\big_image_.jpg" , big_image_new );

    out << "finish !!!" << endl;
    cout << "small images in big image: " << counter << endl;




    double alpha = 0.8; double beta;
    Mat src1 = imread(save_path2.toStdString() + "test_small_image_new\\big_image_.jpg", CV_LOAD_IMAGE_COLOR);
    Mat src3 = imread(save_path2.toStdString() + "test_small_image_new\\ms_.jpg", CV_LOAD_IMAGE_COLOR);
    Mat dst;
    beta = ( 1.0 - alpha );
    addWeighted( src1, alpha, src3, beta, 0.0, dst);
    imwrite( save_path2.toStdString() + "test_small_image_new\\result2_new.jpg", dst);


    unsigned char R_img_text;
    unsigned char G_img_text;
    unsigned char B_img_text;
    unsigned char R_img_to_modify;
    unsigned char G_img_to_modify;
    unsigned char B_img_to_modify;
    unsigned char R_img_to_modify_a;
    unsigned char R_img_to_modify_b;
    unsigned char R_img_to_modify_c;
    unsigned char R_img_to_modify_d;
    unsigned char R_img_to_modify_e;
    cv::Mat img_text = cv::imread(save_path2.toStdString() + "test_small_image_new\\ms_.jpg");
    cv::Mat img_to_modify = cv::imread(save_path2.toStdString() + "test_small_image_new\\big_image_.jpg");
    cv::Mat original_clone = img_to_modify.clone();
    cout << "ms_.jpg: " << img_text.rows << "  -  " << img_text.cols << endl;
    cout << "big_image_.jpg: " << img_to_modify.rows << "  -  " << img_to_modify.cols << endl;

    for(int y=1; y<img_text.rows-1; y++){
        for(int x=1; x<img_text.cols-1; x++){
            // You can now access the pixel value with cv::Vec3b
            R_img_text = img_text.at<cv::Vec3b>(y,x)[2];
            G_img_text = img_text.at<cv::Vec3b>(y,x)[1];
            B_img_text = img_text.at<cv::Vec3b>(y,x)[0];
            R_img_to_modify = img_to_modify.at<cv::Vec3b>(y,x)[2];
            G_img_to_modify = img_to_modify.at<cv::Vec3b>(y,x)[1];
            B_img_to_modify = img_to_modify.at<cv::Vec3b>(y,x)[0];
            R_img_to_modify_a = img_to_modify.at<cv::Vec3b>(y-1,x)[2]; // this point to localize the x(croix) present on image
            R_img_to_modify_b = img_to_modify.at<cv::Vec3b>(y,x-1)[2];
            R_img_to_modify_c = img_to_modify.at<cv::Vec3b>(y+1,x)[2];
            R_img_to_modify_d = img_to_modify.at<cv::Vec3b>(y,x+1)[2];
            R_img_to_modify_e= img_to_modify.at<cv::Vec3b>(y+1,x+1)[2];




            if(R_img_text==0 && G_img_text==0 && B_img_text==0){
                if((R_img_to_modify < 15) && (R_img_to_modify_a<15) && (R_img_to_modify_b<15) && (R_img_to_modify_c<15) && (R_img_to_modify_d<15) && (R_img_to_modify_e>230)){
                    circle(original_clone, Point(x,y), 5, CV_RGB(255,255,255), -1, 8, 0);
                    line(original_clone, Point(x-3,y), Point(x+3,y), CV_RGB(0,0,0), 1, 8, 0);
                    line(original_clone, Point(x,y-3), Point(x,y+3), CV_RGB(0,0,0), 1, 8, 0);
                }
            }
            else if(R_img_text!=0 && G_img_text!=0 && B_img_text!=0){
                if((R_img_to_modify < 15) && (R_img_to_modify_a<15) && (R_img_to_modify_b<15) && (R_img_to_modify_c<15) && (R_img_to_modify_d<15) && (R_img_to_modify_e>230)){
                    circle(original_clone, Point(x,y), 5, CV_RGB(255,255,0), -1, 8, 0);
                    line(original_clone, Point(x-3,y), Point(x+3,y), CV_RGB(0,0,0), 1, 8, 0);
                    line(original_clone, Point(x,y-3), Point(x,y+3), CV_RGB(0,0,0), 1, 8, 0);
                }
            }
        }
    }
    imwrite( save_path2.toStdString() + "test_small_image_new\\fax_.jpg", original_clone);
    out << "temporary finish !!!" << endl;
    waitKey(0);

    return a.exec();
}



