/*! \file RBitStream.cpp
 *	\brief Class to read bits to a binary file
 *	      
 *	
 *
 *	Class that can read one bit, n bits or strings to a binary file
 *  
 *	Press ESC to exit.
 *
 */


#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>

using namespace std;

/*! Reading BitStream class */
class RBitStream {
	private:
		unsigned char cacheRead /*! uchar used to keep the 8 bits considering the least you can read from a file is 8 bits (1 byte) */;
		int posRead/*! int used to keep track of the position in cacheRead */;
		string FileName/*! File Input*/ ;
		ifstream ifsb/*! File stream used to read the information */;
		
		//! Read 8 bits from the file (1 byte), using the input stream it reads from the file knowing we only want to read 1 byte
		void readByte(){
			ifsb.read(reinterpret_cast<char*>(&cacheRead), 1);
		}
		
	public: 
		
		
	    //! A constructor, Initiates the input file stream, reads the first byte and starts the trackers for the 8 bits.
	    /*!
	      \param file A string with the name of the input file
	    */
		RBitStream(string file){
			FileName = file;
			ifsb.open(FileName, ios::binary);
			readByte();
			posRead = 7;
		}
		
		//! Read a bit function, read the bit of of the current position, decrease the position after and it that position is no longer valid then we reset the postion and read another 8 bits
		int readBit(){
			unsigned char res = ((cacheRead & (1 << posRead)) != 0);
			posRead--;
			if (posRead < 0){
				readByte();
				posRead=7;
			}
			return res;
		}
		//! Read n bits function, using the readBit function read each bit and add them considering their position 
	    /*!
	      \param n The number of bits to read
	    */
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
		
		//! Read a string function, using the readNBits function, reads 8 bits n times and concatnates them to a string
	    /*!
	      \param n The length of the string
	    */
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



















