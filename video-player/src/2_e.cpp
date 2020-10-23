#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using namespace std;
using namespace cv;

int main(void)
{
    Mat initial_image, gaussian_blured_image, blured_image;
    int k1 = 1, k2 = 1;
    int b1 = 1, b2 = 1;
    double input1, input2, input3, input4;

    VideoCapture cap("../videos/akiyo_qcif.y4m");

    cout << "Gaussian kernel size = k1 x k2\n";

    cout << "Enter k1: ";
    cin >> input1;

    if (input1 > 0 && (int)input1 == input1) {
        if ((int)input1 % 2 != 0) k1 = input1;
    }

    cout << "Enter k2: ";
    cin >> input2;

    if (input2 > 0 && (int)input2 == input2) {
        if ((int)input2 % 2 != 0) k2 = input2;
    }

    cout << "Blurring kernel size = b1 x b2\n";

    cout << "Enter b1: ";
    cin >> input3;

    if (input3 > 0) b1 = input3;

    cout << "Enter b2: ";
    cin >> input4;

    if (input4 > 0) b2 = input4;

    while (1) {

    Mat frame, resized_frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
        break;

    resize(frame, resized_frame, Size(500, 500));

    /*------------ Gaussian Blur ------------*/

    gaussian_blured_image = gaussian_blur(resized_frame, k1, k2);

    imshow("Gaussian Blured Video", gaussian_blured_image);

    // Press  ESC on keyboard to exit
    char c = (char) waitKey(25);
    if (c==27) break;
    }

    while (1) {

    Mat frame2, resized_frame2;
    cap >> frame2;
 
    if (frame2.empty()) break;

    resize(frame2, resized_frame2, Size(500, 500));

    /*--------------- Blur ---------------*/

    blured_image = regular_blur(resized_frame2, b1, b2);

    imshow("Blured Video", blured_image);

    char c = (char) waitKey(25);
    if (c==27) break;
    }

    waitKey(0);
}