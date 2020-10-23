#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using namespace std;
using namespace cv;

int main(void)
{
    Mat initial_image, thresholded_image;
    int t_type_input, t_value_input;

    VideoCapture cap("../videos/akiyo_qcif.y4m");

    cout << "Thresholding type? \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted\n";
    cin >> t_type_input;

    cout << "Thresholding value:";
    cin >> t_value_input;

    while (1) {

    Mat frame, resized_frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
        break;

    resize(frame, resized_frame, Size(500, 500));

    imshow("Original Video", resized_frame);

    thresholded_image = threshold_image(resized_frame, t_value_input, t_type_input);

    imshow("Thresholded Video", thresholded_image);

    // Press  ESC on keyboard to exit
    char c = (char) waitKey(25);
    if (c==27) break;
    }

    waitKey(0);
}