// 1.2 Вернуть значение бита в числе N по его номеру K.

#include <iostream>

using namespace std;

int main(){
    int n = 0, k = 0;
    cin >> n >> k;
    cout << ((n >> k) & 1);

}
