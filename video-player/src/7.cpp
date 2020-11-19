#include "Golomb.cpp"
#include <vector>

using namespace std;

int main(void) 
{
    Golomb g2("golomb.bin", 2, 'e');
    Golomb g4("golomb.bin", 4, 'e');
    Golomb g5("golomb.bin", 5, 'e');

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

    /*
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
    */

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

    /*
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
    */

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

    /*
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
    */

   g2.close();
   g4.close();
   g5.close();

   BitStream bsr("golomb.bin", 'r');
    
    bsr.readNBits(65);

    bsr.closeFI();

}