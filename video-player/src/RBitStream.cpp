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

			/**
             * Function to read byte
             */

			ifsb.read(reinterpret_cast<char*>(&cacheRead), 1);
		}
		
	public: 
		
		RBitStream(string file){

			/**
			 * WBitStream Constructor
			 * 
			 * @param file file to be read
			 */

			FileName = file;
			ifsb.open(FileName, ios::binary);
			readByte();
			posRead = 7;
		}
		
		int readBit(){

			/**
             * Function to read bit
             */

			unsigned char res = ((cacheRead & (1 << posRead)) != 0);
			posRead--;
			if (posRead < 0){
				readByte();
				posRead=7;
			}
			return res;
		}
		
		int readNBits(int n){

			/**
             * Function to read N bits from file
             * 
			 * @param n number of bits to be read
             */

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

			/**
             * Function to read string from file
             * 
			 * @param n number of strings to be read
             */

			string res = "";
			for(int i = 0; i < n; i++){
				res+=((char) readNBits(8));
			}
			return res;
		}
		
		void close(){

			/**
             * Function to close the file
             */
			
			ifsb.close();
		}
};



















