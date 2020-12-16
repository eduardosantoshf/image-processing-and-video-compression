#include<iostream>
#include <vector>
#include "LosslessHybridCodec.cpp"

using namespace std;

int main (int argc, char** argv) {

    LosslessHybridCodec l(argv[1], "teste.bin", 0, 7, 4, 5, 5, 5, 1);
    l.losslessHybridEncode();

	return 0;
}