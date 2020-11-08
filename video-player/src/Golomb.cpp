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
            vector<bool> encodedNumber;

            if (n < 0) {
                encodedNumber.push_back(1);
                n = abs(n);
            }
            else encodedNumber.push_back(0);

            q = floor(n / m);
            r = n - q * m;

            for (int i = 0; i < q; i++) {
                encodedNumber.push_back(1);
            }
            encodedNumber.push_back(0);

            bool a[8];
            int i;

            //convert r into binary
            for (i = 0; r > 0; i++) {
                a[i] = r % 2;
                r = r / 2;
            }

            for (i = i - 1; i >= 0; i--) {
                encodedNumber.push_back(a[i]);
            }

            return encodedNumber;
        }

        int decode(vector<bool> encodedNumber) {
            bool signal = encodedNumber.at(0);

            int i = 1;
            int c = 0;
            int q, r;
            string sbinary = "";

            vector<bool> v;

            if (encodedNumber.at(1) == 0) {
                q = 0;
                for (int j = 1; j < encodedNumber.size(); j++) {
                    v.push_back(encodedNumber.at(j));
                    sbinary += to_string(encodedNumber.at(j));
                }
            } 
            else {
                while (encodedNumber.at(i) == 1) i++;
                i--;
                for (int k = i + 1; k < encodedNumber.size(); k++) {
                    v.push_back(encodedNumber.at(k));
                    sbinary += to_string(encodedNumber.at(k));
                }
                q = i;
            }

            //convert r from binary to decimal
            r = stoi(sbinary, 0, 2);

            if (signal) {
                return (-1 * (r + q * m));
            } else {
                return (r + q * m);
            }
        }
};