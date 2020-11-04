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

            /**
             * Function used to write individual bits on file, 
             * knowing that to write data in a file we have to
             * do it 1 byte (8 bits) at a time
             * 
             * @param bit value that we want to write
             */

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

            /**
             * Function used to read one bit at a time from
             * the given file
             * 
             * @param n position of the bit we want to read, 
             * starting at 0
             */

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

           /**
             * Function used to read N bits at a time from
             * the given file
             * 
             * @param n number of bits we want to read, 
             * starting from the first one (index 0)
             */

            for (int i = 0; i < n; i++) {
                readBit(i);
            }
       }

       void writeNBits(int n, int b) {

       }

        void readFile() {

            /**
             * Function used to read all the lines 
             * from the file
             */

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