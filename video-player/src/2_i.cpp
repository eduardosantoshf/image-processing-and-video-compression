#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using namespace std;
using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;
int lowThreshold = 0;
const int max_lowThreshold = 20;
const int ratio2 = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";

static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(3,3) );
    canny_edge_detector( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio2, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}

int main(void)
{
    VideoCapture cap("../videos/akiyo_qcif.y4m");

    while (1) {

        Mat frame;
        // Capture frame-by-frame
        cap >> frame;
    
        // If the frame is empty, break immediately
        if (frame.empty()) break;

        resize(frame, src, Size(500, 500));
        
        dst.create( src.size(), src.type() );
        cvtColor( src, src_gray, COLOR_BGR2GRAY );
        namedWindow( window_name, WINDOW_AUTOSIZE );
        createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
        CannyThreshold(0, 0);
            
        // Press  ESC on keyboard to exit
        char c = (char) waitKey(25);
        if (c==27) break;
    }
    waitKey(0);
}