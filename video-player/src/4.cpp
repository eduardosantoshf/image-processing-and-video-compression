#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include <vector>
#include "methods.cpp"

using namespace std;
using namespace cv;

class BitStream {
    public:
        int pos = 0;
        unsigned char buff;
        const char* filename;

        BitStream(const char* fn) {
            filename = fn;
        }

        void writebit(int bit) {
            //cout << bit;
            if (bit) 
                buff |= (bit << pos);
            pos++;
            cout << pos;
            ofstream ofs("test.bin", ios::binary);

            if (pos == 8) {
                ofs.write(reinterpret_cast<const char*>(&buff), sizeof(buff));
                ofs.close();
                pos = 0;
                buff = 0;
            }
        }
        /*
        void readfile() {
            char* buffer = new char[20];
            ifstream ifsb("test.bin", ios::binary);
            ifsb.read(buffer, 20);
            if (ifsb) cout << "Read successfully!";
            else cout << "Error!";
            ifsb.close();
            //for (char x: buffer) cout << x << endl;
        }
        */
        
};

int main(void)
{
    /*
    //Test to write binary file
    vector<int> v;
    for (int i = 0 ; i < 10 ; i++) v.push_back(i * 10);

    ofstream ofsb("data.bin", ios::binary); 
    ofsb.write(reinterpret_cast<char*>(v.data()), v.size() * sizeof(int)); 
    ofsb.close();

    //Test to read binary file
    v.clear();
    v.resize(10);
    ifstream ifsb("data.bin", ios::binary); 
    ifsb.read(reinterpret_cast<char*>(v.data()), v.size() * sizeof(int)); 
    ifsb.close();
    for(auto x : v) cout << x << endl;
    */



    BitStream bs("teste.txt");
    bs.writebit(0);
    bs.writebit(1);
    bs.writebit(0);
    bs.writebit(0);
    bs.writebit(0);
    bs.writebit(0);
    bs.writebit(0);
    bs.writebit(0);
    //bs.readfile();
}

