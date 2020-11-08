#include "Golomb.cpp"
#include"BitStream.cpp"
#include<vector>

using namespace std;

int main(void) 
{
    Golomb golomb("teste.txt", 2, 'w');

    vector<bool> v = golomb.encode(13);
    vector<bool> v2 = golomb.encode(10);

    cout << "encoded number: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v.at(i);
    }

    cout << "\n" << "decoded number: " << golomb.decode(v) << "\n";

    BitStream bsw("testGolomb.bin", 'w');

    cout << "\n";

    for (int i = 0; i < v.size(); i++) {
        bsw.writeBit(v.at(i));
        cout << v.at(i);
    }

    cout << "\n";

    for (int i = 0; i < v2.size(); i++) {
        bsw.writeBit(v2.at(i));
        cout << v2.at(i);
    }
    cout << "\n";
    cout << "\n";

    /*
    bsw.writeBit(1);
    bsw.writeBit(0);
    bsw.writeBit(1);
    bsw.writeBit(1);
    bsw.writeBit(1);
    bsw.writeBit(1);
    bsw.writeBit(0);
    bsw.writeBit(0);
    */

    bsw.closeFO();

    BitStream bsr("testGolomb.bin", 'r');

    cout << "\n";
    cout << "\n";

    bsr.readFile();
    bsr.closeFI();

}