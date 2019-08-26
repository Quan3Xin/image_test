
#include <iostream>
 
using namespace std;
 
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
 
using namespace cv;
 
int main()
{
	Mat img = imread("/Users/quan/Downloads/1536875932666.jpg");
	float radius = img.cols > img.rows ? (img.rows / 3) : (img.cols / 3);
 
 
	VideoCapture cap;
	// cap.open("/Users/quan/Desktop/男人和女人的区别.mp4");
	cap.open(0);
	Mat frame, blob;

    double fps = 30.0;
    int frame_width  = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height  = cap.get(CAP_PROP_FRAME_HEIGHT);
    
    string oriFilename = "./output.avi";
    string filename = "./oriVideo.avi";
    int keys = -1;
    
    VideoWriter video(filename,
            VideoWriter::fourcc('M','J','P','G'), fps,
            Size(frame_width, frame_height)); 
    VideoWriter oriVideo(oriFilename,
            VideoWriter::fourcc('M','J','P','G'), fps,
            Size(frame_width, frame_height)); 
    
    while(keys != ' '){

        cap >> frame;
        
        oriVideo.write(frame);
        const double exponential_e = exp(1.0);
        /** EDgES **/
        // Apply median filter to remove possible noise
        Mat imgMedian;
        medianBlur(frame, imgMedian, 7);
        // Detect edges with canny
        Mat imgCanny;
        Canny(imgMedian, imgCanny, 50, 150);
        // Dilate the edges
        Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
        dilate(imgCanny, imgCanny, kernel);
     
        // Scale edges values to 1 and invert values
        imgCanny = imgCanny / 255;
        imgCanny = 1 - imgCanny;
        // Use float values to allow multiply between 0 and 1
        Mat imgCannyf;
        imgCanny.convertTo(imgCannyf, CV_32FC3);
        // Blur the edgest to do smooth effect
        blur(imgCannyf, imgCannyf, Size(5, 5));
     
        /** COLOR **/
        // Apply bilateral filter to homogenizes color
        Mat imgBF;
        bilateralFilter(frame, imgBF, 9, 150.0, 150.0);
        // truncate colors
        Mat result = imgBF / 25;
        result = result * 25;
        /** MERgES COLOR + EDgES **/
        // Create a 3 channles for edges
        Mat imgCanny3c;
        Mat cannyChannels[] = { imgCannyf, imgCannyf, imgCannyf };
        merge(cannyChannels, 3, imgCanny3c);
        // Convert color result to float
        Mat resultf;
        result.convertTo(resultf, CV_32FC3);
        // Multiply color and edges matrices
    //	cout << imgCanny3c << endl;
        multiply(resultf, imgCanny3c, resultf);
    //	cout << resultf << endl;
        // convert to 8 bits color
        resultf.convertTo(result, CV_8UC3);
        video.write(result);
        // Show image
        
        switch(keys){
            case 'k': imshow("Result", result);
                      break;
            case 'j': imshow("Origin", frame);
                      break;
            default: imshow("Origin", frame);
                     break;
        }
        //imshow("Result", result);
        
        int k =waitKey(10);
        if(k >0) {keys = k;}
    }
    //waitKey(0);
    cap.release();
    video.release();


 
 
    return 0;

}
