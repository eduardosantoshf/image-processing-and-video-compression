/*! \file WBitStream.cpp
 *	\brief Class to write bits to a binary file
 *	       
 *	
 *
 *	Class that can write one bit, n bits or strings to a binary file
 *  
 *	Press ESC to exit.
 *
 */


#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>

using namespace std;

/*! Writing BitStream class */
class WBitStream {
	private:
		unsigned char cacheWrite/*! uchar used to keep the bits until we can write them as 1 byte */;
		int posWrite/*! int used to keep track of the position in cacheWrite */;
		string FileName/*! File for output */;
		ofstream *ofsb;
		
		//! Write the cacheWrite content as a byte in the specified file using ofstream
		void writeByte(){
			ofsb->write(reinterpret_cast<char*>(&cacheWrite), 1);
		}
		
	public: 
		
		//! A constructor, matches the input to the FileName atribute, initiates the cacheWrite a 0 and the position at the start(7).
	    /*!
	      \param file A string with the name of the output file
	    */
	    
		WBitStream(string file){
			ofsb = new ofstream(file, ios::binary | ios::app | ios::out);
			FileName = file;
			cacheWrite = 0;
			posWrite = 7;
		
		}
		
		//! Write a bit function, creates a uchar with 1 in the correct position for the bits already stored and then using the or operator bit wise "write" it on the cache, if the position is not valid ofter it is updated the function resets the position and the cache writing the byte it had stored
	    /*!
	      \param i  0 or 1 the value of the bit you want to write
	    */	
	    
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
		
		//! Write n bits function, function that writes the bits left to right from the n-1 bit to the end, each bit is written using the write bit function
	    /*!
	      \param c The integer you want to write to the file
	      \param n The amount of bits to write said integer
	    */
		
		void writeNBits(int c, int n){
			for (int i = n-1 ; i != -1 ; i--) {
			    writeBit((int)((c & (1 << i)) != 0));
			}
		}
		
		//! Write a string function, function that  for each char of the string used the writeNBits function to write a byte, each for each char of the string
	    /*!
	      \param s The String you want to write to the file
	    */
		void writeString(string s){
			for(char& c : s) {
   			    writeNBits((int)c,8);
			}
		}
		
		//! Write the bits in the buffer if its not a multiple of 8
		
		void close(){
			writeByte();
		}
		
		void closeNoWrite(){
			ofsb->close();
		}
};




















