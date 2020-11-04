#include<iostream>
#include<fstream>
#include "BitStream.cpp"

using namespace std;

int main(void)
{

    BitStream bs("test.bin");
    
    
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(0);
    bs.writeBit(1);
    bs.writeBit(0);
    bs.writeBit(0);
    
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(0);
    bs.writeBit(1);
    bs.writeBit(1);
    
    bs.readFile();

    cout << "\n";

    bs.readBit(0);
    bs.readBit(1);
    bs.readBit(2);
    
    cout << "\n";
 
    bs.readNBits(13);
}

