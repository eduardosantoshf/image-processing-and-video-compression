#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include "methods.cpp"

using namespace std;
using namespace cv;

int main(void)
{
    Mat initial_image, yuv_image, hsv_image, final_frame_yuv, final_frame_hsv;
    VideoCapture cap("../videos/akiyo_qcif.y4m");

    while (1) {

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty()) break;

    /*------------ Convert to YUV ------------*/

    final_frame_yuv = bgr_to_yuv(frame);
    vector<Mat> yuv_channels;
    split(final_frame_yuv, yuv_channels);

    /*
    Mat Y = (0.257 * yuv_channels[2]) + (0.504 * yuv_channels[1]) + (0.098 * yuv_channels[0]) + 16;
    Mat U = -(0.148 * yuv_channels[2]) - (0.291 * yuv_channels[1]) + (0.439 * yuv_channels[0]) + 128;
    Mat V = (0.439 * yuv_channels[2]) - (0.368 * yuv_channels[1]) - (0.071 *  yuv_channels[0]) + 128;
    */

    // Display the resulting frames
    imshow("YUV Video", final_frame_yuv);
    imshow("YUV - Y", yuv_channels[0]);
    imshow("YUV - U", yuv_channels[1]);
    imshow("YUV - V", yuv_channels[2]);

    // Press  ESC on keyboard to exit
    char c = (char) waitKey(25);
    if (c==27) break;
    }

    while (1) {

    Mat frame2;
    cap >> frame2;
 
    if (frame2.empty()) break;

    /*------------ Convert to HSV ------------*/

    final_frame_hsv = bgr_to_hsv(frame2);
    imshow("HSV Video", final_frame_hsv);

    vector<Mat> hsv_channels;
    split(final_frame_hsv, hsv_channels);

    imshow("HSV - H", hsv_channels[0]);
    imshow("HSV - S", hsv_channels[1]);
    imshow("HSV - V", hsv_channels[2]);
    
    char c = (char) waitKey(25);
    if (c==27) break;
    }
    
    waitKey(0);
}