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

            int c = 0;

            if (flag) {
                
                VideoCapture cap(initialF);

                Mat frame;

                while(1) {
                    cap >> frame;
                    c++;
                    if (frame.empty()) break;
                }
                predictor = new Predictor(resultFile, videoFormat, predictorType, m, c, flag, blockSize, searchArea)
            }
            else predictor = new Predictor(resultFile, 0, 0, 0, 0, 0, blockSize, searchArea)
        }
};