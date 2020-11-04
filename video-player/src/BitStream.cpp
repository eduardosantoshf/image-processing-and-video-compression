#include<iostream>
#include<fstream>

using namespace std;

class BitStream {
    public:
        int pos = 0;
        unsigned char buff = 0;
        fstream fsi;
        fstream fso;
        fstream fsiF;
        unsigned char readBuff = 0;
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
           int cont = 1;
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

       void readNBits(int n) {
            for (int i = 0; i < n; i++) {
                readBit(i);
            }
       }

       void writeNBits(int n, int b) {

       }

        void readFile() {
            fsiF.open("test.bin", ios::binary | ios::in);
            char c;
            while (fsiF.get(c)) {
                for (int i = 7; i >= 0; i--)
                    cout << ((c >> i) & 1);
                    cout << "\n";
            }
            fsiF.close();
        }
};