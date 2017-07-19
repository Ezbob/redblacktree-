#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <exception>

namespace RedBlackTree {

	class duplicatedKeyError : public std::exception {
		virtual const char *what() const throw() {
			return "Duplicated key error; Red black tree does not support duplicated keys";
		} 
	};

	template<typename K, typename V>
	class Tree {
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

			~Node() {
				data = nullptr;
				left = nullptr;
				right = nullptr;
				parent = nullptr;
			}

			template<typename Q, typename L>
			friend std::ostream& operator<< (std::ostream &os, Node<Q, L> &n) {
				std::vector<std::string> ss;
				recursivePrint(&n, os, ss);
				return os;
			}
			static void recursivePrint(struct Node<U, R> *n, std::ostream &os, std::vector<std::string> &ss);
			
		};

		void rotateLeft( Node<K, V> * );
		void rotateRight( Node<K, V> * );
		void transplantTree( Node<K, V> *, Node<K,V> * );
		void insertFixup( Node<K, V> * );
		void removeFixup( Node<K, V> * );
		Node<K,V> *minKeyNode( Node<K,V> * ); 
		inline Node<K, V> *getFrom( Node<K, V> *, K );
		inline void deleteRecursively(Node<K, V> *);

		Node<K, V> *root;
		static struct Node<K, V> *const nill;

	public:

		Tree() : root{nill} {}

		~Tree() {
			deleteRecursively(root);
			if ( root != nill ) {
				delete root;
			}
		}

		V *minimum();
		V *maximum();

		void insert(K key, V *value);
		V *get(K key);
		bool remove(K key);

		template<typename M, typename N>
		friend std::ostream &operator<<(std::ostream& os, Tree<M, N> &t) {
			return os << *t.root;
		}

		void del();
	};

	template<typename K,typename V>
	Tree<K, V>::Node<K,V> *const Tree<K, V>::nill = new Tree<K, V>::Node<K, V>{};
	
}

#include "redblacktree.tpp"

#endif