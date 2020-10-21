#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using namespace cv;

int main(void)
{
    Mat initial_image, grayscaled_image, dst, resized_frame;
    VideoCapture cap("../videos/akiyo_qcif.y4m"); 

    while (1) {

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
        break;

    resize(frame, resized_frame, Size(500, 500));

    // Display the resulting frame
    imshow("Original Video", resized_frame);

    /*------------ Convert to grayscale ------------*/

    grayscaled_image = bgr_to_grayscale(resized_frame);

    imshow("Grayscaled Video", grayscaled_image);

    /*----------- Histogram Equalization -----------*/

    dst = histo_equalization(grayscaled_image);

    imshow("Equalized Video", dst);

    // Press  ESC on keyboard to exit
    char c = (char) waitKey(25);
    if (c==27) break;
    }

    waitKey(0);
}