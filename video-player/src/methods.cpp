#include <opencv2/opencv.hpp>
#include <iostream>
#include "methods.hpp"

using namespace std;
using namespace cv;

Mat insert_watermark(Mat initial_image, Mat watermark, double beta)
{
    double alpha;
    Mat watermarked_image;

    alpha = (1.0 - beta);
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

Mat gaussian_blur(Mat initial_image, int k1, int k2)
{
    Mat filtered_image;
    GaussianBlur(initial_image, filtered_image, Size(k1, k2), 0, 0);
    return filtered_image;
}

Mat regular_blur(Mat initial_image, int b1, int b2)
{
    Mat blured_image;
    blur(initial_image, blured_image, Size(b1, b2));
    return blured_image;
}
/*
Mat threshold_image(Mat initial_image, int t_value, int t_type)
{
    Mat threshold_image;
    threshold(initial_image, threshold_image, t_value, 255, t_type);
    return threshold_image;
}
*/

