#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>

using namespace std;


class RBitStream {
	private:
		unsigned char cacheRead;
		int posRead;
		string FileName;
		ifstream ifsb;
		
		void readByte(){
			ifsb.read(reinterpret_cast<char*>(&cacheRead), 1);
		}
		
	public: 
		RBitStream(string file){
			FileName = file;
			ifsb.open(FileName, ios::binary);
			readByte();
			posRead = 7;
		}
		
		int readBit(){
			unsigned char res = ((cacheRead & (1 << posRead)) != 0);
			posRead--;
			if (posRead < 0){
				readByte();
				posRead=7;
			}
			return res;
		}

		int readNBits(int n){
			int res = 0;
			for (int i = 7 ; i != 7-n ; i--) {
				int b = readBit();
				if(b == 1){
					res += pow(2,n-1-(7-i));
				}
			}
			return res;
		}
		
		string readString(int n){
			string res = "";
			for(int i = 0; i < n; i++){
				res+=((char) readNBits(8));
			}
			return res;
		}
		
		void close(){
			ifsb.close();
		}
		
};