#include <opencv2/core/mat.hpp>

/**
 * Function used to insert a watermark into an image
 * 
 * @param initial_image Mat class, containing the initial image
 * @param watermark Mat class, containing the watermark
 * @param double input, which will be the alpha parameter
 * @return both images, merged
 */
cv::Mat insert_watermark(cv::Mat initial_image, cv::Mat watermark, double input);

cv::Mat bgr_to_yuv(cv::Mat initial_image);

cv::Mat bgr_to_hsv(cv::Mat initial_image);

cv::Mat bgr_to_grayscale(cv::Mat initial_image);

cv::Mat histo_equalization(cv::Mat initial_image);

cv::Mat gaussian_blur(cv::Mat initial_image);

cv::Mat regular_blur(cv::Mat initial_image, int b1, int b2);

cv::Mat threshold_image(cv::Mat initial_image, int t_value, int t_type);

cv::Mat morphological_operation(cv::Mat initial_image, cv::Mat result_image, int op, cv::Mat elem);

cv::Mat sobel_derivative(cv::Mat initial_image, int ddepth, int x, int y, int ksize, int scale, int delta);

cv::Mat scharr_derivative(cv::Mat initial_image, int ddepth, int x, int y, int scale, int delta);

cv::Mat laplacian_derivative(cv::Mat initial_image);
