#include<iostream>
#include <vector>
#include "LosslessIntraFrameCodec.cpp"

using namespace std;

int main (int argc, char** argv) {

    LosslessIntraFrameCodec l(argv[1],"LosslessEncoded_p7_m4.bin", 0, 6, 4, 1);
    l.losslessEncode();

    //LosslessIntraFrameCodec l2("", argv[1], 0, 0, 0, 0);
    //l2.losslessDecode();
	return 0;
}