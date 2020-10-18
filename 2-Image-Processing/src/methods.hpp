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

/**
 * Structure to define a pixel using an array of three unsigned chars.
 * Each element of the array represents a value in the RGB color scheme.
 * 
 * Red value: rgb[0]
 * Green value: rgb[1]
 * Blue value: rgb[2]
*/
typedef struct 
{
    unsigned char rgb[3];
} RGBPixel;