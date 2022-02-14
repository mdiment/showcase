#include <iostream>

template<class T>
class IsLessDefault {
public:
	bool operator() ( const T& l, const T& r ) const { return l > r; }
};

template<class T, class IsLess = IsLessDefault<T>>
class BTree {
public:
	BTree( const IsLess& _isLess = IsLessDefault<T>()) : root( nullptr ), isLess(_isLess) {}
	~BTree() { delete root; }

	void Insert( const T& key );
    void Print();
private:
    struct Node {
		T Key;
		Node* Left = nullptr;
        Node* Right = nullptr;

		explicit Node( T key) : Key( key ) {}
		~Node() { 
            delete Left; 
            delete Right;
        }
	};
    IsLess isLess;
	Node* root;
};

template<typename T, typename IsLess>
void BTree<T, IsLess>::Insert( const T& key )
{
	if( !root )
		root = new Node( key );
    else{
        Node* temp = root;
        while (temp != nullptr){
            if (isLess(temp->Key, key)){
                if (temp->Left)
                    temp = temp->Left;
                else{
                    temp->Left = new Node(key);
                    temp = nullptr;
                }
            }
            else
                if (temp->Right)
                    temp = temp->Right;
                else{
                    temp->Right = new Node(key);
                    temp = nullptr;
                }
        }
    }
}

template <class T, class IsLess>
void BTree<T, IsLess>::Print(){
    Node* temp = root;
    while (temp) {
        Node* next = temp->Left;
        if (next) {
            temp->Left = next->Right;
            next->Right = temp;
            temp = next;
        } else {
            std::cout << temp->Key << ' ';
            temp = temp->Right;
        }
    }
}

int main(){
    BTree<int> btree;
    int n = 0, x = 0;
    std::cin >> n;

    for(int i = 0; i < n; ++i){
        std::cin >> x;
        btree.Insert(x);
    } 

    btree.Print();
}
