#include<iostream>
#include<fstream>
#include <vector>
#include "methods.cpp"

using namespace std;
using namespace cv;

class BitStream {
    public:
        int pos = 0;
        unsigned char buff = 0;
        const char* filename;
        int position = 0;

        BitStream(const char* fn) {
            filename = fn;
        }

        void writeBit(int bit) {
            if (bit) 
                buff |= (1 << pos);

            pos++;

            if (pos == 8) {
                ofstream ofs("test.bin", ios::binary | ios::app);
                ofs.write(reinterpret_cast<char*>(&buff), sizeof(buff) * sizeof(char));
                ofs.close();
                pos = 0;
                buff = 0;
            }
        }
        
        void readBit() { //only read first line
            ifstream ifs("test.bin", ios::binary | ios::in);
            char c;
            while (ifs.get(c)) {
                if (position <= 7) {
                    cout << ((c >> position) & 1);
                    cout << "\n";
                    position++;
                }
            }
        }
        
        void readFile() {
            ifstream ifs("test.bin", ios::binary | ios::in);
            char c;
            while (ifs.get(c)) {
                for (int i = 7; i >= 0; i--)
                    cout << ((c >> i) & 1);
                    cout << "\n";
            }
        }
};

int main(void)
{
    ofstream o;

    BitStream bs("teste.txt");
    bs.writeBit(0);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(0);

    //bs.flushBits();
    /*
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(1);
    */

    //bs.readBit();
    bs.readFile();
    bs.readBit();
    bs.readBit();
    bs.readBit();
    bs.readBit();
    bs.readBit();
    bs.readBit();
    bs.readBit();
}

