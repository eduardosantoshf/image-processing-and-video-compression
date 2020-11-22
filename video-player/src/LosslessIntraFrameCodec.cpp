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
        LosslessIntraFrameCodec(string initialFile, string resultFile, int videoFormat, int predictorType, int m, int framesNumber) {
            this->initialFile = initialFile;
            this->videoFormat = videoFormat;
            this->predictorType = predictorType;
            this->m = m;

            int c = 0;

            VideoCapture cap(initialFile);

            Mat frame;

            while(1) {
                cap >> frame;
                c++;
                if (frame.empty()) break;
            }

            predictor = new Predictor(resultFile, videoFormat, predictorType, m, c, 1);
        }

        void losslessEncode() {
            VideoCapture cap(initialFile);

            Mat frame;

            vector<Mat> channels;

            if (predictorType == 1) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG1(channels[0]);
                    predictor->encodeJPEG1(channels[1]);
                    predictor->encodeJPEG1(channels[2]);
                }
            }
            else if (predictorType == 2) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG2(channels[0]);
                    predictor->encodeJPEG2(channels[1]);
                    predictor->encodeJPEG2(channels[2]);
                }
            }
            else if (predictorType == 3) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG3(channels[0]);
                    predictor->encodeJPEG3(channels[1]);
                    predictor->encodeJPEG3(channels[2]);
                }
            }
            else if (predictorType == 4) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG4(channels[0]);
                    predictor->encodeJPEG4(channels[1]);
                    predictor->encodeJPEG4(channels[2]);
                }
            }
            else if (predictorType == 5) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG5(channels[0]);
                    predictor->encodeJPEG5(channels[1]);
                    predictor->encodeJPEG5(channels[2]);
                }
            }
            else if (predictorType == 6) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG6(channels[0]);
                    predictor->encodeJPEG6(channels[1]);
                    predictor->encodeJPEG6(channels[2]);
                }
            }
            else if (predictorType == 7) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEG7(channels[0]);
                    predictor->encodeJPEG7(channels[1]);
                    predictor->encodeJPEG7(channels[2]);
                }
            }
            else {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodeJPEGLS(channels[0]);
                    predictor->encodeJPEGLS(channels[1]);
                    predictor->encodeJPEGLS(channels[2]);
                }
            }
        }

        void losslessDecode() {
            Mat decodedFrame;
            Mat decodedChannel0;
            Mat decodedChannel1;
            Mat decodedChannel2;

            vector<Mat> decodedChannels;

            if (predictor->getPredictorType() == 1) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG1();
                    decodedChannel1 = predictor->decodeJPEG1();
                    decodedChannel2 = predictor->decodeJPEG1();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
            else if (predictor->getPredictorType() == 2) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG2();
                    decodedChannel1 = predictor->decodeJPEG2();
                    decodedChannel2 = predictor->decodeJPEG2();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
            else if (predictor->getPredictorType() == 3) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG3();
                    decodedChannel1 = predictor->decodeJPEG3();
                    decodedChannel2 = predictor->decodeJPEG3();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
            else if (predictor->getPredictorType() == 4) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG4();
                    decodedChannel1 = predictor->decodeJPEG4();
                    decodedChannel2 = predictor->decodeJPEG4();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
            else if (predictor->getPredictorType() == 5) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG5();
                    decodedChannel1 = predictor->decodeJPEG5();
                    decodedChannel2 = predictor->decodeJPEG5();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
            else if (predictor->getPredictorType() == 6) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG6();
                    decodedChannel1 = predictor->decodeJPEG6();
                    decodedChannel2 = predictor->decodeJPEG6();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
            else if (predictor->getPredictorType() == 7) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEG7();
                    decodedChannel1 = predictor->decodeJPEG7();
                    decodedChannel2 = predictor->decodeJPEG7();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
            else {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodeJPEGLS();
                    decodedChannel1 = predictor->decodeJPEGLS();
                    decodedChannel2 = predictor->decodeJPEGLS();

                    decodedChannels.push_back(decodedChannel0);
                    decodedChannels.push_back(decodedChannel1);
                    decodedChannels.push_back(decodedChannel2);

                    merge(decodedChannels, decodedFrame);
                    imshow("Decoded Frame", decodedFrame);
                }
            }
        }
};