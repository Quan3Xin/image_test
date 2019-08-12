#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>

#include <stdio.h>

using namespace cv;
using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
double fibi(int a)
{
    if (a <= 1){
        return a;
    }
    printf("a is -->>>:%d\n", a);
    return fibi(a-1) + fibi(a-2);
}
int main() 
{ 
    //VideoCapture vid("/Users/quan/Desktop/demo.mp4");
    VideoCapture vid(0);
    // int a = 10;
    // double b = fibi(a);
    // cout << b << endl;
    if(!vid.isOpened()){

        cout << "Can't open video"<< endl;
        return -1;
    }
    double fps = vid.get(CAP_PROP_FPS);
    if(fps == 0){
        cout << "Can't get video fps" << endl;
        return -1;
    }
    int delay_ms = 1000.0 / fps;
    
    char fps_str[10];
    int keys = -1;
    int bins = 360;
    int grnHue = 120;
    int hueOffset = 50;
    Mat histogram(bins, 1, CV_32FC1);
    for(int i=0; i < bins; i++){
        histogram.at<float>(i, 0) = (i > grnHue - hueOffset)
            &&
            (i < grnHue + hueOffset)?
            255.0:0.0;
    }
    Rect srchWnd(0,0,100,100);
    TermCriteria criteria(
            TermCriteria::MAX_ITER + TermCriteria::EPS,
            20,
            1.0 );
    while(keys != ' '){
        sprintf(fps_str, "%0.2f", fps);
        String fpsString("FPS:");
        fpsString+=fps_str;
        Mat frame;
        vid >> frame;
        cv::putText(
                frame,
                fpsString,
                Point(50,50),
                FONT_HERSHEY_SIMPLEX,
                2,
                Scalar(130,0,2)
               );
        if(frame.empty()){
            break;
        }
        switch(keys){
            case 'k': applyColorMap(frame, frame, COLORMAP_JET);
                      break;
            case 'j': applyColorMap(frame, frame, COLORMAP_HOT);
                      break;
        }
        
        // applyColorMap(frame, frame, COLORMAP_JET);
        imshow("Video", frame);
        int k =waitKey(10);
        if(k >0) {keys = k;}


    }
    vid.release();
    /*
    Mat images;
    String image_url = "/Users/quan/Developer/C/picture_ex/123.png";
    Mat out_images = imread(image_url, IMREAD_GRAYSCALE);
    
    namedWindow("test", 0);
    namedWindow("Fifed", WINDOW_AUTOSIZE);
    imshow("test", out_images);
    resizeWindow("test", 480, 360);

    imwrite("output_image.png", out_images);
    moveWindow("test", 600, 360);
    moveWindow("Fifed", out_images.cols + 10, 360);
    Mat channels[3];
    split(out_images, channels);
    imshow("channels", channels[1]);
    


    waitKey(0);

    cout <<"end programming\n"<<endl;
    

    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};
    string trackerType = trackerTypes[2];
    Ptr<Tracker> tracker = TrackerKCF::create();

    VideoCapture capture(0);
    if (!capture.isOpened()){
        cout <<"Could not found camera"<<endl;
        return -1;
    }
    capture.get(CAP_PROP_FPS);
    Rect2d bbox(287, 23, 86, 320);
    Mat frame;
    rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
    imshow("Tracking",frame);
    tracker->init(frame, bbox);


    while(capture.read(frame))
    {
        double timer = (double)getTickCount();
        bool ok = tracker->update(frame, bbox);
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
        if( ok ){
            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
        } else {
        }

        

    }
    */
    return 0;

}
