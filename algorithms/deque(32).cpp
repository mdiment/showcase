// Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.

// 3_2. Реализовать дек с динамическим зацикленным буфером.
// Требования: Дек должен быть реализован в виде класса.

#include <iostream>
#include <assert.h>
#include <cstring>
#include <sstream>

class deque{
private:
    int *dq = nullptr;
    int head = 0;
    int tail = 0;
    size_t buffer_size = 2;
    void grow();
public:
    deque() = default;
    ~deque();
    deque(const deque& ) = delete;
    deque& operator = ( const deque& ) = delete;

    int push_front(int new_elem);
    int pop_front();
    int push_back(int new_elem);
    int pop_back();
    // void print( std::ostream& output );
    bool is_empty();
};

deque::~deque(){
    delete [] dq;
}

// void deque::print( std::ostream& output ){
//     if (head == tail){
//         if(is_empty()){
//             output << "deque is empty" << std::endl;
//         }
//         else{
//             output << pop_front() << " ";
//         }
//     }
//     for(int i = head; i != tail; i + 1 < buffer_size ? ++i : i = 0){
//             output << dq[i] << " ";
//     }
//     return;
// }

bool deque::is_empty(){ return (dq == nullptr);}

void deque::grow(){
    int *temp = dq;
    dq = new int[buffer_size * 2];
    memcpy(dq, temp + head, (buffer_size - head) * sizeof(int));
    memcpy(dq + buffer_size - head, temp, tail * sizeof(int));
    
    head = 0;
    tail = buffer_size;
    buffer_size *= 2;
    delete [] temp;
}

int deque::push_front(int new_elem){
    if (head == tail)
        if (is_empty()){ 
            dq = new int[buffer_size];
        }else{
            grow();
        }
    if (head){ --head;}
    else{ head = buffer_size - 1;}
    dq[head] = new_elem;
    return 0;
}

int deque::pop_front(){
    int result = dq[head];
    if (head < buffer_size - 1){ head++; }
    else{ head = 0; }
    return result;
}

int deque::push_back(int new_elem){
    if (head == tail)
        if (is_empty()){ 
            dq = new int[buffer_size];
        }else{
            grow();
        }
    
    dq[tail] = new_elem;
    if (tail + 1 == buffer_size){ tail = 0;}
    else{ ++tail;}
    return 0;
}

int deque::pop_back(){
    if (tail){ tail--; }
    else{ tail = buffer_size - 1; }
    return dq[tail];
}

void run( std::istream& input, std::ostream& output ){
    int n = 0;
    bool flag = true;
    deque deque_obj;
    input >> n;
    for(int i = 0; i < n; ++i){
        int command = 0;
		int value = 0;
		input >> command >> value;
		switch( command ) {
        case 1:
            deque_obj.push_front(value);
            break;
		case 2:
			if( deque_obj.is_empty() )
				flag = flag && value == -1;
			else
				flag = flag && deque_obj.pop_front() == value;
			break;
		case 3:
			deque_obj.push_back(value);
			break;
        case 4:
            if( deque_obj.is_empty() )
				flag = flag && value == -1;
            else
 			    flag = flag && deque_obj.pop_back() == value;
			break;
		default:
			assert( false );
		}
    }
    output << (flag ? "YES" : "NO") << std::endl;
    // deque_obj.print(output);
}

void testKstat()
{
	{
		std::stringstream input;
		std::stringstream output;
		input << "3 1 44 3 50 2 44";
		run( input, output );
        assert( output.str() == "YES\n50 " );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "2 2 -1 1 10";
		run( input, output );
        assert( output.str() == "YES\n10 " );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "2 3 44 4 66";
		run( input, output );
        assert( output.str() == "NO\n44 " );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "8 2 -1 4 -1 1 2 3 4 4 4 1 8 1 10 3 5";
		run( input, output );
        assert( output.str() == "YES\n10 8 2 5 " );
	}
    {
		std::stringstream input;
		std::stringstream output;
		input << "9 2 -1 4 -1 1 2 3 4 4 4 1 8 3 7 1 10 3 5";
		run( input, output );
        assert( output.str() == "YES\n10 8 2 5 " );
	}
}

int main(){
    // testKstat();
    run(std::cin, std::cout);
    return 0;
}