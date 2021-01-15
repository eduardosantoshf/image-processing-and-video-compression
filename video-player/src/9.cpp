#include<iostream>
#include <vector>
#include <chrono>
#include "LosslessHybridCodec.cpp"

using namespace std;

int main (int argc, char** argv) {
	//vector<int> m_values = {2, 4, 5, 6, 7, 8};
	
	//attention to the subsampling mode of the video
	//parameters: output file name - block size - search space - 
	LosslessHybridCodec l(argv[1], 16, 3, 0, 7, 2, 5, 1, "../results/teste1_m" + to_string(2) + ".bin");
	
	auto t1 = std::chrono::high_resolution_clock::now();
	l.encode();
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

	cout << "Encoding time: " << duration / 1000000 << "s" << endl;

	/*
	LosslessHybridCodec d("", 0, 0, 0, 0, 0, 0, 0, argv[1]);

	auto t1 = std::chrono::high_resolution_clock::now();
	d.decode();
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

	cout << "Decoding time: " << duration / 1000000 << "s" << endl;
	*/

	return 0;
}