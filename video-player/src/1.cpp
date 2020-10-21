#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {

  // Create a VideoCapture object and open the input file
  VideoCapture cap("../videos/akiyo_qcif.y4m"); 
   
  if (!cap.isOpened()){
    cout << "Error opening file" << endl;
    return -1;
  }
	
  while (1) {

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
      break;

    // Display the resulting frame
    imshow("Frame", frame);

    // Press  ESC on keyboard to exit
    char c = (char) waitKey(25);
    if (c==27) break;
  }
 
  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  destroyAllWindows();
	
  return 0;
}