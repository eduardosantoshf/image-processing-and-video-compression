#include<iostream>
#include <vector>
#include "LosslessHybridCodec.cpp"

using namespace std;

int main (int argc, char** argv) {

    LosslessHybridCodec l(argv[1], 4, 5, 0, 4, 4, 5, 1, "teste.bin");
	l.encode();

    //LosslessHybridCodec d("", 0, 0, 0, 0, 0, 0, 0, argv[1]);
	//d.decode();

	return 0;
}