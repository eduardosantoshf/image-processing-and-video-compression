#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat image,src, src_gray;
    Mat grad_sobel;
    Mat grad_scharr;
    Mat grad_laplacian;
    int ksize = 1;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    VideoCapture cap("../videos/akiyo_qcif.y4m");

    while (1)
    {
        Mat frame, resized_frame;
        // Capture frame-by-frame
        cap >> frame;
    
        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        resize(frame, resized_frame, Size(500, 500));
        // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
        GaussianBlur(resized_frame, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
        // Convert the image to grayscale
        cvtColor(src, src_gray, COLOR_BGR2GRAY);

        /*--------------- Sobel Derivative ---------------*/

        Mat grad_x_sobel, grad_y_sobel;
        Mat abs_grad_x_sobel, abs_grad_y_sobel;
        Sobel(src_gray, grad_x_sobel, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(src_gray, grad_y_sobel, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x_sobel, abs_grad_x_sobel);
        convertScaleAbs(grad_y_sobel, abs_grad_y_sobel);
        addWeighted(abs_grad_x_sobel, 0.5, abs_grad_y_sobel, 0.5, 0, grad_sobel);
        imshow("Sobel Video", grad_sobel);

        /*--------------- Scharr Derivative ---------------*/

        Mat grad_x_scharr, grad_y_scharr;
        Mat abs_grad_x_scharr, abs_grad_y_scharr;
        Scharr(src_gray, grad_x_scharr, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
        Scharr(src_gray, grad_y_scharr, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x_scharr, abs_grad_x_scharr);
        convertScaleAbs(grad_y_scharr, abs_grad_y_scharr);
        addWeighted(abs_grad_x_scharr, 0.5, abs_grad_y_scharr, 0.5, 0, grad_scharr);
        imshow("Scharr Video", grad_scharr);

        /*-------------- Laplacian Derivative --------------*/

        Mat grad_laplacian;
        Mat abs_grad_laplacian;
        Mat result;
        Laplacian(src_gray, grad_laplacian, ddepth, ksize, scale, delta, BORDER_DEFAULT);
        Laplacian(src_gray, grad_laplacian, CV_16S, 5);
        convertScaleAbs(grad_laplacian, result, 1);
        imshow("Laplacian Video", result);
        
        // Press  ESC on keyboard to exit
        char c = (char) waitKey(25);
        if (c==27) break;
    }

    waitKey(0);
}