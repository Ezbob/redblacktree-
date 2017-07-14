#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <iomanip>
#include "redblacktree.hpp"
using namespace RedBlackTree;

namespace RedBlackTree {
	template<typename K,typename V>
    Node<K,V> *Node<K, V>::nill = new Node<K, V>{};
}

template<typename K, typename V>
void Node<K, V>::recursive_print(Node<K, V> *n, std::ostream &os, std::vector<std::string> &ss) {

    if ( n == Node<K, V>::nill ) {
        os << "NULL(BLACK)\n";
        return;
    } else {
        os << "Node(";     
        switch(n->color) {
            case Color::RED:
                os << "RED";
                break;
            case Color::BLACK:
                os << "BLACK";
                break;
        }
        os << ", " << n->key << ", " << *n->data << ")\n";
    }

    for (auto str : ss) {
    	os << str;
    }

    os << " ├──";
    ss.push_back(std::string{" │ "});
    recursive_print(n->right, os, ss);
    ss.pop_back();

    for (auto str : ss) {
    	os << str;
    }

    os << " └──";
    ss.push_back(std::string{"   "});
    recursive_print(n->left, os, ss);
    ss.pop_back();    
}

template<typename K, typename V>
Node<K, V> *Tree<K, V>::getFrom(Node<K, V> *node, K key) {
	Node<K, V> *iter = node;
	while ( iter->key != key ) {
		if ( iter->key > key ) {
			iter = iter->left;
		} else {
			iter = iter->right;
		}
	}
	return iter;
}

template<typename K, typename V>
Node<K, V>* Tree<K, V>::minKeyNode(Node<K, V> *n) {
	if (n == Node<K, V>::nill) {
		return Node<K, V>::nill;
	} else if (n->left == Node<K, V>::nill) {
		return n;
	}
	Node<K, V> *iterator = n->left;
	while (iterator->left != Node<K, V>::nill) {
		iterator = iterator->left;
	}
	return iterator;
}

template<typename K, typename V>
void Tree<K, V>::insert(const K key, V *value) {

	Node<K, V> *n = new Node<K, V>{key, value, Node<K, V>::nill, Node<K, V>::nill, Node<K, V>::nill};
	
	Node<K, V> *iterator = this->root;
	Node<K, V> *p = Node<K, V>::nill;
	

	while ( iterator != Node<K, V>::nill ) { // traversal of the tree finding parent of node
        p = iterator;
        if ( n->key < iterator->key ) {
            iterator = iterator->left;
        } else if ( n->key > iterator->key ) {
            iterator = iterator->right;
        } else {
        	throw duplicatedKeyError;
        }
    }

    n->parent = p;
    
    if ( p == Node<K, V>::nill ) {
    	this->root = n;
    } else if ( n->key < p->key  ) {
    	p->left = n;
    } else {
    	p->right = n;
    }

    n->left = Node<K, V>::nill;
    n->right = Node<K, V>::nill;
    n->color = Color::RED;
    this->insertFixup(n);
}

template<typename K, typename V>
bool Tree<K, V>::remove(K key) {
	Node<K, V> *node;
	node = getFrom(this->root, key);

    if ( node == Node<K, V>::nill ) {
    	return false;
    }

    Node<K, V> *point = Node<K, V>::nill;
    Node<K, V> *min = node;
    auto minColor = min->color;

	if ( node->left == Node<K, V>::nill ) {
        point = node->right;
        transplantTree(node, node->right);     
    } else if ( node->right == Node<K, V>::nill ) {
        point = node->left;
        transplantTree(node, node->left);
    } else {
        min = minKeyNode( node->right );

        minColor = min->color;
        point = min->right;

        if ( min->parent == node ) {
            point->parent = min;
        } else {
            transplantTree(min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }
        transplantTree(node, min);
        min->left = node->left;
        min->left->parent = min;
        min->color = node->color;
    }
    
    if ( minColor == Color::BLACK ) {
    	removeFixup(point);
    }
    delete node;

    return true;
}


template<typename K, typename V>
V *Tree<K, V>::minimum() {
	Node<K, V> *iter = this->root;

	while ( iter != Node<K, V>::nill and iter->left != Node<K, V>::nill ) {
		iter = iter->left;
	}

	return iter->data;
}

template<typename K, typename V>
V *Tree<K, V>::maximum() {
	Node<K, V> *iter = this->root;

	while ( iter != Node<K, V>::nill and iter->right != Node<K, V>::nill ) {
		iter = iter->right;
	}

	return iter->data;
}

template<typename K, typename V>
V *Tree<K, V>::get(const K key) {
	Node<K, V> *iter = getFrom(this->root, key);

	if (iter != Node<K, V>::nill) {
		return iter->data;
	}
	return nullptr;
}

template<typename K, typename V>
void Tree<K, V>::rotateLeft(Node<K, V> *node) {

    Node<K, V> *rightNode = node->right;
    node->right = rightNode->left;

    if ( rightNode->left != Node<K, V>::nill ) {
        rightNode->left->parent = node;
    }
    rightNode->parent = node->parent;

    if ( node->parent == Node<K, V>::nill ) {
        this->root = rightNode;
    } else if ( node == node->parent->left ) {
        node->parent->left = rightNode;
    } else {
        node->parent->right = rightNode;
    }
    rightNode->left = node;
    node->parent = rightNode;
}

template<typename K, typename V>
void Tree<K, V>::rotateRight(Node<K, V> *node) {

    Node<K, V> *leftNode = node->left;
    node->left = leftNode->right;
    
    if ( leftNode->right != Node<K, V>::nill ) {
        leftNode->right->parent = node;
    }
    leftNode->parent = node->parent;

    if ( node->parent == Node<K, V>::nill ) {
        this->root = leftNode;
    } else if ( node == node->parent->left ){
        node->parent->left = leftNode;
    } else {
        node->parent->right = leftNode;
    }
    leftNode->right = node;
    node->parent = leftNode;
}

template<typename K, typename V>
void Tree<K, V>::transplantTree( Node<K, V> *old, Node<K,V> *transplant ) {
	if ( old->parent == Node<K, V>::nill ) {
        this->root = transplant;
    } else if ( old == old->parent->left ) {
        old->parent->left = transplant;
    } else {
        old->parent->right = transplant;
    }
	transplant->parent = old->parent;
}

template<typename K, typename V>
void Tree<K, V>::insertFixup(Node<K, V> *node) {
	while ( node->parent->color == Color::RED ) {
        if ( node->parent == node->parent->parent->left ) {
            Node<K, V> *right_node = node->parent->parent->right;
            if ( right_node->color == Color::RED ) {
                // uncle and parent of node is red -> color them black
                node->parent->color = Color::BLACK;
                right_node->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
                
            } else if ( node == node->parent->right ) {
                // uncle is black
                node = node->parent;
                rotateLeft(node);
            } else {
            	node->parent->color = Color::BLACK;
            	node->parent->parent->color = Color::RED;
            	rotateRight(node->parent->parent);
            }

        } else {
            Node<K, V> *left_node = node->parent->parent->left;
            if ( left_node->color == Color::RED ) {
                node->parent->color = Color::BLACK;
                left_node->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
                
            } else if ( node == node->parent->left ) {
                node = node->parent;
                rotateRight(node);
            } else {
            	node->parent->color = Color::BLACK;
            	node->parent->parent->color = Color::RED;
            	rotateLeft(node->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

template<typename K, typename V>
void Tree<K, V>::removeFixup(Node<K, V> *node) {

	while ( node->color == Color::BLACK && node != this->root ) {
		if ( node == node->parent->left ) {
            Node<K, V> *sibling = node->parent->right;
            if ( sibling->color == Color::RED ) {

                sibling->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateLeft( node->parent );
                sibling = node->parent->right;
            } 
            if ( sibling->left->color == Color::BLACK && 
            	sibling->right->color == Color::BLACK ) {
                
                sibling->color = Color::RED;
                node = node->parent;
            } else if ( sibling->right->color == Color::BLACK ) {

                sibling->left->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateRight( sibling );
                sibling = node->parent->right;
            } else {
            	sibling->color = node->parent->color;
	            node->parent->color = Color::BLACK;
	            sibling->right->color = Color::BLACK;
	            rotateLeft( node->parent );
	            node = this->root;
            }

        } else if ( node == node->parent->right ) {
            Node<K, V> *sibling = node->parent->left;
            if ( sibling->color == Color::RED ) {
                sibling->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateRight( node->parent );
                sibling = node->parent->left;
            }
            if ( sibling->right->color == Color::BLACK && 
            	sibling->left->color == Color::BLACK ) {
                sibling->color = Color::RED;
                node = node->parent;
            } else if ( sibling->left->color == Color::BLACK ) {
                sibling->right->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateLeft( sibling );
                sibling = node->parent->left;
            } else {
            	sibling->color = node->parent->color;
	            node->parent->color = Color::BLACK;
	            sibling->left->color = Color::BLACK;
	            rotateRight( node->parent );
	            node = this->root;
            }
        } 
    }
    node->color = Color::BLACK;
}


int main()
{
	Tree<int, std::string> t;
	std::string v{"hello"};
	std::string v2{"hello23"};
	std::string v3{"Plah"};

	auto a = {2, 3, 42, 1, 23, 0, -1};

	for ( auto elm : a ) {
		if ( elm == 42) {
			t.insert(elm, &v3);
		}  else if (elm == -1) {
			t.insert(elm, &v2);
		} else {
			t.insert(elm, &v);	
		}
	}

	std::cout << t << std::endl;

	std::cout << "min: " << *t.minimum() << " max: " << *t.maximum() << std::endl; 

	std::cout << "--->" << *t.get(-1) << std::endl;

	std::cout << *t.maximum() << std::endl;

	for ( auto elm : a ) {
		std::cout << "removing element: " << elm << std::endl;
		if ( t.remove(elm) ) {
			std::cout << "Huzzarh!" << std::endl;
			std::cout << t << std::endl;
		} else {
			std::cout << "Buhuh" << std::endl;
		}
	}

	return 0;
}