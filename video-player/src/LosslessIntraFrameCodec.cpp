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

            predictor = new Predictor(resultFile, videoFormat, predictorType, m, c);
        }

        void losslessEncode() {
            VideoCapture cap(initialFile);

            Mat frame;

            vector<Mat> channels;

            string encodingType = "encodeJPEG" + to_string(predictorType);

            if (predictorType >= 1 && predictorType <= 7) {
                while(1) {
                    cap >> frame;

                    if (frame.empty()) break;

                    split(frame, channels);

                    predictor->encodingType(channels[0]);
                    predictor->encodingType(channels[1]);
                    predictor->encodingType(channels[2]);
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

        int losslessDecode() {
            Mat decodedFrame;
            Mat decodedChannel0;
            Mat decodedChannel1;
            Mat decodedChannel2;

            vector<Mat> decodedChannels;

            string decodingType = "decodeJPEG" + to_string(predictor->getPredictorType());

            if (predictor->getPredictorType() >= 1 && predictor->getPredictorType() <= 7) {
                for (int i = 0; i < predictor->getFramesNumber(); i++) {
                    decodedChannel0 = predictor->decodingType();
                    decodedChannel1 = predictor->decodingType();
                    decodedChannel2 = predictor->decodingType();

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