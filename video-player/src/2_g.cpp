#include<opencv2/opencv.hpp>
#include<iostream>
#include "methods.cpp"

using namespace cv;

Mat src, dst, modified_image;
int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

const char* window_name = "Morphology Transformations Demo";

void Morphology_Operations( int, void* );

int main( int argc, char** argv )
{
    VideoCapture cap("../videos/akiyo_qcif.y4m");

    while (1) {

    Mat frame, resized_frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
        break;

    resize(frame, resized_frame, Size(500, 500));

    src = resized_frame;
    
    namedWindow( window_name, WINDOW_AUTOSIZE ); // Create window
    createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator, Morphology_Operations );
    createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,
                    &morph_elem, max_elem,
                    Morphology_Operations );
    createTrackbar( "Kernel size:\n 2n +1", window_name,
                    &morph_size, max_kernel_size,
                    Morphology_Operations );
    Morphology_Operations( 0, 0 );

    char c = (char) waitKey(25);
    if (c==27) break;
    }

    waitKey(0);
    return 0;
}

void Morphology_Operations( int, void* )
{
    // Since MORPH_X : 2,3,4,5 and 6
    int operation = morph_operator + 2;
    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    dst = morphological_operation( src, dst, operation, element );
    imshow( window_name, dst );
}