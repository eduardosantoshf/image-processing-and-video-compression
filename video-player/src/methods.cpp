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

Mat threshold_image(Mat initial_image, int t_value, int t_type)
{
    Mat thresholded_image;
    threshold(initial_image, thresholded_image, t_value, 255, t_type);
    return thresholded_image;
}

Mat morphological_operation(Mat initial_image, Mat result_image, int op, Mat elem)
{
    morphologyEx(initial_image, result_image, op, elem);
    return result_image;
}

Mat sobel_derivative(Mat initial_image, int ddepth, int x, int y, int ksize, int scale, int delta)
{
    Mat grad;
    Sobel(initial_image, grad, ddepth, x, y, ksize, scale, delta, BORDER_DEFAULT);
    return grad;
}

Mat scharr_derivative(Mat initial_image, int ddepth, int x, int y, int scale, int delta)
{
    Mat grad;
    Scharr(initial_image, grad, ddepth, x, y, scale, delta, BORDER_DEFAULT);
    return grad;
}

Mat laplacian_derivative(Mat initial_image)
{
    Mat grad;
    Laplacian(initial_image, grad, CV_16S, 5);
    return grad;
}

Mat canny_edge_detector(Mat detected_edges, Mat detected_edges2, int lowThreshold, int ratio2, int kernel_size)
{
    Canny( detected_edges, detected_edges2, lowThreshold, lowThreshold * ratio2, kernel_size);
    return detected_edges2;
}
