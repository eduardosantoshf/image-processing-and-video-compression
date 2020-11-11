#include "Golomb.cpp"
#include "BitStream.cpp"
#include <vector>

using namespace std;

int main(void) 
{
    Golomb golomb(2);
    Golomb g2(2);
    Golomb g4(4);
    Golomb g5(5);

    //-------------------- m = 2 --------------------//

    vector<bool> g2_1 = g2.encode(10);
    vector<bool> g2_2 = g2.encode(-3);
    vector<bool> g2_3 = g2.encode(27);
    vector<bool> g2_4 = g2.encode(-15);
    vector<bool> g2_5 = g2.encode(19);

    cout << "Encoding numbers for m = 2 (First bit represents signal: 0 - positive; 1 - negative) ..." << "\n";

    cout << "10: ";
    for (int c = 0; c < g2_1.size(); c++) cout << g2_1.at(c);
    cout << "\n" << "-3: ";
    for (int c = 0; c < g2_2.size(); c++) cout << g2_2.at(c);
    cout << "\n" << "27: ";
    for (int c = 0; c < g2_3.size(); c++) cout << g2_3.at(c);
    cout << "\n" << "-15: ";
    for (int c = 0; c < g2_4.size(); c++) cout << g2_4.at(c);
    cout << "\n" << "19: ";
    for (int c = 0; c < g2_5.size(); c++) cout << g2_5.at(c);
    
    cout << "\n";
    cout << "\n";

    cout << "Decoding numbers ..." << "\n";

    for (int c = 0; c < g2_1.size(); c++) cout << g2_1.at(c);
    cout << ": " << g2.decode(g2_1) << "\n";
    for (int c = 0; c < g2_2.size(); c++) cout << g2_2.at(c);
    cout << ": " << g2.decode(g2_2) << "\n";
    for (int c = 0; c < g2_3.size(); c++) cout << g2_3.at(c);
    cout << ": " << g2.decode(g2_3) << "\n";
    for (int c = 0; c < g2_4.size(); c++) cout << g2_4.at(c);
    cout << ": " << g2.decode(g2_4) << "\n";
    for (int c = 0; c < g2_5.size(); c++) cout << g2_5.at(c);
    cout << ": " << g2.decode(g2_5) << "\n";

    cout << "\n";
    cout << "\n";

    //-------------------- m = 4 --------------------//

    vector<bool> g4_1 = g4.encode(10);
    vector<bool> g4_2 = g4.encode(-3);
    vector<bool> g4_3 = g4.encode(27);
    vector<bool> g4_4 = g4.encode(-15);
    vector<bool> g4_5 = g4.encode(19);

    cout << "Encoding numbers for m = 4 (First bit represents signal: 0 - positive; 1 - negative) ..." << "\n";

    cout << "10: ";
    for (int c = 0; c < g4_1.size(); c++) cout << g4_1.at(c);
    cout << "\n" << "-3: ";
    for (int c = 0; c < g4_2.size(); c++) cout << g4_2.at(c);
    cout << "\n" << "27: ";
    for (int c = 0; c < g4_3.size(); c++) cout << g4_3.at(c);
    cout << "\n" << "-15: ";
    for (int c = 0; c < g4_4.size(); c++) cout << g4_4.at(c);
    cout << "\n" << "19: ";
    for (int c = 0; c < g4_5.size(); c++) cout << g4_5.at(c);
    
    cout << "\n";
    cout << "\n";

    cout << "Decoding numbers ..." << "\n";

    for (int c = 0; c < g4_1.size(); c++) cout << g4_1.at(c);
    cout << ": " << g4.decode(g4_1) << "\n";
    for (int c = 0; c < g4_2.size(); c++) cout << g4_2.at(c);
    cout << ": " << g4.decode(g4_2) << "\n";
    for (int c = 0; c < g4_3.size(); c++) cout << g4_3.at(c);
    cout << ": " << g4.decode(g4_3) << "\n";
    for (int c = 0; c < g4_4.size(); c++) cout << g4_4.at(c);
    cout << ": " << g4.decode(g4_4) << "\n";
    for (int c = 0; c < g4_5.size(); c++) cout << g4_5.at(c);
    cout << ": " << g4.decode(g4_5) << "\n";

    cout << "\n";
    cout << "\n";

    //-------------------- m = 5 --------------------//

    vector<bool> g5_1 = g5.encode(10);
    vector<bool> g5_2 = g5.encode(-3);
    vector<bool> g5_3 = g5.encode(27);
    vector<bool> g5_4 = g5.encode(-15);
    vector<bool> g5_5 = g5.encode(19);
    
    cout << "Encoding numbers for m = 5 (First bit represents signal: 0 - positive; 1 - negative) ..." << "\n";

    cout << "10: ";
    for (int c = 0; c < g5_1.size(); c++) cout << g5_1.at(c);
    cout << "\n" << "-3: ";
    for (int c = 0; c < g5_2.size(); c++) cout << g5_2.at(c);
    cout << "\n" << "27: ";
    for (int c = 0; c < g5_3.size(); c++) cout << g5_3.at(c);
    cout << "\n" << "-15: ";
    for (int c = 0; c < g5_4.size(); c++) cout << g5_4.at(c);
    cout << "\n" << "19: ";
    for (int c = 0; c < g5_5.size(); c++) cout << g5_5.at(c);
    
    cout << "\n";
    cout << "\n";

    cout << "Decoding numbers ..." << "\n";

    for (int c = 0; c < g5_1.size(); c++) cout << g5_1.at(c);
    cout << ": " << g5.decode(g5_1) << "\n";
    for (int c = 0; c < g5_2.size(); c++) cout << g5_2.at(c);
    cout << ": " << g5.decode(g5_2) << "\n";
    for (int c = 0; c < g5_3.size(); c++) cout << g5_3.at(c);
    cout << ": " << g5.decode(g5_3) << "\n";
    for (int c = 0; c < g5_4.size(); c++) cout << g5_4.at(c);
    cout << ": " << g5.decode(g5_4) << "\n";
    for (int c = 0; c < g5_5.size(); c++) cout << g5_5.at(c);
    cout << ": " << g5.decode(g5_5) << "\n";

    cout << "\n";
    cout << "\n";


    cout << "/----- Working with binary file, using BitStream class -----/" << "\n" << "\n";

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
    cout << "First decoded number: " << golomb.decode(d) << "\n";

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