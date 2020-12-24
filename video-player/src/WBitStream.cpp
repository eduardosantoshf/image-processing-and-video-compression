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

			/**
             * Function to write byte
             */

			ofsb->write(reinterpret_cast<char*>(&cacheWrite), 1);
		}
		
	public:

	    /**
         * WBitStream Constructor
		 * 
         * @param file given file
         */

		WBitStream(string file){
			ofsb = new ofstream(file, ios::binary | ios::app | ios::out);
			FileName = file;
			cacheWrite = 0;
			posWrite = 7;
		
		}
		
		void writeBit(int i){

			/**
             * Function to write individual bits on file
             * 
             * @param i bbit to be written
             */

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

			/**
             * Function to write N bits on file
             * 
             * @param c number to be written
			 * @param n number of bits used to write the number
             */

			for (int i = n-1 ; i != -1 ; i--) {
			    writeBit((int)((c & (1 << i)) != 0));
			}
		}

		void writeString(string s){

			/**
             * Function to write a string on a file
             * 
             * @param s string to be written
             */

			for(char& c : s) {
   			    writeNBits((int)c,8);
			}
		}
		
		void close(){

			/**
             * Function to close the file, with write
             */

			writeByte();
		}
		
		void closeNoWrite(){

			/**
             * Function to close the file
             */
			
			ofsb->close();
		}
};




















