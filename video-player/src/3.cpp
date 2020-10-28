#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include "methods.cpp"

using namespace std;
using namespace cv;

int main(void)
{
    Mat yuv_src, resized_src;

    Mat src = imread("../images/windowsXP.jpg", IMREAD_COLOR);
    
    resize(src, resized_src, Size(500, 500));

    if (src.empty()) return EXIT_FAILURE;

    //yuv_src = bgr_to_yuv(src);
    cvtColor(src, yuv_src, COLOR_RGB2YUV);

    vector<Mat> yuv_channels;
    split(yuv_src, yuv_channels);

    /*------------------ YUV422 ------------------*/

    Mat new_yuv_channel_1(Size(yuv_channels[1].size().width, yuv_channels[1].size().height / 2), 0);
    Mat new_yuv_channel_2(Size(yuv_channels[1].size().width, yuv_channels[1].size().height / 2), 0);
    
    for (int i = 0; i < yuv_channels[1].size().width; i++) {
        for (int j = 0; j < yuv_channels[1].size().height; j += 2) {
            new_yuv_channel_1.at<uchar> (Point(i, j / 2)) = yuv_channels[1].at<uchar> (Point(i, j));
            new_yuv_channel_2.at<uchar> (Point(i, j / 2)) = yuv_channels[2].at<uchar> (Point(i, j));
        }
    }

    Mat final_channel_0 = yuv_channels[0];
    Mat final_channel_1(Size(yuv_channels[1].size().width, yuv_channels[1].size().height), 0);
    Mat final_channel_2(Size(yuv_channels[1].size().width, yuv_channels[1].size().height), 0);
    
    for (int k = 0; k < yuv_channels[1].size().width; k++) {
        for (int l = 0; l < yuv_channels[1].size().width; l++) {
            final_channel_1.at<uchar> (Point(k, l)) = new_yuv_channel_1.at<uchar> (Point(k, l / 2));
            final_channel_2.at<uchar> (Point(k, l)) = new_yuv_channel_2.at<uchar> (Point(k, l / 2));
        }
    }
    
    vector<Mat> frameList;
    frameList.push_back(final_channel_0);
    frameList.push_back(final_channel_1);
    frameList.push_back(final_channel_2);

    Mat result_422;

    merge(frameList, result_422);

    cvtColor(result_422, result_422, COLOR_YUV2RGB);
    
    imshow("YUV422", result_422);

    /*------------------ YUV420 ------------------*/

    Mat new_yuv_channel_1_420(Size(yuv_channels[1].size().width / 2, yuv_channels[1].size().height / 2), 0);
    Mat new_yuv_channel_2_420(Size(yuv_channels[1].size().width / 2, yuv_channels[1].size().height / 2), 0);

    for (int i = 0; i < yuv_channels[1].size().width; i += 2) {
        for (int j = 0; j < yuv_channels[1].size().height; j += 2) {
            new_yuv_channel_1_420.at<uchar> (Point(i / 2, j / 2)) = yuv_channels[1].at<uchar> (Point(i, j));
            new_yuv_channel_2_420.at<uchar> (Point(i / 2, j / 2)) = yuv_channels[2].at<uchar> (Point(i, j));
        }
    }
    
    Mat final_channel_0_420 = yuv_channels[0];
    Mat final_channel_1_420(Size(yuv_channels[1].size().width, yuv_channels[1].size().height), 0);
    Mat final_channel_2_420(Size(yuv_channels[1].size().width, yuv_channels[1].size().height), 0);

    for (int i = 0; i < yuv_channels[1].size().width; i++) {
        for (int j = 0; j < yuv_channels[1].size().width; j++) {
            final_channel_1_420.at<uchar> (Point(i, j)) = new_yuv_channel_1_420.at<uchar> (Point(i / 2, j / 2));
            final_channel_2_420.at<uchar> (Point(i, j)) = new_yuv_channel_2_420.at<uchar> (Point(i / 2, j / 2));
        }
    }
    
    vector<Mat> frameList_420;
    frameList_420.push_back(final_channel_0_420);
    frameList_420.push_back(final_channel_1_420);
    frameList_420.push_back(final_channel_2_420);

    Mat result_420;

    merge(frameList, result_420);

    cvtColor(result_420, result_420, COLOR_YUV2RGB);
    
    imshow("YUV420", result_420);
    
    waitKey(0);
    
}