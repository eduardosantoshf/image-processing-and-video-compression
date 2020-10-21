#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include "methods.cpp"

using namespace cv;

int main(void)
{
    VideoCapture cap("../videos/akiyo_qcif.y4m");
    Mat resized_frame;

    while (1) {

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
        break;

    resize(frame, resized_frame, Size(800, 800));

    std::vector<Mat> bgr_planes;
    split(resized_frame, bgr_planes);

    int histSize = 256;

    float range[] = {0, 256}; //the upper boundary is exclusive
    const float* histRange = {range};

    bool uniform = true, accumulate = false;

    Mat b_hist, g_hist, r_hist;

    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double) hist_w/histSize);

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar( 0,0,0));

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    for (int i = 1; i < histSize; i++)
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }

    imshow("Source video", resized_frame);
    imshow("calcHist Demo", histImage);

    // Press  ESC on keyboard to exit
    char c = (char) waitKey(25);
    if (c==27) break;
    }
    
    waitKey();
    return EXIT_SUCCESS;
}