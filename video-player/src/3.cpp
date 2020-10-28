#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include "methods.cpp"

using namespace std;
using namespace cv;

int main(void)
{
    Mat yuv_src, resized_src;
    Mat new_yuv_channel_1, new_yuv_channel_2;

    Mat src = imread("../images/windowsXP.jpg", IMREAD_COLOR);

    resize(src, resized_src, Size(500, 500));

    if (src.empty()) return EXIT_FAILURE;

    yuv_src = bgr_to_yuv(src);

    vector<Mat> yuv_channels;
    split(yuv_src, yuv_channels);

    
    for (int i = 0; i < yuv_channels[1].size().width; i++) {
        for (int j = 0; j < yuv_channels[1].size().height; i += 2) {
            new_yuv_channel_1.at<uchar> (Point(i, j / 2)) = yuv_channels[1].at<uchar> (Point(i, j));
            new_yuv_channel_2.at<uchar> (Point(i, j / 2)) = yuv_channels[2].at<uchar> (Point(i, j));
        }
    }

    vector<Mat> frameList;
    frameList.push_back(yuv_channels[0]);
    frameList.push_back(new_yuv_channel_1);
    frameList.push_back(new_yuv_channel_2);

    Mat result;

    merge(frameList, result);
    
    imshow("u", result);

    waitKey(0);
    
}