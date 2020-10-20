#include <opencv2/core/mat.hpp>

/**
 * Function used to insert a watermark into an image
 * 
 * @param initial_image Mat class, containing the initial image
 * @param watermark Mat class, containing the watermark
 * @param double input, which will be the alpha parameter
 * @return the both images, merged
 */
cv::Mat insert_watermark(cv::Mat initial_image, cv::Mat watermark, double input);

cv::Mat bgr_to_yuv(cv::Mat initial_image);

cv::Mat bgr_to_hsv(cv::Mat initial_image);

cv::Mat bgr_to_grayscale(cv::Mat initial_image);

cv::Mat histo_equalization(cv::Mat initial_image);
