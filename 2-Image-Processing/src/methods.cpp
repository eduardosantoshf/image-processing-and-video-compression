#include <opencv2/opencv.hpp>
#include <iostream>

#include "methods.hpp"

using namespace std;
using namespace cv;

Mat insert_watermark(Mat initial_image, Mat watermark, double input)
{
    double alpha = 0.5; double beta;
    Mat watermarked_image;

    if (input >= 0 && input <= 1) alpha = input;

    beta = ( 1.0 - alpha );
    addWeighted(initial_image, alpha, watermark, beta, 0.0, watermarked_image);

    return watermarked_image;
}

Mat bgr_to_yuv(Mat initial_image)
{
    Mat yuv_image;
    cvtColor(initial_image, yuv_image, COLOR_BGR2YUV);
    return yuv_image;
}

Mat bgr_to_hsv(Mat initial_image)
{
    Mat hsv_image;
    cvtColor(initial_image, hsv_image, COLOR_BGR2HSV);
    return hsv_image;
}

Mat bgr_to_grayscale(Mat initial_image)
{
    Mat grayscaled_image;
    cvtColor(initial_image, grayscaled_image, COLOR_BGR2GRAY);
    return grayscaled_image;
}

Mat histo_equalization(Mat initial_image)
{
    Mat dst;
    equalizeHist(initial_image, dst);
    return dst;
}


