#include<iostream>
#include<fstream>
#include<bitset>

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
                buff |= (1 << pos); //add to buffer
            pos++;

            if (pos == 8) { //when we have enough bits to write 1 byte (8 bits), write to the file
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
                cont++; //check which byte we'll be reading
                n = n - 8;
            }

            for (int j = 0; j < cont; j++) {
                fsi.read(reinterpret_cast<char *>(&readBuff), 1);
            }

            b = bitset<8>(readBuff); //store the byte in a bitset
            cout << b << "\n";

            cout << b.to_string()[n] << "\n"; //there has got to be a better way of doing this
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

           /**
             * Function used to write N bits at a time from
             * the given file
             * 
             * @param n number we want to be written in binary
             * @param b how many bits do we want the number n
             * to be written
             */

            int a[8];
            int i;
            int lengthA = 0; //length of a
            int surplus = 0; //numer of extra 0s we'll add at the left side of the binary number

            //turn the given number into binary
            for (i = 0; n > 0; i++) {
                a[i] = n % 2;
                n = n / 2;
            }

            cout << "Binary number" << "\n";
            for (i = i - 1; i >= 0; i--) {
                cout << a[i];
                lengthA++;
            }

            cout << "\n";
            cout << "\n";

            if (b >= lengthA) {
                surplus = b - lengthA;
                for (int j = b; j < 8; j++) {
                    writeBit(0); //write the surplus' 0s
                }
                for (int k = lengthA; k > 0; k--) {
                    writeBit(a[k]); //write the binary number
                }
                for (int l = 0; l < surplus; l++) {
                    writeBit(0); //write the remaining 0s
                }
            }
       }

        void readFile() {

            /**
             * Function used to read all the lines 
             * from the file
             */

            fsiF.open("test.bin", ios::binary | ios::in);
            char c;

            //read all the bytes from the binary file
            while (fsiF.get(c)) {
                for (int i = 7; i >= 0; i--)
                    cout << ((c >> i) & 1);
                    cout << "\n";
            }
            fsiF.close();
        }
};