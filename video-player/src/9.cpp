#include<iostream>
#include <vector>
#include <chrono>
#include "LosslessHybridCodec.cpp"

using namespace std;

int main (int argc, char** argv) {
	//vector<int> m_values = {2, 4, 5, 6, 7, 8};
	
	//attention to the subsampling mode of the video
	//parameters: output file name, block size, search space, video type, predictor type, m, periodicity, flag, filename
	int m = 8;
	int video_type = 2;
	LosslessHybridCodec l(argv[1], 5, 3, video_type, 7, m, 5, 1, "../results/park_joy_420_m" + to_string(m) + ".bin");
	
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