#include<iostream>
#include<fstream>
#include "methods.cpp"

using namespace std;
using namespace cv;

class BitStream {
    public:
        int pos = 0;
        unsigned char buff = 0;
        fstream fsi;
        fstream fso;
        unsigned char readBuff = 0;
        int cont = 1;
        bitset<8> b;
        string filename;

        BitStream(string fn) {
            filename = fn;
        }
        
        void writeBit(int bit) {
            if (bit) 
                buff |= (1 << pos);
            pos++;

            if (pos == 8) {
                fso.open(filename, ios::binary | ios::app);
                fso.write(reinterpret_cast<char*>(&buff), sizeof(buff) * sizeof(char));
                fso.close();
                pos = 0;
                buff = 0;
            }
        }

       void readBit(int n) {
            fsi.open(filename, ios::in | ios::binary);
            readBuff = 0;

            while (n > 7) {
                cont++;
                n = n - 8;
            }

            for (int j = 0; j < cont; j++) {
                fsi.read(reinterpret_cast<char *>(&readBuff), 1);
            }

            b = bitset<8>(readBuff);
            cout << b << "\n";

            cout << b.to_string()[n] << "\n";
            fsi.close();

       }

        void readFile() {
            ifstream ifs("test.bin", ios::binary | ios::in);
            char c;
            while (ifs.get(c)) {
                for (int i = 7; i >= 0; i--)
                    cout << ((c >> i) & 1);
                    cout << "\n";
            }
            ifs.close();
        }
};

int main(void)
{

    BitStream bs("test.bin");
    /*
    
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
    */
    
    bs.readFile();
    //bs.readBit(11);

    
    for (int i = 0; i < 16; i++) {
        bs.readBit(i);
    }
    
 
}

