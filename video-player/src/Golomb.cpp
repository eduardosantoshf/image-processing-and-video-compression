#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

class Golomb {
    public:
        int m;

        Golomb(int m) {
            this->m = m;
        }

        vector<bool> base2Encode(int n) {
            /**
             * Function to encode a given number using golomb 
             * coding, given a parameter m, power of 2
             * 
             * @param n number we want to encode
             * @return encoded number, stored in a vector
             */
            vector<bool> encodedNumber;

            if (n < 0) {
                encodedNumber.push_back(1);
                n = abs(n);
            }
            else encodedNumber.push_back(0);

            int q = floor(n / m);
            int r = n - q * m;

            //unary code 
            for (int i = 0; i < q; i++) encodedNumber.push_back(1);

            encodedNumber.push_back(0);

            vector<bool> v;
            int i;

            //convert r into binary
            for (i = 0; r > 0; i++) {
                bool a = r % 2;
                v.push_back(a);
                r = r / 2;
            }

            for (i = i - 1; i >= 0; i--) encodedNumber.push_back(v.at(i));

            return encodedNumber;
        }
        
        int base2Decode(vector<bool> encodedNumber) {
            /**
             * Function to decode a given number using 
             * golomb coding, given a parameter m, 
             * power of 2
             * 
             * @param encodedNumber number we want to decode, stored
             * in a vector
             * @return decoded number
             */
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

        vector<bool> truncatedEncode(int n) {
            /**
             * Function to encode a given number using golomb 
             * coding, given a parameter m, not power of 2
             * 
             * @param n number we want to encode
             * @return encoded number, stored in a vector
             */
            int b = ceil(log2(m));

            vector<bool> encodedNumber;

            if (n < 0) {
                encodedNumber.push_back(1);
                n = abs(n);
            }
            else encodedNumber.push_back(0);

            int q = floor(n / m);
            int r = n - q * m;
            
            //unary code
            for (int i = 0; i < q; i++) encodedNumber.push_back(1);

            encodedNumber.push_back(0);

            //binary code
            if (r < (pow(2, b) - m)) {
                vector<bool> v;
                int bits = b - 1;
                int num = r;
                int j;

                //transform num into binary
                for (j = 0; num > 0; j++) {
                    bool a = num % 2;
                    v.push_back(a);
                    num = num / 2;
                }

                //add 0s to binary number
                while (v.size() < bits) v.push_back(0);

                for (int o = v.size() - 1; o >= 0; o--) encodedNumber.push_back(v.at(o));

            }
            else {
                vector<bool> v2;
                int bits2 = b;
                int num2 = r + pow(2, b) - m;
                int k;

                //transform num2 into binary
                for (k = 0; num2 > 0; k++) {
                    bool a = num2 % 2;
                    v2.push_back(a);
                    num2 = num2 / 2;
                }

                //add 0s to binary number
                while (v2.size() < bits2) v2.push_back(0);

                for (int o = v2.size() - 1; o >= 0; o--) encodedNumber.push_back(v2.at(o));
            }

            return encodedNumber;   
        }

        int truncatedDecode(vector<bool> encodedNumber) {
            /**
             * Function to decode a given number using 
             * golomb coding, given a parameter m, 
             * not power of 2
             * 
             * @param encodedNumber number we want to decode, stored
             * in a vector
             * @return decoded number
             */
            int b = ceil(log2(m));
            int q, r, i = 1;
            string sbinary = "";
            int signal;

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

            //check if number is positive | negative
            if (encodedNumber.at(0) == 0)
                signal = 1;
            else
                signal = -1;
            
            if (r < (pow(2, b) - m))
                return signal * (r + q * m);
            else
                return signal * (r + m - pow(2, b) + q * m); 
        }
};