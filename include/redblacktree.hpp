#ifndef RB_TREE_H
#define RB_TREE_H
#include <iostream>
#include <sstream>
#include <vector>

namespace RedBlackTree {

	#define RBT_IS_RED(node) (node != nullptr && node->color == RedBlackTree::Color::RED)
	#define RBT_IS_BLACK(node) (node == nullptr || node->color == RedBlackTree::Color::BLACK)


	enum Color {
		BLACK, RED
	};

	class duplicatedKeyError: public std::exception {

		virtual const char *what() const throw() {
			return "Duplicated key error; Red black tree does not support duplicated keys";
		} 

	} duplicatedKeyError;

	std::string ToString(Color color) {
		switch(color) {
			case Color::RED:
				return std::string{"RED"};
			case Color::BLACK:
				return std::string{"BLACK"};
			default:
				return std::string{};
		}
	}

	template<typename K, typename V>
	class Tree {

		template<typename U, typename R>
		struct Node {
			enum RedBlackTree::Color color;
			U key;
			R *data;
			struct Node *left;
			struct Node *right;
			struct Node *parent;

			Node() : color(Color::BLACK) {}

			Node(U k, R *d) : color(Color::BLACK), key(k), data(d) {
				left = nullptr;
				right = nullptr;
				parent = nullptr;
			}

			Node(U k, R *d, struct Node *parent, struct Node *left, struct Node *right) : color(Color::BLACK), 
				key(k), data(d), parent(parent), left(left), right(right) {}

			template<typename Q, typename L>
			friend std::ostream& operator<< (std::ostream &os, Node<Q, L> &n) {
				std::vector<std::string> ss;
				recursive_print(&n, os, ss);
				return os;
			}

		private:
			static void recursive_print(Node<U, R> *n, std::ostream &os, std::vector<std::string> &ss);
		};

		Node<K, V> *root;

		void rotateLeft( Node<K, V> * );
		void rotateRight( Node<K, V> * );
		void transplantTree( Node<K, V> *, Node<K,V> * );
		void insertFixup( Node<K, V> * );
		void removeFixup( Node<K, V> * );
		Node<K,V> *minKeyNode( Node<K,V> * );  

	public:	

		Tree() : root{nullptr} {}

		V *minimum();
		V *maximum();

		void insert(const K key, V *value);
		V *get(const K key);
		K *getKey(const V value);
		bool remove(K key);

		template<typename M, typename N>
		friend std::ostream &operator<<(std::ostream& os, Tree<M, N> &t) {
			return os << *t.root;
		}
	};
}

#endif