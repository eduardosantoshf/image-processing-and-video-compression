#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using std::cin;
using std::cout;
using std::endl;
using namespace cv;

int main(void)
{
    double alpha = 0.5; double input;
    Mat initial_image, watermark, watermarked_image;

    cout << "* Enter alpha [0.0-1.0]: ";
    cin >> input;

    initial_image = imread("../images/windowsXP.jpg");

    if (initial_image.data == 0) {cout << "\n\nError..!!\n"; return EXIT_FAILURE;}

    watermark = imread("../images/ubuntuBeaver.jpg");

    if (watermark.data == 0) {cout << "\n\nError..!!\n"; return EXIT_FAILURE;}

    if (input >= 0 && input <= 1) alpha = input;

    watermarked_image = insert_watermark(initial_image, watermark, alpha);

    imshow("Linear Blend", watermarked_image);
    waitKey(0);

}