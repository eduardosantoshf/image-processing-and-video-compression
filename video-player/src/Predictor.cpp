#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include "Golomb2.cpp"

using namespace cv;
using namespace std;

class Predictor {
	private:
		int m;
		int predictorType;
		int videoFormat;
		int framesNumber;
		int height;
		int width;
		int flag;
		Golomb *golomb;
		WBitStream *wbs;

	public: 
	   	Predictor (string filename, int vt, int pt, int mValue, int fn, int flag2) {
               /**
                * Predictor constructor
                * @param filename file name
                * @param vt video format
                * @param pt predictor type
                * @param mValue value of m
                * @param fn number of frames from the file
                * @param flag2 if 1 encode, else decode
                */
	   		if (flag2 == 1){
		   		m = mValue;
		   		predictorType = pt;
		   		videoFormat = vt;
		   		golomb = new Golomb(filename, m, 1);
		   		flag = 1;
		   		framesNumber = fn;

		   		wbs = new WBitStream(filename);
		   		wbs->writeNBits(videoFormat,8);
		   		wbs->writeNBits(predictorType, 8);
		   		wbs->writeNBits(m, 8);
		   	}else{
		   		RBitStream rbs(filename);

		   		videoFormat = rbs.readNBits(8);
                cout << "Type video: " << videoFormat << endl;

		   		predictorType = rbs.readNBits(8);
                cout << "Type : " << predictorType << endl;

		   		m = rbs.readNBits(8);
                cout << "m: " << m << endl;

		   		width = stoi(rbs.readString(4));
                cout << "width: " << width << endl;

		   		height = stoi(rbs.readString(4));
                cout << "height: " << height<< endl;

		   		framesNumber = stoi(rbs.readString(4));
                cout << "framesNumber: " << framesNumber<< endl;

		   		rbs.close();

		   		golomb = new Golomb(filename, m, 0);
				golomb->SkipNBytes(15);
		   	}
	   	}
	   	
        int getFramesNumber(){
            /**
             * Function to get the number of frames
             * 
             * @return number of frames
             */ 
            return framesNumber;
        }
        
        int getPredictorType(){
            /**
             * Function to get the predictor type
             * 
             * @return predictor type
             */ 
            return predictorType;
        }
	    	
	    void encodeJPEG1(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 1st predictor
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int a = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((c - 1) >= 0)
                        a = (int) frame.at<uchar>(l, c - 1);
                    else
                        a = 0;

                    pixel = (int) frame.at<uchar>(l, c);

                    golomb->encode(pixel - a);
                }
            }
        }
        
        void encodeJPEG2(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 2nd predictor
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int b = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((l - 1) >= 0)
                        b = (int) frame.at<uchar>(l - 1, c);
                    else
                        b = 0;

                    pixel = (int) frame.at<uchar>(l, c);
                    
                    golomb->encode(pixel - b);
                }
            }
        }

        void encodeJPEG3(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 3rd predictor
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int c = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((l - 1) >= 0 && (c - 1) >= 0)
                        c = (int) frame.at<uchar>(l - 1, c - 1);
                    else
                        c = 0;
                    
                    pixel = (int) frame.at<uchar>(l, c);
                    
                    golomb->encode(pixel - c);
                }
            }
        }

        void encodeJPEG4(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 4th predictor
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int a = 0;
            int b = 0;
            int c2 = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((c - 1) >= 0)
                        a = (int) frame.at<uchar>(l, c - 1);
                    else
                        a = 0;
                    
                    if ((l - 1) >= 0)
                        b = (int) frame.at<uchar>(l - 1, c);
                    else
                        b = 0;
                    
                    if ((l - 1) >= 0 && (c - 1) >= 0)
                        c2 = (int) frame.at<uchar>(l - 1, c - 1);
                    else
                        c2 = 0;
                    
                    pixel = (int) frame.at<uchar>(l, c);

                    golomb->encode(pixel - (a + b - c2));
                }
            }
        }

        void encodeJPEG5(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 5th predictor
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int a = 0;
            int b = 0;
            int c2 = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((c - 1) >= 0)
                        a = (int) frame.at<uchar>(l, c - 1);
                    else
                        a = 0;
                    
                    if ((l - 1) >= 0)
                        b = (int) frame.at<uchar>(l - 1, c);
                    else
                        b = 0;
                    
                    if ((l - 1) >= 0 && (c - 1) >= 0)
                        c2 = (int) frame.at<uchar>(l - 1, c - 1);
                    else
                        c2 = 0;
                    
                    pixel = (int) frame.at<uchar>(l, c);

                    golomb->encode(pixel - (a + ((b - c2) / 2)));
                }
            }
        }

        void encodeJPEG6(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 6th predictor
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int a = 0;
            int b = 0;
            int c2 = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((c - 1) >= 0)
                        a = (int) frame.at<uchar>(l, c - 1);
                    else
                        a = 0;
                    
                    if ((l - 1) >= 0)
                        b = (int) frame.at<uchar>(l - 1, c);
                    else
                        b = 0;
                    
                    if ((l - 1) >= 0 && (c - 1) >= 0)
                        c2 = (int) frame.at<uchar>(l - 1, c - 1);
                    else
                        c2 = 0;
                    
                    pixel = (int) frame.at<uchar>(l, c);

                    golomb->encode(pixel - (b + ((a - c2) / 2)));
                }
            }
        }

        void encodeJPEG7(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 7th predictor
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int a = 0;
            int b = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((c - 1) >= 0)
                        a = (int) frame.at<uchar>(l, c - 1);
                    else
                        a = 0;
                    
                    if ((l - 1) >= 0)
                        b = (int) frame.at<uchar>(l - 1, c);
                    else
                        b = 0;

                    pixel = (int) frame.at<uchar>(l, c);

                    golomb->encode(pixel - ((a + b) / 2));
                }
            }
        }

        void encodeJPEGLS(Mat frame) {
            /**
             * Function to encode a given frame, using
             * the 8h predictor - JPEG-LS
             * 
             * @param frame frame
             */
            int lines = frame.rows;
            int columns = frame.cols;

            writeFrameSize(lines, columns, frame);

            int pixel;
            int a = 0;
            int b = 0;
            int c2 = 0;

            for (int l = 0; l < lines; l++) {
                for (int c = 0; c < columns; c++) {
                    if ((c - 1) >= 0)
                        a = (int) frame.at<uchar>(l, c - 1);
                    else
                        a = 0;
                    
                    if ((l - 1) >= 0)
                        b = (int) frame.at<uchar>(l - 1, c);
                    else
                        b = 0;
                    
                    if ((l - 1) >= 0 && (c - 1) >= 0)
                        c2 = (int) frame.at<uchar>(l - 1, c - 1);
                    else
                        c2 = 0;
                    
                    pixel = (int) frame.at<uchar>(l, c);

                    if (c2 >= max(a, b))
                        golomb->encode(pixel - min(a, b));
                    else if (c2 <= min(a, b))
                        golomb->encode(pixel - max(a, b));
                    else
                        golomb->encode(pixel - (a + b - c2));
                }
            }
        }
	   	
	   	Mat decodeJPEG1(){
            /**
             * Function to decode one JPEG1 encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

            int a;

			for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if((c - 1) >= 0){
						a = (int) decodedFrame.at<uchar>(l, c - 1);
					}
                    else{
						a = 0;
					}
					int d = golomb->decode();
					decodedFrame.at<uchar>(l, c) = (unsigned char) (d + a);				
				}
			}
			return decodedFrame;
	   	}
	   	
	   	Mat decodeJPEG2(){
            /**
             * Function to decode one JPEG2 encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

            int b;
			
            for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if (l - 1 >= 0){
						b = (int) decodedFrame.at<uchar>(l - 1,c);
					}
                    else{
						b = 0;
					}

					int d = golomb->decode();

					decodedFrame.at<uchar>(l, c) = (unsigned char) (d + b);			
				}
			}
			return decodedFrame;
	   	}
	   	
	   	Mat decodeJPEG3(){
            /**
             * Function to decode one JPEG3 encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

            int c;

			for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if ((l - 1) >= 0 && (c - 1) >= 0){
						c = (int) decodedFrame.at<uchar>(l - 1, c - 1);
					}
                    else{
						c = 0;
					}
					int d = golomb->decode();
					decodedFrame.at<uchar>(l, c) = (unsigned char) (d + c);		
				}
			}
			return decodedFrame;
	   	}
	   	
	   	Mat decodeJPEG4(){
            /**
             * Function to decode one JPEG4 encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

			int a, b, c2;

			for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if ((c - 1) >= 0){
						a = (int) decodedFrame.at<uchar>(l, c - 1);
					}
                    else {
						a = 0;
					}
					if ((l - 1) >= 0){
						b = (int) decodedFrame.at<uchar>(l - 1, c);
					}
                    else {
						b = 0;
					}
					if ((l - 1) >= 0 && (c - 1) >= 0){
						c2 = (int) decodedFrame.at<uchar>(l - 1, c - 1);
					}
                    else {
						c2 = 0;
					}

					int d = golomb->decode();
					decodedFrame.at<uchar>(l, c) = (unsigned char) (d + a + b - c2);
				}
			}
			return decodedFrame;
	   	}
	   	
	   	Mat decodeJPEG5(){
            /**
             * Function to decode one JPEG5 encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

			int a, b, c2;

			for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if ((c - 1) >= 0){
						a = (int) decodedFrame.at<uchar>(l, c - 1);
					}
                    else {
						a = 0;
					}
					if ((l - 1) >= 0){
						b = (int) decodedFrame.at<uchar>(l - 1, c);
					}
                    else {
						b = 0;
					}
					if ((l - 1) >= 0 && (c - 1) >= 0){
						c2 = (int) decodedFrame.at<uchar>(l - 1, c - 1);
					}
                    else {
						c2 = 0;
					}

					int d = golomb->decode();
					decodedFrame.at<uchar>(l, c) = (unsigned char) (d + a + (b - c2)/2);
				}
			}
			return decodedFrame;
	   	}
	   	
	   	Mat decodeJPEG6(){
            /**
             * Function to decode one JPEG6 encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

			int a, b, c2;

			for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if ((c - 1) >= 0){
						a = (int) decodedFrame.at<uchar>(l, c - 1);
					}
                    else {
						a = 0;
					}
					if ((l - 1) >= 0){
						b = (int) decodedFrame.at<uchar>(l - 1, c);
					}
                    else {
						b = 0;
					}
					if ((l - 1) >= 0 && (c - 1) >= 0){
						c2 = (int) decodedFrame.at<uchar>(l - 1, c - 1);
					}
                    else {
						c2 = 0;
					}

					int d = golomb->decode();
					decodedFrame.at<uchar>(l, c) = (unsigned char) (d + b + (a - c2)/2);
				}
			}
			return decodedFrame;
	   	}
	   	
	   	Mat decodeJPEG7(){
            /**
             * Function to decode one JPEG7 encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

			int a, b;

			for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if ((c - 1) >= 0){
						a = (int) decodedFrame.at<uchar>(l, c - 1);
					}
                    else {
						a = 0;
					}
					if ((l - 1) >= 0){
						b = (int) decodedFrame.at<uchar>(l - 1, c);
					}
                    else {
						b = 0;
					}
					int d = golomb->decode();
					decodedFrame.at<uchar>(l, c) = (unsigned char) (d + (a + b)/2);
				}
			}
			return decodedFrame;
	   	}
	   	
	   	Mat decodeJPEGLS(){
            /**
             * Function to decode one JPEG-LS encoded frame
             * 
             * @return decoded frame
             */
	   		Mat decodedFrame(height, width, 0);

			int a, b, c2;

			for (int l = 0; l < height; l++){
				for (int c = 0; c < width; c++){
					if ((c - 1) >= 0){
						a = (int) decodedFrame.at<uchar>(l, c - 1);
					}
                    else {
						a = 0;
					}
					if ((l - 1) >= 0){
						b = (int) decodedFrame.at<uchar>(l - 1, c);
					}
                    else {
						b = 0;
					}
                    if ((l - 1) >= 0 && (c - 1) >= 0){
						c2 = (int) decodedFrame.at<uchar>(l - 1, c - 1);
					}
                    else {
						c2 = 0;
					}

                    int d = golomb->decode();

					if (c2 >= max(a,b)) {
						decodedFrame.at<uchar>(l, c) = (unsigned char) (d + min(a, b));					
					} else if (c2 <= min(a,b)) {
						decodedFrame.at<uchar>(l, c) = (unsigned char) (d + max(a, b));	
					} else {
						decodedFrame.at<uchar>(l, c) = (unsigned char) (d + a + b - c2);
					}
				}
			}
			return decodedFrame;
	   	}

        void writeFrameSize(int lines, int columns, Mat frame) {
            /**
             * Function to write some info at the begining of
             * a to be encrypted file 
             */
            if (flag) {
                cout << "Writing file info..." << endl;
                string width  = to_string(columns);
                string height = to_string(lines);
                string frames = to_string(framesNumber);

                for (int w = 0; w < 4 - width.length(); w++){
                    width = "0" + width;
                }
                for (int h = 0; h < 4 - height.length(); h++){
                    height = "0" + height;
                }
                for (int f = 0; f < 4 - frames.length(); f++){
                    frames = "0" + frames;
                }

                wbs->writeString(width);
                wbs->writeString(height);
                wbs->writeString(frames);

                flag = 0;
            }
        }
};