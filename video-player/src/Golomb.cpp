#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

class Golomb {
    public:
        string filename;
        char encodingType;
        int m;
        int q;
        int r;

        Golomb(string fn, int m, char type) {
            filename = fn;
            encodingType = type;
            this->m = m;
        }

        vector<bool> encode(int n) {
            vector<bool> codedNumber;

            if (n < 0) {
                codedNumber.push_back(1);
                n = abs(n);
            }
            else codedNumber.push_back(0);

            q = floor(n / m);
            r = n - q * m;

            for (int i = 0; i < q; i++) {
                codedNumber.push_back(1);
            }
            codedNumber.push_back(0);

            bool a[8];
            int i;

            for (i = 0; r > 0; i++) {
                a[i] = r % 2;
                r = r / 2;
            }

            for (i = i - 1; i >= 0; i--) {
                codedNumber.push_back(a[i]);
            }

            return codedNumber;
        }
};