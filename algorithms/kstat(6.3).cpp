// Дано множество целых чисел из [0..109] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
//     10% перцентиль
//     медиана
//     90% перцентиль
// Требования:
//     К дополнительной памяти: O(n).
//     Среднее время работы: O(n)
//     Должна быть отдельно выделенная функция partition.
//     Рекурсия запрещена.
//     Решение должно поддерживать передачу функции сравнения снаружи.
// 6_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”. 
// Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

#include <iostream>
#include <sstream>
#include <assert.h>

template<class T>
class IsLessDefault {
public:
	bool operator() ( const T& l, const T& r ) const { return l < r; }
};

template<class T, class IsLess = IsLessDefault<T>>
int partition(T *a, int low, int high, IsLess isLess = IsLessDefault<T>()){
    int pivotPos = rand() % (high - low) + low;
	std::swap( a[high], a[pivotPos] );
    T pivot = a[high];
    int i = low;
    int j = low;
    while (j < high){
        if (isLess(a[j], pivot)){ 
            std::swap( a[j], a[i] );
            ++i; 
        }
        ++j;
    }
    std::swap( a[i], a[j] );
    return i;
}

template<class T, class IsLess = IsLessDefault<T>>
int kstat(T* a, int low, int high, int k, IsLess isLess = IsLessDefault<T>()){
    int pivotPos = 0;
    int l = low;
    int h = high;
    do{
        pivotPos = partition( a, l, h, isLess );
        if( pivotPos == k ) {
            return a[k];
        }
        if (k < pivotPos){
            h = pivotPos - 1;
        }
        else{
            l = pivotPos + 1;
        }
    }while(l != h);
    return a[l];
}

int run( std::istream& input, std::ostream& output ){
	int n = 0;
    input >> n;
    int* arr_int = new int[n];
	for(int i = 0; i < n; ++i){ input >> arr_int[i]; }
    output << kstat<int>(arr_int, 0, n - 1, n / 10) << std::endl;
    output << kstat<int>(arr_int, 0, n - 1, n / 2) << std::endl;
    output << kstat<int>(arr_int, 0, n - 1, n * 9 / 10) << std::endl;

	return 0;
}

void testKstat()
{
	{
		std::stringstream input;
		std::stringstream output;
		input << "10 1 2 3 4 5 6 7 8 9 10";
		run( input, output );
        assert( output.str() == "2\n6\n10\n" );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "5 6 0 4 5 3";
		run( input, output );
        assert( output.str() == "0\n4\n6\n" );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "15 6 0 4 5 3 20 47 13 54 32 7 92 33 15 34";
		run( input, output );
        assert( output.str() == "3\n15\n54\n" );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "10 3 2 1 4 5 9 8 10 6 7";
		run( input, output );
        assert( output.str() == "2\n6\n10\n" );
	}
}

int main(){
    // testKstat();
    run(std::cin, std::cout);
    return 0;
}