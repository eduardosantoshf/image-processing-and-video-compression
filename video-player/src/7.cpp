#include "Golomb.cpp"
#include "BitStream.cpp"
#include <vector>

using namespace std;

int main(void) 
{
    Golomb golomb("teste.txt", 2, 'w');

    vector<bool> v = golomb.encode(13);
    vector<bool> v2 = golomb.encode(-10);
    int i;
    int i2;

    cout << "encoded number: ";
    for (i = 0; i < v.size(); i++) {
        cout << v.at(i);
    }
    cout << "\n" << "decoded number: " << golomb.decode(v) << "\n";
    cout << "\n" << "Writing number on file..." << "\n";
    cout << "\n";

    cout << "encoded number: ";
    for (i2 = 0; i2 < v2.size(); i2++) {
        cout << v2.at(i2);
    }
    cout << "\n" << "decoded number: " << golomb.decode(v2) << "\n";
    cout << "\n" << "Writing number on file..." << "\n";

    BitStream bsw("testGolomb.bin", 'w');

    for (int j = 0; j < v.size(); j++) {
        //bsw.writeBit(v.at(j));
    }

    for (int k = 0; k < v2.size(); k++) {
        //bsw.writeBit(v2.at(k));
    }

    bsw.closeFO();

    BitStream bsr("testGolomb.bin", 'r');

    cout << "\n";

    cout << "File content:" << "\n";
    bsr.readFile();

    cout << "\n" << "Reading and decoding first number from file..." << "\n";

    vector<bool> d;

    for (int l = 0; l < i; l++) {
        d.push_back(bsr.readBit());
    }
    cout << "First ecoded number: " << golomb.decode(d) << "\n";

    vector<bool> d2;

    for (int m = 0; m < i2; m++) {
        d2.push_back(bsr.readBit());
    }
    cout << "Second decoded number: " << golomb.decode(d2) << "\n";

    for (int s = 0; s < 17; s++) {
        //cout << bsr.readBit();
    }

    bsr.closeFI();

}