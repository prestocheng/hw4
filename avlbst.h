#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* c);
    virtual void rotateRight(AVLNode<Key,Value>* g);
    virtual void rotateLeft(AVLNode<Key,Value>* g);
    virtual void removeFix(AVLNode<Key, Value>* n, int8_t ndiff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // Empty tree
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    Node<Key, Value>* c = this->root_;
    AVLNode<Key, Value>* currNode = static_cast<AVLNode<Key,Value>*>(c);
    // Traverse tree
    while (currNode != nullptr) {
        // Must go left
        if (new_item.first < currNode->getKey()) {
            if (currNode->getLeft() == nullptr) { // Insert here if no more left
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, static_cast<AVLNode<Key,Value>*>(currNode));
                currNode->setLeft(newNode);
                newNode->setBalance(0);
                insertFix(currNode, newNode);
                break;
            }
            else {  // Else keep going
                currNode = currNode->getLeft();
            }
        }
        // Must go right
        else if (new_item.first > currNode->getKey()) {
            if (currNode->getRight() == nullptr) {// Insert here if no more right
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, static_cast<AVLNode<Key,Value>*>(currNode));
                currNode->setRight(newNode);
                newNode->setBalance(0);
                insertFix(currNode, newNode);
                break;
            }
            else {  // Else keep going
                currNode = currNode->getRight();
            }
        }
        // Equal, so overwrite
        else if (new_item.first == currNode->getKey()) {
            currNode->setValue(new_item.second);
            return;
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    AVLNode<Key, Value>* g = p->getParent();
    if (p == nullptr || g == nullptr) {
        return;
    }

    // p is left child of g
    if (g->getLeft() == p) {
        g->setBalance(g->getBalance() - 1);

        // Case 1
        if (g->getBalance() == 0) {
            return;
        }
        // Case 2
        else if (g->getBalance() == -1) {
            insertFix(g, p);
            return;
        }
        // Case 3
        else if (g->getBalance() == -2) {
            // Check for zig zig
            if (p->getLeft() == n) {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }

            // Must be zig zag
            else {
                rotateLeft(p);
                rotateRight(g);
                // Case 3a
                if (n->getBalance() == -1) {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                // Case 3b
                else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 1) {
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
            return;
        }
    }

    // p is right child of g
    else {
        g->setBalance(g->getBalance() + 1);

        // Case 1
        if (g->getBalance() == 0) {
            return;
        }
        // Case 2
        else if (g->getBalance() == 1) {
            insertFix(g, p);
            return;
        }
        // Case 3
        else if (g->getBalance() == 2) {
            // Check for zig zig
            if (p->getRight() == n) {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }

            // Must be zig zag
            else {
                rotateRight(p);
                rotateLeft(g);
                // Case 3a
                if (n->getBalance() == 1) {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                // Case 3b
                else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == -1) {
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
            return;
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* c)
{
    AVLNode<Key, Value>* parent = c->getParent();
    AVLNode<Key, Value>* child = c->getLeft();
    AVLNode<Key, Value>* b = child->getRight();

    if (parent == nullptr) {
        this->root_ = child;
    }
    else if (parent->getLeft() == c) {
        parent->setLeft(child);
    }
    else {
        parent->setRight(child);
    }

    child->setParent(parent);
    child->setRight(c);
    c->setParent(child);
    c->setLeft(b);
    if (b != nullptr) {
        b->setParent(c);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* c)
{
    AVLNode<Key, Value>* parent = c->getParent();
    AVLNode<Key, Value>* child = c->getRight();
    AVLNode<Key, Value>* b = child->getLeft();

    if (parent == nullptr) {
        this->root_ = child;
    }
    else if (parent->getLeft() == c) {
        parent->setLeft(child);
    }
    else {
        parent->setRight(child);
    }

    child->setParent(parent);
    child->setLeft(c);
    c->setParent(child);
    c->setRight(b);
    if (b != nullptr) {
        b->setParent(c);
    }
}




/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    int8_t diff = 0;
    AVLNode<Key, Value>* currNode = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::internalFind(key));

    if (currNode == nullptr) {
        return;
    }

    AVLNode<Key, Value>* p = currNode->getParent();

    // Keep track of how much to add
    if (p != nullptr) {
        if (p->getLeft() == currNode) {
            diff = 1;
        }
        else {
            diff = -1;
        }
    }

    // Remove node
    BinarySearchTree<Key, Value>::remove(key);

    removeFix(p, diff);
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int8_t diff)
{
    if (n == nullptr) {
        return;
    }

    int8_t ndiff = 0;
    AVLNode<Key, Value>* p = n->getParent();
    if (p != nullptr) {
        if (p->getLeft() == n) {
            ndiff = 1;
        }
        else {
            ndiff = -1;
        }
    }


    // Assume diff = -1
    
    n->setBalance(n->getBalance() + diff);
    if (diff == -1) {

        // Case 1
        if (n->getBalance() + diff == -2) {
            AVLNode<Key, Value>* c = n->getLeft();
            // Case 1a: zig zig
            if (c->getBalance() == -1) {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Case 1b: zig zig
            else if (c->getBalance() == 0) {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }
            // Case 1c: zig zag
            else {
                AVLNode<Key, Value>* g = c->getRight();
                int8_t balance_g = g->getBalance();
                rotateLeft(c);
                rotateRight(n);
                if (balance_g == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (balance_g == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }

        // Case 2
        else if (n->getBalance() + diff == -1) {
            n->setBalance(-1);
        }

        // Case 3
        else {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
    
    n->setBalance(n->getBalance() + diff);
    else if (diff == 1) {

        // Case 1
        if (n->getBalance() + diff == 2) {
            AVLNode<Key, Value>* c = n->getRight();
        
            // Case 1a: zig zig
            if (c->getBalance() == 1) {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Case 1b: zig zig
            else if (c->getBalance() == 0) {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            }
            // Case 1c: zig zag
            else {
                AVLNode<Key, Value>* g = c->getLeft();
                int8_t balance_g = g->getBalance();
                rotateRight(c);
                rotateLeft(n);
                if (balance_g == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (balance_g == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }

        // Case 2
        else if (n->getBalance() + diff == 1) {
            n->setBalance(1);
        }

        // Case 3
        else {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }

}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
