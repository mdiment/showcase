// Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения массива строго возрастают, а на интервале [m, n-1] строго убывают. Найти m за O(log m). 
// Требования:  Время работы O(log m). Внимание! В этой задаче сначала нужно определить диапазон для бинарного поиска размером порядка m с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск

#include <iostream>

using namespace std;

int find_bound(int *a, int n){
    int bound = 1;
    do{
        bound *= 2;
    }while (bound < n && a[bound - 1] < a[bound]);
    if (bound > n){
        bound = n;
    }
    return bound;
}

int binary_search(int *a, int bound){
    int first = bound / 2;
    int last = bound;
    while( first < last ) {
        int mid = ( first + last ) / 2;
        if (mid == 0){
            return 0;
        }
        if( a[mid - 1] < a[mid] )
            first = mid + 1;
        else 
            last = mid;
    }
    return first - 1;
}
int main(){
    int n = 0;
    cin >> n;
    int *a = new int[n];

    for(int i = 0; i < n; ++i){
        cin >> a[i];
    }

    int bound = find_bound(a, n);
    
    cout << binary_search(a, bound);

    delete[] a;
}