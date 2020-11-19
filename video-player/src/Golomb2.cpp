/*! \file Golomb.cpp
 *	\brief Class to encode and decode Golomb code
 *	      
 *	
 *
 *	
 *  
 *	Press ESC to exit.
 *
 */

#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>
#include "RBitStream.cpp"
#include "WBitStream.cpp"

using namespace std;

/*! Golomb code class */
class Golomb {
	private:
		int m/*! Paramter m for the Glomb code */;
		int flag/*! Flag to signal if we are reading or writing */;
		RBitStream* rbs/*! Reading BitStream */;
		WBitStream* wbs/*! Writing BitStream */;;
		
	public: 
	
	   	//! A constructor, Initiates the type of BitStream considering the flag, and atributes value to the class atributes
	    /*!
	      \param file A string with the name of the file
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are reading(0) or Writing(1)
	    */
	   	Golomb(string file, int M, int Flag){
	   		if(flag == 0){
	   			rbs = new RBitStream(file);
	   		}else{
	   			wbs = new WBitStream(file);
	   		}
	   		m = M;
	   		flag = Flag;
	   	}
	   	
	   	//! Encode de number passed as a parameter, calculating the quotient and the remainder and ecoding inunary and binary respectively depending on the value of m
	    /*!
	      \param n The number to encode
	    */
	   	void encode(int n) {
            int sign = 0;
            if (n < 0) {
                sign = 1;
                n = n * (-1);
            }
            int r = n % m;
            int q = n / m;
            wbs -> writeBit(sign);
            // Unary part
            for (int c = 0; c < q; c++) {
                wbs -> writeBit(0);
            }
            wbs -> writeBit(1);
            if ((m & (m - 1)) == 0) {
                // Binary part
                int pow = log2(m);
                unsigned char u = (unsigned char) r;
                for (int i = 7 - (7 - pow) - 1; i != -1; i--) {
                wbs -> writeBit((int)((u & (1 << i)) != 0));
                }
            } else {
                // Binary part truncated
                int b = ceil(log2(m));
                unsigned char tr = (unsigned char) r;
                if (r < pow(2, b) - m) {
                wbs -> writeNBits(r, b - 1);
                //for (int i = 7 -(7-(b-1)) -1 ; i != -1 ; i--) {
                //	wbs->writeBit((int)((tr & (1 << i)) != 0));
                //}	
                } else {
                int bigger_r = r + pow(2, b) - m;
                unsigned char br = (unsigned char) bigger_r;
                wbs -> writeNBits(bigger_r, b);
                //for (int i = 7 -(7-b) -1 ; i != -1 ; i--) {
                //	wbs->writeBit((int)((br & (1 << i)) != 0));
                //}	
                }
            }
        }
	   	//! Decode a number from the file passed, first we read 0 bits until we find a bit with the value of 1, the number of 0's is the quotient, secondly we read NBits from the file with N being the log2(m), lastly return the value of the integer decoded, this is not true when m is not a power of 2 since we need to do some more calculations
	   	int decode() {
            int sign = rbs -> readBit();
            int bit = rbs -> readBit();
            int quo = 0;
            while (bit != 1) {
                bit = rbs -> readBit();
                quo++;
            }
            if ((m & (m - 1)) == 0) {
                int pow = log2(m);
                int r = rbs -> readNBits(pow);
                if (sign == 0) {
                return quo * m + r;
                } else {
                return (quo * m + r) * (-1);
                }

            } else {
                int b = ceil(log2(m));
                int z = rbs -> readNBits(b - 1);
                if (z < pow(2, b) - m) {
                if (sign == 0) {
                    return quo * m + z;
                } else {
                    return (quo * m + z) * (-1);
                }
                } else {
                int other_bit = rbs -> readBit();
                int fr = (z << 1) + other_bit;
                if (sign == 0) {
                    return quo * m + (fr - pow(2, b) + m);
                } else {
                    return (quo * m + (fr - pow(2, b) + m)) * (-1);
                }
                }
            }
        }
	   	
	   	//! simply close the write stream considering we mingh not encode a number of bits multiple of 8
	   	void close(){
	   		if (flag == 1){
	   			wbs->close();
	   		}
	   	}
		
};



















