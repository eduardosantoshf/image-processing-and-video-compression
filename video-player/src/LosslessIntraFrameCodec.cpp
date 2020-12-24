#include<iostream>
#include<vector>
#include "Predictor.cpp"

class LosslessIntraFrameCodec {
    private:
        int m;
        int predictorType;
        int videoFormat;
        string initialFile;
        Predictor *predictor;

    public:

        /**
         * Lossless Intra-Frame Codec Constructor
         * 
         * @param initialF initial file name
         * @param resultFile result file name
         * @param vf video format
         * @param pt predictor type
         * @param mValue value of m
         * @param flag initialize the predictor with given values
         */

        LosslessIntraFrameCodec(string initialF, string resultFile, int vf, int pt, int mValue, int flag) {
            initialFile = initialF;
            videoFormat = vf;
            predictorType = pt;
            m = mValue;

            int c = 0;

            if (flag) {

                VideoCapture cap(initialF);

                Mat frame;

                while(1) {
                    cap >> frame;
                    c++;
                    if (frame.empty()) break;
                }

                predictor = new Predictor(resultFile, vf, pt, m, c, 1);
            }
            else predictor = new Predictor(resultFile, 0, 0, 0, 0, 0);
        }

        void losslessEncode() {

            /**
             * Function to encode a given video, using
             * 1 of the 8 predictors
             */

            VideoCapture cap(initialFile);

            Mat frame;

            int contador = 0;

            if (predictorType == 1) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);
                    
                    predictor->encodeJPEG1(channels[0]);
                    predictor->encodeJPEG1(channels[1]);
                    predictor->encodeJPEG1(channels[2]);
                }
            }
            
            else if (predictorType == 2) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG2(channels[0]);
                    predictor->encodeJPEG2(channels[1]);
                    predictor->encodeJPEG2(channels[2]);
                }
            }
            else if (predictorType == 3) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG3(channels[0]);
                    predictor->encodeJPEG3(channels[1]);
                    predictor->encodeJPEG3(channels[2]);
                }
            }
            else if (predictorType == 4) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG4(channels[0]);
                    predictor->encodeJPEG4(channels[1]);
                    predictor->encodeJPEG4(channels[2]);
                }
            }
            else if (predictorType == 5) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG5(channels[0]);
                    predictor->encodeJPEG5(channels[1]);
                    predictor->encodeJPEG5(channels[2]);
                }
            }
            else if (predictorType == 6) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG6(channels[0]);
                    predictor->encodeJPEG6(channels[1]);
                    predictor->encodeJPEG6(channels[2]);
                }
            }
            else if (predictorType == 7) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG7(channels[0]);
                    predictor->encodeJPEG7(channels[1]);
                    predictor->encodeJPEG7(channels[2]);
                }
            }
            else {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEGLS(channels[0]);
                    predictor->encodeJPEGLS(channels[1]);
                    predictor->encodeJPEGLS(channels[2]);
                }
            }
            
        }

        void losslessDecode() {

            /**
             * Function to decode a given .bin file,
             * checking the predictor type and m value
             * from the file and choosing the respective algorithm
             * to decode 
             */
            
            Mat decodedFrame;
            Mat decodedChannel0;
            Mat decodedChannel1;
            Mat decodedChannel2;

            vector<Mat> decodedChannels;

            int contador = 0;
            
            if (predictor->getPredictorType() == 1) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG1();
                    decodedChannel1 = predictor->decodeJPEG1();
                    decodedChannel2 = predictor->decodeJPEG1();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
            else if (predictor->getPredictorType() == 2) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG2();
                    decodedChannel1 = predictor->decodeJPEG2();
                    decodedChannel2 = predictor->decodeJPEG2();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
            else if (predictor->getPredictorType() == 3) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG3();
                    decodedChannel1 = predictor->decodeJPEG3();
                    decodedChannel2 = predictor->decodeJPEG3();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
            else if (predictor->getPredictorType() == 4) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG4();
                    decodedChannel1 = predictor->decodeJPEG4();
                    decodedChannel2 = predictor->decodeJPEG4();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
            else if (predictor->getPredictorType() == 5) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG5();
                    decodedChannel1 = predictor->decodeJPEG5();
                    decodedChannel2 = predictor->decodeJPEG5();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
            else if (predictor->getPredictorType() == 6) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG6();
                    decodedChannel1 = predictor->decodeJPEG6();
                    decodedChannel2 = predictor->decodeJPEG6();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
            else if (predictor->getPredictorType() == 7) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG7();
                    decodedChannel1 = predictor->decodeJPEG7();
                    decodedChannel2 = predictor->decodeJPEG7();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels,decodedFrame);
                    imshow("Display",decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
            else {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEGLS();
                    decodedChannel1 = predictor->decodeJPEGLS();
                    decodedChannel2 = predictor->decodeJPEGLS();

                    decodedChannels = {decodedChannel0,decodedChannel1,decodedChannel2};

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);

                    if (waitKey(30) >= 0) break;

                    cout << "Decoding frame: " << contador++ << endl;
                }
            }
        }
};