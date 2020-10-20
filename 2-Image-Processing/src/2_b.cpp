#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include "methods.cpp"

using namespace std;
using namespace cv;

int main(void)
{
    Mat initial_image, yuv_image, hsv_image;
    initial_image = imread("../images/windowsXP.jpg");
    //imshow("Original image", initial_image);

    /*------------ Convert to YUV ------------*/

    yuv_image = convert_to_yuv(initial_image);
    //imshow("YUV image", yuv_image);

    vector<Mat> yuv_channels;
    split(yuv_image, yuv_channels);

    /*
    Mat Y = (0.257 * yuv_channels[2]) + (0.504 * yuv_channels[1]) + (0.098 * yuv_channels[0]) + 16;
    Mat U = -(0.148 * yuv_channels[2]) - (0.291 * yuv_channels[1]) + (0.439 * yuv_channels[0]) + 128;
    Mat V = (0.439 * yuv_channels[2]) - (0.368 * yuv_channels[1]) - (0.071 *  yuv_channels[0]) + 128;
    */
    imshow("YUV - Y", yuv_channels[0]);
    imshow("YUV - U", yuv_channels[1]);
    imshow("YUV - V", yuv_channels[2]);

    /*------------ Convert to HSV ------------*/

    hsv_image = convert_to_hsv(initial_image);
    //imshow("HSV image", hsv_image);

    vector<Mat> hsv_channels;
    split(hsv_image, hsv_channels);

    imshow("HSV - H", hsv_channels[0]);
    imshow("HSV - S", hsv_channels[1]);
    imshow("HSV - V", hsv_channels[2]);
    
    waitKey(0);
}