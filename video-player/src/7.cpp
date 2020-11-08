#include "Golomb.cpp"
#include<vector>

using namespace std;

int main(void) 
{
    Golomb golomb("teste.txt", 4, 'w');

    vector<bool> v = golomb.encode(-20);

    for (int i = 0; i < v.size(); i++) {
        cout << v.at(i) << "\n";
    }

    cout << golomb.decode(v);
}