#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using namespace cv;

int main(void)
{
    double alpha = 0.5; double input;
    Mat watermark, watermarked_image;
    Mat resized_frame; Mat resized_image;

    std::cout << "* Enter alpha [0.0-1.0]: ";
    std::cin >> input;

    if (input >= 0 && input <= 1) alpha = input;

    VideoCapture cap("../videos/akiyo_qcif.y4m"); 

    watermark = imread("../images/ubuntuBeaver.jpg");

    if (watermark.data == 0) {std::cout << "\n\nError..!!\n"; return EXIT_FAILURE;}

    resize(watermark, resized_image, Size(256, 256));

    while (1) {

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
        break;

    resize(frame, resized_frame, Size(256, 256));

    watermarked_image = insert_watermark(resized_frame, resized_image, alpha);

    // Display the resulting frame
    imshow("Watermarked Video", watermarked_image);

    // Press  ESC on keyboard to exit
    char c = (char) waitKey(25);
    if (c==27) break;
    }

    waitKey(0);
}