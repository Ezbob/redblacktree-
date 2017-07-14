#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

namespace RedBlackTree {

	class duplicatedKeyError: public std::exception {
		virtual const char *what() const throw() {
			return "Duplicated key error; Red black tree does not support duplicated keys";
		} 
	} duplicatedKeyError;

	enum Color {
		BLACK,
		RED
	};

	template<typename U, typename R>
	struct Node {
		
		enum Color color;
		
		U key;
		R *data;
		struct Node *left;
		struct Node *right;
		struct Node *parent;

		Node() : color(Color::BLACK), left(this), right(this), parent(this) {}

		Node(U k, R *d) : color(Color::BLACK), key(k), data(d) {
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}

		Node(U k, R *d, struct Node *parent, struct Node *left, struct Node *right) : color(Color::BLACK), 
			key(k), data(d), left(left), right(right), parent(parent) {}

		template<typename Q, typename L>
		friend std::ostream& operator<< (std::ostream &os, Node<Q, L> &n) {
			std::vector<std::string> ss;
			recursive_print(&n, os, ss);
			return os;
		}
		static void recursive_print(struct Node<U, R> *n, std::ostream &os, std::vector<std::string> &ss);
		static struct Node<U, R> *nill;
	};

	template<typename K, typename V>
	class Tree {

		void rotateLeft( Node<K, V> * );
		void rotateRight( Node<K, V> * );
		void transplantTree( Node<K, V> *, Node<K,V> * );
		void insertFixup( Node<K, V> * );
		void removeFixup( Node<K, V> * );
		Node<K,V> *minKeyNode( Node<K,V> * ); 
		inline Node<K, V> *getFrom( Node<K, V> *, K );

		Node<K, V> *root;

	public:	

		Tree() : root{Node<K, V>::nill} {}

		V *minimum();
		V *maximum();

		void insert(const K key, V *value);
		V *get(const K key);
		bool remove(K key);

		template<typename M, typename N>
		friend std::ostream &operator<<(std::ostream& os, Tree<M, N> &t) {
			return os << *t.root;
		}
	};
}

#endif