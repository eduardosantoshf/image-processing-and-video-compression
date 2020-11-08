#include<iostream>
#include<fstream>
#include<bitset>

using namespace std;

class BitStream {
    public:
        int pos = 0;
        int pos2 = 0;
        unsigned char buff = 0;
        fstream fsi;
        fstream fso;
        fstream fsiF;
        unsigned char readBuff = 0;
        bitset<8> b;
        string filename;
        char type;

        BitStream(string fn, char op) {
            filename = fn;
            type = op;
            if (type == 'w')
                fso.open(filename, ios::binary | ios::app);
            else if (type == 'r') {
                fsi.open(filename, ios::in | ios::binary);
            }
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
                fso.write(reinterpret_cast<char*>(&buff), sizeof(buff) * sizeof(char));
                pos = 0;
                buff = 0;
            }
        }

        void closeFO() {
            fso.close();
        }

        void closeFI() {
            fsi.close();
        }

       int readBit() {
            char bit;
            /**
             * Function used to read one bit at a time from
             * the given file
             */

            if (pos2 == 0) {
                fsi.read(reinterpret_cast<char *>(&readBuff), 1);
                pos2 = 7;
            }

            cout << "pos: " << pos2 << "\n";

            b = bitset<8>(readBuff); //store the byte in a bitset
            //cout << b << "\n";

            bit = b.to_string()[pos2];
            pos2--;
            return bit - '0';
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
                cout << readBit() << "\n";
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

            fsiF.open(filename, ios::binary | ios::in);
            char c;

            //read all the bytes from the binary file
            while (fsiF.get(c)) {
                for (int i = 0; i < 8; i++)
                    cout << ((c >> i) & 1);
                    cout << "\n";
            }
            fsiF.close();
        }
};