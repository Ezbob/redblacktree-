#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <iomanip>
#include "redblacktree.hpp"
using namespace RedBlackTree;


template<typename K, typename V>
RedBlackTree::Tree<K, V>::Node<K, V>* RedBlackTree::Tree<K, V>::minKeyNode(RedBlackTree::Tree<K, V>::Node<K, V> *n) {
	if (n == nullptr) {
		return nullptr;
	} else if (n->left == nullptr) {
		return n;
	}
	Node<K, V> *iterator = n->left;
	while (iterator->left != nullptr) {
		iterator = iterator->left;
	}
	return iterator;
}


template<typename K, typename V>
template<typename U, typename R>
void Tree<K, V>::Node<U, R>::recursive_print(Node<U, R> *n, std::ostream &os, std::vector<std::string> &ss) {

    if ( n == nullptr ) {
        os << "NULL(BLACK)\n";
        return;
    } else {
        os << "Node(" << ToString(n->color) << ", " << n->key << ", " << *n->data << ")\n";    
    }

    for (auto str : ss) {
    	os << str;
    }

    os << " ├──";
    ss.push_back(std::string{" │ "});
    recursive_print(n->left, os, ss);
    ss.pop_back();

    for (auto str : ss) {
    	os << str;
    }

    os << " └──";
    ss.push_back(std::string{"   "});
    recursive_print(n->right, os, ss);
    ss.pop_back();    
}

template<typename K, typename V>
void Tree<K, V>::insert(const K key, V *value) {
	Node<K, V> *n = new Node<K, V>{key, value};
	Node<K, V> *iterator = this->root;

	while ( iterator != nullptr ) { // traversal of the tree finding parent of node
        n->parent = iterator;
        if ( n->key < iterator->key ) {
            iterator = iterator->left;
        } else if ( n->key > iterator->key ) {
            iterator = iterator->right;
        } else {
        	throw duplicatedKeyError;
        }
    }

    if ( n->parent == nullptr ) {
    	this->root = n;
    } else if ( n->key < n->parent->key  ) {
    	n->parent->left = n;
    } else {
    	n->parent->right = n;
    }

    n->color = RedBlackTree::Color::RED;
    this->insertFixup(n);
}

template<typename K, typename V>
bool Tree<K, V>::remove(K key) {
	Node<K, V> *node = this->root;

	std::cout << " . . " << node << std::endl;

    while ( node != nullptr && node->key != key ) {
        if ( key < node->key ) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if ( node == nullptr ) {
    	std::cout << "false\n";
    	return false;
    }

    Node<K, V> *point;
    Node<K, V> *old = node;
    auto old_color = node->color;

    if ( node->right == nullptr && node->left == nullptr ) {
        // this step is needed since the book uses the T.nill difinition
        // which is always black and is a allocated object (in contrast to our nullptr) 
    	std::cout << "b" << std::endl;
        transplantTree(node, nullptr);
        std::cout << "COMPLETE" << node << std::endl;
    } else if ( node->left == nullptr ) {
    	std::cout << "r" << node->right << std::endl;
        point = node->right;
        transplantTree(node, node->right);       
    } else if ( node->right == nullptr ) {
    	std::cout << "l" << node->left << std::endl;
        point = node->left;
        transplantTree(node, node->left);
    } else {
    	std::cout << "n" << std::endl;
        old = minKeyNode( node->right );

        std::cout << old->key << "!\n";
        old_color = old->color;
        point = old->right;

        if ( old->parent == node ) {
            if ( point != nullptr ) {
                point->parent = old;    
            }
        } else {
            transplantTree(old, old->right);
            old->right = node->right;
            old->right->parent = old;
        }
        transplantTree(node, old);
        old->left = node->left;
        old->left->parent = old;
        old->color = node->color;
    }
    if (point == nullptr) std::cout << " next\n";
    std::cout << " hehe\n";
    if ( old_color == Color::BLACK ) {
    	std::cout << "fixing this\n";
    	removeFixup(point);
    }
    std::cout << "FIXED IT\n";
    delete node;

    return true;
}


template<typename K, typename V>
V *Tree<K, V>::minimum() {
	Node<K, V> *iter = this->root;

	while ( iter != nullptr and iter->left != nullptr ) {
		iter = iter->left;
	}

	return iter->data;
}

template<typename K, typename V>
V *Tree<K, V>::maximum() {
	Node<K, V> *iter = this->root;

	while ( iter != nullptr and iter->right != nullptr ) {
		iter = iter->right;
	}

	return iter->data;
}

template<typename K, typename V>
V *Tree<K, V>::get(const K key) {
	Node<K, V> *iter = this->root;

	while ( iter != nullptr ) {
		if ( iter->key == key ) {
			return iter->data;
		} else if ( iter->key < key ) {
			iter = iter->left;
		} else {
			iter = iter->right;
		}
	}

	return nullptr;
}

template<typename K, typename V>
void Tree<K, V>::rotateLeft(Node<K, V> *node) {
	if ( node->right != nullptr ) {
        Node<K, V> *rightNode = node->right;
        node->right = rightNode->left;

        if ( rightNode->left != nullptr ) {
            rightNode->left->parent = node;
        }
        rightNode->parent = node->parent;

        if ( node->parent == nullptr ) {
            this->root = rightNode;
        } else if ( node == node->parent->left ) {
            node->parent->left = rightNode;
        } else {
            node->parent->right = rightNode;
        }
        rightNode->left = node;
        node->parent = rightNode;
    }
}

template<typename K, typename V>
void Tree<K, V>::rotateRight(Node<K, V> *node) {
	if ( node->left != nullptr ) {
        Node<K, V> *leftNode = node->left;
        node->left = leftNode->right;
        
        if ( leftNode->right != nullptr ) {
            leftNode->right->parent = node;
        }
        leftNode->parent = node->parent;
    
        if ( node->parent == nullptr ) {
            this->root = leftNode;
        } else if ( node == node->parent->left ){
            node->parent->left = leftNode;
        } else {
            node->parent->right = leftNode;
        }
        leftNode->right = node;
        node->parent = leftNode;
    }
}

template<typename K, typename V>
void Tree<K, V>::transplantTree( Node<K, V> *old, Node<K,V> *transplant ) {
	if ( old->parent == nullptr ) {
        this->root = transplant;
    } else if ( old == old->parent->left ) {
        old->parent->left = transplant;
    } else {
        old->parent->right = transplant;
    }
	if ( transplant != nullptr ) {
		transplant->parent = old->parent;   
	} 
}

template<typename K, typename V>
void Tree<K, V>::insertFixup(Node<K, V> *node) {
	if ( node == nullptr ) {
		return;
	}
	while ( RBT_IS_RED( node->parent ) ) {
        if ( node->parent == node->parent->parent->left ) {
            Node<K, V> *right_node = node->parent->parent->right;
            if ( RBT_IS_RED( right_node ) ) {
                // uncle and parent of node is red -> color them black
                node->parent->color = Color::BLACK;
                right_node->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
                continue;
            } else if ( node == node->parent->right ) {
                // uncle is black
                node = node->parent;
                rotateLeft(node);
            }

            node->parent->color = Color::BLACK;
            node->parent->parent->color = Color::RED;
            rotateRight(node->parent->parent);
        } else {
            Node<K, V> *left_node = node->parent->parent->left;
            if ( RBT_IS_RED( left_node ) ) {
                node->parent->color = Color::BLACK;
                left_node->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
                continue;
            } else if ( node == node->parent->left ) {
                node = node->parent;
                rotateRight(node);
            }
            node->parent->color = Color::BLACK;
            node->parent->parent->color = Color::RED;
            rotateLeft(node->parent->parent);
        }
    }
    root->color = Color::BLACK;
}

template<typename K, typename V>
void Tree<K, V>::removeFixup(Node<K, V> *node) {
	std::cout << node->parent << " <<-\n";
	while ( RBT_IS_BLACK( node ) && node != this->root ) {

		if ( node == node->parent->left ) {

            Node<K, V> *sibling = node->parent->right;
            if ( RBT_IS_RED( sibling ) ) {
                sibling->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateLeft( node->parent );
                sibling = node->parent->right;
            }
            if ( RBT_IS_BLACK( sibling->left ) && RBT_IS_BLACK( sibling->right ) ) {
                sibling->color = Color::RED;
                node = node->parent;
            } else if ( RBT_IS_BLACK( sibling->right ) ) {
                sibling->left->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateRight( sibling );
                sibling = node->parent->right;
            }
            sibling->color = node->parent->color;
            node->parent->color = Color::BLACK;
            sibling->right->color = Color::BLACK;
            rotateLeft( node->parent );
            node = this->root;
        } else {
            Node<K, V> *sibling = node->parent->left;
            if ( RBT_IS_RED( sibling ) ) {
                sibling->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateRight( node->parent );
                sibling = node->parent->left;
            }
            if ( RBT_IS_BLACK( sibling->right ) && RBT_IS_BLACK( sibling->left ) ) {
                sibling->color = Color::RED;
                node = node->parent;
            } else if ( RBT_IS_BLACK( sibling->left ) ) {
                sibling->right->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateLeft( sibling );
                sibling = node->parent->left;
            }
            sibling->color = node->parent->color;
            node->parent->color = Color::BLACK;
            sibling->left->color = Color::BLACK;
            rotateRight( node->parent );
            node = this->root;
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
		} else {
			t.insert(elm, &v);	
		}
	}

	std::cout << t << std::endl;

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