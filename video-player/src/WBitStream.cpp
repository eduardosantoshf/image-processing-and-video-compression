#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>

using namespace std;

class WBitStream {
	private:
		unsigned char cacheWrite;
		int posWrite;
		string FileName;
		ofstream *ofsb;
		
		void writeByte(){
			ofsb->write(reinterpret_cast<char*>(&cacheWrite), 1);
		}
		
	public: 

		WBitStream(string file){
			ofsb = new ofstream(file, ios::binary | ios::app | ios::out);
			FileName = file;
			cacheWrite = 0;
			posWrite = 7;
		}
		
		void writeBit(int i){
			if (i == 1){
				unsigned char u = 1 << posWrite;
				cacheWrite |= u;
			}
			posWrite--;
			
			if (posWrite == -1){
				writeByte();
				posWrite = 7;
				cacheWrite = 0;
			}
		}
		
		void writeNBits(int c, int n){
			for (int i = n-1 ; i != -1 ; i--) {
			    writeBit((int)((c & (1 << i)) != 0));
			}
		}
		
		void writeString(string s){
			for(char& c : s) {
   			    writeNBits((int)c,8);
			}
		}
		
		void close(){
			writeByte();
		}

		void closeNoWrite() {
			ofsb->close();
		}
};