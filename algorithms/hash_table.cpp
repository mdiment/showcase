// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
// 1_2. Для разрешения коллизий используйте двойное хеширование.

#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;

struct StringHasher {
	unsigned int operator()( const string& str ) const {
		unsigned int hash = 0;
		for( unsigned int i = 0; i < str.size(); ++i )
			hash = hash * 17 + str[i];
			// hash = hash + str[i];
		return hash;
	}
};

enum state {empty_state, data_state, del_state};

template<class T, class H>
class HashTable {
public:
	explicit HashTable( const H& _hasher );
	HashTable( const HashTable& table ) = delete;
	HashTable& operator=( const HashTable& table ) = delete;
	~HashTable();

	// Если Del - идём дальше
	// Если Data - Data == data ? return true : идём дальше
	// Если Empty - return false
	bool Has( const T& key ) const;
	// Если Del - запоминаем позицию если это первый Del, идём дальше
	// Если Data - Data == data ? return false : идём дальше
	// Если Empty - вставляем в запомненную Del позицию, если была, или в текущую в ином случае
	bool Add( const T& key );
	// Если Del - идём дальше
	// Если Data - Data == data ? помечаем Del и return true : идём дальше
	// Если Empty - return false
	bool Delete( const T& key );

private:
	struct HashTableString {
		T Data;
		unsigned int Hash;
		state TStringState = empty_state;

		HashTableString() : Hash(0), TStringState(empty_state) {}
		HashTableString( const T& data, unsigned int hash, state new_state ) :
			Data( data ), Hash(hash), TStringState( new_state ) {}
	};
	
	H hasher;
	vector<HashTableString> table;
	unsigned int keysCount;

	void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable( const H& _hasher ) :
	hasher( _hasher ),
	table( 8 ),
	keysCount( 0 )
{
}

template<class T, class H>
HashTable<T, H>::~HashTable() {}

template<class T, class H>
bool HashTable<T, H>::Has( const T& data ) const
{
	size_t size = table.size();

	unsigned int absHash = hasher( data );
	unsigned int colision_hash = absHash % size;

	HashTableString element = table[colision_hash];
	state element_state = element.TStringState;
	
	if ((element_state == data_state) && (element.Data == data)){
		return true;
	}
	if (element_state == empty_state){
		return false;
	}

	unsigned int absHashStep = 2 * absHash + 1;

	for(int i = 1; i < size; ++i){
		colision_hash = (absHash + i * absHashStep) % size;
		element = table[colision_hash];
		element_state = element.TStringState;

		switch (element_state)
		{
		case del_state:
		{
			break;
		}
		case data_state:
		{
			if (element.Data == data){
				return true;
			}
			break;
		}
		case empty_state:
		{
			return false;
		}
		default:
			break;
		}
	}

	return false;
}

template<class T, class H>
bool HashTable<T, H>::Add( const T& data )
{
	size_t size = table.size();

	if( 4 * keysCount >= 3 * size ) {
		growTable();
		size *= 2;
	}

	unsigned int absHash = hasher( data );
	unsigned int abs_colision_hash = absHash;
	unsigned int colision_hash = abs_colision_hash % size;

	HashTableString element = table[colision_hash];
	state element_state = element.TStringState;
	int hash_del = 0;
	bool was_del_state = false;
	
	if(element_state == empty_state){
		table[colision_hash] = HashTableString( data, abs_colision_hash, state(data_state) );
		++keysCount;
		return true;
	}
	if ((element_state == data_state) && (element.Data == data)){
		return false;
	}
	if (element_state == del_state){
		hash_del = abs_colision_hash;
		was_del_state = true;
	}
	unsigned int absHashStep = 2 * absHash + 1;
	int i = 1;

	for(; i < size; ++i){
		abs_colision_hash += absHashStep;
		colision_hash = abs_colision_hash % size;

		element = table[colision_hash];
		element_state = element.TStringState;

		switch (element_state)
		{
		case del_state:
		{
			if (!was_del_state){
				hash_del = abs_colision_hash;
				was_del_state = true;
			}
			break;
		}
		case data_state:
		{
			if (element.Data == data){
				return false;
			}
			break;
		}
		case empty_state:
		{
			if (!was_del_state){
				table[colision_hash] = HashTableString( data, absHash, state(data_state) );
			}
			else{
				table[hash_del % size] = HashTableString( data, absHash, state(data_state) );
			}
			++keysCount;
			return true;
		}
		default:
			break;
		}
	}

	if ((i == size) && (was_del_state)){
		table[hash_del % size] = HashTableString( data, absHash, state(data_state) );
		++keysCount;
		return true;
	}
	return false;
}

template<class T, class H>
bool HashTable<T, H>::Delete( const T& data )
{
	size_t size = table.size();
	unsigned int absHash = hasher( data );
	unsigned int colision_hash = absHash % size;

	if ((table[colision_hash].TStringState == data_state) && (table[colision_hash].Data == data)){
		table[colision_hash].TStringState = del_state;
		--keysCount;
		return true;
	}
	if (table[colision_hash].TStringState == empty_state){
		return false;
	}

	unsigned int absHashStep = 2 * absHash + 1;

	for(int i = 1; i < size; ++i){
		colision_hash = (absHash + i * absHashStep) % size;

		switch (table[colision_hash].TStringState)
		{
		case del_state:
		{
			break;
		}
		case data_state:
		{
			if (table[colision_hash].Data == data){
				table[colision_hash].TStringState = del_state;
				--keysCount;
				return true;
			}
			break;
		}
		case empty_state:
		{
			return false;
		}
		default:
			break;
		}
	}
	return false;
}

template<class T, class H>
void HashTable<T, H>::growTable()
{
	size_t size = table.size();
	size_t new_size = size * 2;
	vector<HashTableString> new_table( new_size );

	for( unsigned int i = 0; i < size; ++i ) {
		if (table[i].TStringState == data_state){
			unsigned int abs_new_hash = table[i].Hash;
			unsigned int new_hash = abs_new_hash % new_size;
			if (new_table[new_hash].TStringState == empty_state)
				new_table[new_hash] = HashTableString(table[i].Data, table[i].Hash, state(data_state));
			else{
				unsigned int abs_colision_hash = abs_new_hash;
				unsigned int colision_hash = abs_colision_hash % new_size;
				unsigned int absHashStep = 2 * abs_new_hash + 1;
				bool found_pos = false;
				int j = 1;

				for(; j < new_size && found_pos == false; ++j){
					abs_colision_hash += absHashStep;
					colision_hash = abs_colision_hash % new_size;
					if (new_table[colision_hash].TStringState == empty_state){
						new_table[colision_hash] = HashTableString(table[i].Data, table[i].Hash, state(data_state));
						found_pos = true;
					}
				}
			}
		}
	}
	table = new_table;
}

int main()
{
	StringHasher hasher;
	HashTable<string, StringHasher> table(hasher);
	char operation = 0;
	string data = "";
	while( std::cin >> operation >> data ) {
		switch( operation ) {
		case '+':
			std::cout << (table.Add( data ) ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			std::cout << (table.Delete( data ) ? "OK" : "FAIL") << std::endl;
			break;
		case '?':
			std::cout << (table.Has( data ) ? "OK" : "FAIL") << std::endl;
			break;
		default:
			assert( false );
		}
	}
	return 0;
}