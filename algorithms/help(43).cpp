// 4.3: В операционной системе Technux есть планировщик процессов.
// Каждый процесс характеризуется:
//     приоритетом P
//     временем, которое он уже отработал t
//     временем, которое необходимо для завершения работы процесса T
// Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в очередь процессов.
// Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
// Требуется посчитать кол-во переключений процессора.
// Требования:
//     В качестве очереди с приоритетом нужно использовать кучу.
//     Куча должна быть реализована в виде шаблонного класса.
//     Решение должно поддерживать передачу функции сравнения снаружи.
//     Куча должна быть динамической.

#include <iostream>
#include <sstream>
#include <assert.h>
#include <cstring>

struct Process {
    int P = 0;
    int t = 0;
    int T = 0;

    Process() = default;
    Process(int _P, int _t, int _T) : P(_P), t(_t), T(_T){}
public:
    int gett(){ return t; }
    int getT(){ return T; }
    int getP(){ return P; }
};

bool operator > ( const Process& l, const Process& r )
{
	return l.P * ( l.t + 1) >= r.P * ( r.t + 1);
}

template<class T>
class IsLessDefault {
public:
	bool operator() ( const T& l, const T& r ) const { return l > r; }
};

template<class T, class IsLess = IsLessDefault<T>>
class Heap {
public:
	Heap( const IsLess& _isLess = IsLessDefault<T>()): isLess(_isLess), buffer(nullptr), bufferSize(2), size(0){}
	Heap( const T* arr, int size, IsLess _isLess = IsLessDefault<T>());
	Heap( const Heap& ) = delete;
	~Heap();

	Heap& operator=( const Heap& h ) = delete;

	void add( const T& el );
	const T& PeekMax() const;
	T ExtractMax();
	int Size() const { return size; };

private:
	IsLess isLess;
	T* buffer = nullptr;
	int bufferSize = 2;
	int size = 0;

	void growBuffer();
    void buildHeap();
    void siftDown( int i );
    void siftUp( int i );
};

template<typename T, typename IsLess>
Heap<T, IsLess>::Heap(const T* arr, int _size, IsLess _isLess){
    isLess = _isLess;
    size = _size;
    bufferSize = size;
    buffer = new T[bufferSize];
    memcpy(buffer, arr, _size * sizeof(T));

    buildHeap();    

}
template<typename T, typename IsLess>
Heap<T, IsLess>::~Heap(){ delete[] buffer; }

template<typename T, typename IsLess>
void Heap<T, IsLess>::growBuffer(){
    T *temp = buffer;
    buffer = new T[bufferSize * 2];
    memcpy(buffer, temp, bufferSize * sizeof(T));
    bufferSize *= 2;
    delete [] temp;
    return;
}

template<typename T, typename IsLess>
void Heap<T, IsLess>::add( const T& el ){
    if (buffer == nullptr){
        buffer = new T[bufferSize];
    }
    if (Size() == bufferSize){
        growBuffer();
    }

    buffer[size] = el;
    ++size;

    siftUp(size - 1);
    return;
}

template<typename T, typename IsLess>
void Heap<T, IsLess>::siftDown(int i){
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if( left < size && isLess(buffer[i], buffer[left]) )
        largest = left;
    if( right < size && isLess(buffer[largest], buffer[right]) )
        largest = right;
    if( largest != i ) {
        std::swap( buffer[i], buffer[largest] );
        siftDown( largest );
    }
}

template<typename T, typename IsLess>
void Heap<T, IsLess>::buildHeap(){
    for( int i = bufferSize / 2 - 1; i >= 0; --i ) {
        siftDown( i );
    }
}

template<typename T, typename IsLess>
void Heap<T, IsLess>::siftUp( int i ){
    while( i > 0 ) {
        int parent = ( i - 1 ) / 2;
        if( isLess(buffer[i], buffer[parent]) )
            return;
        std::swap( buffer[i], buffer[parent] );
        i = parent;
    }
}

template<typename T, typename IsLess>
T Heap<T, IsLess>::ExtractMax(){
    assert( size != 0 );
    T result = buffer[0];
    buffer[0] = buffer[size - 1];
    size--;
    if( size ) {
        siftDown( 0 );
    }
    return result;
}
template<typename T, typename IsLess>
int count_switches(Heap<T, IsLess>& heap){
    int n = 0;
    while(heap.Size() != 0 ){
        n++;
        T el = heap.ExtractMax();
        int t = el.t;
        int P = el.P;
        int _T = el.T;
        if (t + P < _T){ heap.add(Process(P, t + P, _T)); }
    }
    return n;
}

int run( std::istream& input, std::ostream& output )
{
	Heap<Process> heap;
	int n = 0;
    Process el;
	input >> n;
	for(int i = 0; i < n; ++i){
        int P = 0, T = 0;
        input >> P >> T;
    	el = Process(P, 0, T);
        heap.add(el);
    }

    output << count_switches(heap);
	return 0;
}

void testHeap()
{
	{
		std::stringstream input;
		std::stringstream output;
		input << "5 1 10 1 5 2 5 3 7 2 4";
		run( input, output );
        assert( output.str() == "23" );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "3 1 10 1 5 2 5";
		run( input, output );
        assert( output.str() == "18" );
	}
}

int main(){
    // testHeap();
    run(std::cin, std::cout);
    return 0;
}