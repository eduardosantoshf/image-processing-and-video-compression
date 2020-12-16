#include<iostream>
#include<vector>
#include "Predictor.cpp"

class LosslessHybridCodec {
    private:
        int m;
        int videoFormat;
        int predictorType;
        int blockSize;
        int searchArea;
        int periodicity;
        int currentFrame;
        string initialFile;
        Predictor *predictor;

    public:
        
        LosslessHybridCodec(string initialF, string resultFile, int vF, int pT, int mValue, int bS, int sA, int p, int flag) {
            initialFile = initialF;
            videoFormat = vF;
            predictorType = pT;
            m = mValue;
            blockSize = bS;
            searchArea = sA;
            periodicity = p;
            currentFrame = 0;

            int c = 0;

            if (flag) {
                
                VideoCapture cap(initialF);

                Mat frame;

                while(1) {
                    cap >> frame;
                    c++;
                    if (frame.empty()) break;
                }
                predictor = new Predictor(resultFile, videoFormat, predictorType, m, c, flag, blockSize, searchArea, periodicity);
            }
            else predictor = new Predictor(resultFile, 0, 0, 0, 0, 0, 0, 0, 0);
        }

        void losslessHybridEncode(){
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

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEG1(channels[0]);
                        predictor->encodeJPEG1(channels[1]);
                        predictor->encodeJPEG1(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            } else if (predictorType == 2) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEG2(channels[0]);
                        predictor->encodeJPEG2(channels[1]);
                        predictor->encodeJPEG2(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            } else if (predictorType == 3) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEG3(channels[0]);
                        predictor->encodeJPEG3(channels[1]);
                        predictor->encodeJPEG3(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            } else if (predictorType == 4) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEG4(channels[0]);
                        predictor->encodeJPEG4(channels[1]);
                        predictor->encodeJPEG4(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            } else if (predictorType == 5) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEG5(channels[0]);
                        predictor->encodeJPEG5(channels[1]);
                        predictor->encodeJPEG5(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            } else if (predictorType == 6) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEG6(channels[0]);
                        predictor->encodeJPEG6(channels[1]);
                        predictor->encodeJPEG6(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            } else if (predictorType == 7) {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEG7(channels[0]);
                        predictor->encodeJPEG7(channels[1]);
                        predictor->encodeJPEG7(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            } else {
                while(1) {
                    vector<Mat> channels;
                    cap >> frame;
                    cout << "Encoding Frame: "<< contador << endl;
                    contador++;

                    if (frame.empty()) break;

                    split(frame, channels);

                    if (currentFrame % periodicity == 0) {

                        predictor->encodeJPEGLS(channels[0]);
                        predictor->encodeJPEGLS(channels[1]);
                        predictor->encodeJPEGLS(channels[2]);

                    } else predictor->encodeBlock(channels);

                    predictor->setLastFrame(channels);

                    currentFrame++;
                }
            }
            predictor->close();
        }

        void losslessHybridDecode(){
            Mat decodedFrame;
            Mat p1, p2, p3;
            vector<Mat> channels;

            int numberOfFrames = predictor->getFramesNumber();
            int pType = predictor->getPredictorType();
            int vFormat = predictor->getVideoFormat();
            int period = predictor->getPeriodicity();

            int contador = 0;

            if (pType == 1) {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEG1();
                        p2 = predictor->decodeJPEG1();
                        p3 = predictor->decodeJPEG1();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            } else if (pType == 2) {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEG2();
                        p2 = predictor->decodeJPEG2();
                        p3 = predictor->decodeJPEG2();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            } else if (pType == 3) {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEG3();
                        p2 = predictor->decodeJPEG3();
                        p3 = predictor->decodeJPEG3();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            } else if (pType == 4) {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEG4();
                        p2 = predictor->decodeJPEG4();
                        p3 = predictor->decodeJPEG4();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            } else if (pType == 5) {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEG5();
                        p2 = predictor->decodeJPEG5();
                        p3 = predictor->decodeJPEG5();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            } else if (pType == 6) {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEG6();
                        p2 = predictor->decodeJPEG6();
                        p3 = predictor->decodeJPEG6();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            } else if (pType == 7) {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEG7();
                        p2 = predictor->decodeJPEG7();
                        p3 = predictor->decodeJPEG7();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            } else {
                for (int i = 0; i < numberOfFrames; i++) {
                    if (contador % period == 0) {
                        p1 = predictor->decodeJPEGLS();
                        p2 = predictor->decodeJPEGLS();
                        p3 = predictor->decodeJPEGLS();

                        channels = {p1, p2, p3};
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);
                        
                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;

                        cout << "Decoding frame: " << endl;
                    } else {
                        channels = predictor->decodeBlock();
                        merge(channels, decodedFrame);

                        predictor->setLastFrame(channels);

                        imshow("Decoded frame", decodedFrame);

                        if (waitKey(30) >= 0) break;
                    }
                    contador++;
                }
            }
        }
};