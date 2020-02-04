#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a 
* data member, plus other additional helper functions. You do NOT need 
* to implement any functionality or add additional data members or 
* helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the 
* base class constructor and setting the color to red since every new 
* node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, 
    AVLNode<Key, Value> *parent) :
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
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast 
* is necessary to make sure that our node is a AVLNode.
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
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void insertFix( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    virtual void fixRotate(AVLNode<Key,Value>* g, AVLNode<Key,Value>* p,
                            AVLNode<Key,Value>* n);
    virtual void rotateLeft(AVLNode<Key,Value>* n);
    virtual void rotateRight(AVLNode<Key,Value>* n);
    virtual AVLNode<Key,Value>* predecessor(AVLNode<Key,Value>* curr);
    virtual void removeFix(AVLNode<Key,Value>* n, int diff);
    virtual void promoteChild(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* child);
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    Key k = new_item.first;
    Value v = new_item.second;
    //if tree is empty just insert it
    if(this->empty()){
        this->root_ = new AVLNode<Key, Value>(k,v,NULL);
        return;
    }
    //find where key fits into the tree
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* in;
    while(curr != NULL){
        if(k < curr->getKey()){
            //if no left child, insert new node there and update balance
            if(curr->getLeft() == NULL){
                in = new AVLNode<Key, Value>(k,v,curr);
                curr->setLeft(in);
                curr->updateBalance(-1);
                break;
            }
            curr = curr->getLeft();
        }
        else if(k > curr->getKey()){
            //if no right child, insert new node there and update balance
            if(curr->getRight() == NULL){
                in = new AVLNode<Key, Value>(k,v,curr);
                curr->setRight(in);
                curr->updateBalance(1);
                break;
            }
            curr = curr->getRight();
        }
        //else key already exists, overwrite it
        else{
            curr->setValue(v);
            //return;
            break;
        }
    }
    //fix any unbalance
    if(curr->getBalance() != 0){
        insertFix(curr, in);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
    //if parent is NULL, return
    if(p==NULL){return;}
    if(p->getParent()==NULL){return;}
    AVLNode<Key,Value>* g = static_cast<AVLNode<Key, Value>*>(p->getParent());
    //update g's balance
    if(p == g->getLeft()){
        g->updateBalance(-1);
    }
    else{
        g->updateBalance(1);
    }
    //check if need to fix
    if(g->getBalance()==0){return;}
    else if(g->getBalance()==1 || g->getBalance()==-1){
        insertFix(g,p);
    }
    else{
        fixRotate(g,p,n);
    }
}

/**
* Takes care of balance changes and what rotation sequence to use
* for leftleft,rightright,leftright,and rightleft cases
*/
template<class Key, class Value>
void AVLTree<Key, Value>::fixRotate(AVLNode<Key,Value>* g, AVLNode<Key,Value>* p,
    AVLNode<Key,Value>* n){
    //if left-right zig-zag
    if(p == g->getLeft() && n == p->getRight()){
        rotateLeft(p);
        rotateRight(g);
        if(n->getBalance()==-1){
            n->setBalance(0);
            p->setBalance(0);
            g->setBalance(1);
        }
        else if(n->getBalance()==0){
            n->setBalance(0);
            p->setBalance(0);
            g->setBalance(0);
        }
        else{
            n->setBalance(0);
            p->setBalance(-1);
            g->setBalance(0);
        }
    }
    //if right-left zig-zag
    else if(p == g->getRight() && n == p->getLeft()){
        rotateRight(p);
        rotateLeft(g);
        if(n->getBalance()==1){
            n->setBalance(0);
            p->setBalance(0);
            g->setBalance(-1);
        }
        else if(n->getBalance()==0){
            n->setBalance(0);
            p->setBalance(0);
            g->setBalance(0);
        }
        else{
            n->setBalance(0);
            p->setBalance(1);
            g->setBalance(0);
        }
    }
    //if left-left zig-zig
    else if(p == g->getLeft() && n == p->getLeft()){
        rotateRight(g);
        p->setBalance(0);
        g->setBalance(0);
    }
    //if right-right zig-zig
    else{
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key,Value>* n){
    //right subtree promoted to n's position
    AVLNode<Key,Value>* r = n->getRight();
    //update root if needed
    if(n == this->root_){
        this->root_= r;
    }
    //
    //keep track of everything that will be changed
    AVLNode<Key,Value>* n_p = n->getParent();
    AVLNode<Key,Value>* r_lt = r->getLeft();
    //change n's pointers
    n->setRight(r_lt);
    n->setParent(r);
    //change r's pointers
    r->setParent(n_p);
    r->setLeft(n);
    //change pointers of other nodes affected
    if(n_p == NULL){;}
    else if(n == n_p->getLeft()){
        n_p->setLeft(r);
    }
    else{
        n_p->setRight(r);
    }
    if(r_lt!=NULL){
        r_lt->setParent(n);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* n){
    //left subtree promoted to n's position
    AVLNode<Key,Value>* lt = n->getLeft();
    //update root if needed
    if(n == this->root_){
        this->root_= lt;
    }
    //keep track of everything that will be changed
    AVLNode<Key,Value>* n_p = n->getParent();
    AVLNode<Key,Value>* lt_r = lt->getRight();
    //change n's pointers
    n->setLeft(lt_r);
    n->setParent(lt);
    //change lt's pointers
    lt->setParent(n_p);
    lt->setRight(n);
    //change pointers of other nodes affected
    if(n_p == NULL){;}
    else if(n == n_p->getLeft()){
        n_p->setLeft(lt);
    }
    else{
        n_p->setRight(lt);
    }
    if(lt_r!=NULL){
        lt_r->setParent(n);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    //copy remove from binarysearchtree 
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>\
        (BinarySearchTree<Key,Value>::internalFind(key));
    //if key doesn't exist, do nothing
    if(curr==NULL){
        return;
    }
    //if tree only has root node, delete and set root to NULL
    if(curr == this->root_ && curr->getLeft()==NULL && curr->getRight()==NULL){
        delete curr;
        this->root_=NULL;
        return;
    }
    //while curr is not a leafnode, swap it with something
    while(curr->getLeft()!=NULL || curr->getRight()!=NULL){
        bool currisroot(curr==this->root_);
        AVLNode<Key,Value>* next;
        //if there are two children, swap with predecessor
        if(curr->getLeft()!=NULL && curr->getRight()!=NULL){
            next = predecessor(curr);
            nodeSwap(curr,next);
        }
        //if one child, swap with child
        else if(curr->getLeft()!=NULL && curr->getRight()==NULL){
            next = curr->getLeft();
            promoteChild(curr,next);
        }
        else{
            next = curr->getRight();
            promoteChild(curr,next);
        }
        if(currisroot){
            this->root_ = next;
        }
    }
    //update parent pointers and balance
    AVLNode<Key,Value>* g = curr->getParent();
    int diff = 0;
    if(g==NULL){;}
    else if(curr == g->getLeft()){
        g->setLeft(NULL);
        diff = 1;
    }
    else if(curr == g->getRight()){
        g->setRight(NULL);
        diff = -1;
    }
    delete curr;
    removeFix(g,diff);
}

template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* n, int diff){
    if(n==NULL){return;}
    AVLNode<Key,Value>* p = n->getParent();
    int ndiff = 0;
    if(p==NULL){;}
    else if(n == p->getLeft()){
        ndiff = 1;
    }
    else{
        ndiff = -1;
    }
    //go through cases
    //n removed left-child, heavier on right side
    if(n->getBalance()+diff == 2){
        AVLNode<Key,Value>* c = n->getRight();
        if(c==NULL){;}
        else{
            int c_bal = c->getBalance();
            //rightright case
            if(c_bal==1 || c_bal==0){
                rotateLeft(n);
                if(c_bal==1){
                    c->setBalance(0);
                    n->setBalance(0);
                    removeFix(p,ndiff);
                }
                else if(c_bal==0){
                    c->setBalance(-1);
                    n->setBalance(1);
                    return;
                }
            }
            //rightleft case
            else if(c_bal==-1){
                fixRotate(n,c,c->getLeft());
                removeFix(p,ndiff);
            }
        }
    }
    //n removed right-child, heavier on left side
    else if(n->getBalance()+diff == -2){
        AVLNode<Key, Value>* c = n->getLeft();
        if(c==NULL){;}
        else{
            int c_bal = c->getBalance();
            //leftleft case
            if(c_bal==-1 || c_bal==0){
                rotateRight(n);
                if(c_bal==-1){
                    c->setBalance(0);
                    n->setBalance(0);
                    removeFix(p,ndiff);
                }
                else if(c_bal==0){
                    c->setBalance(1);
                    n->setBalance(-1);
                    return;
                }
            }
            //leftright case
            else if(c_bal==1){
                fixRotate(n,c,c->getRight());
                removeFix(p,ndiff);
            }
        }
    }
    else if(n->getBalance()+diff == 1){
        n->setBalance(1);
        return;
    }
    else if(n->getBalance()+diff == -1){
        n->setBalance(-1);
        return;
    }
    else if(n->getBalance()+diff==0){
        removeFix(p,ndiff);
    }
}

template<class Key, class Value>
AVLNode<Key,Value>* 
AVLTree<Key,Value>::predecessor(AVLNode<Key,Value>* curr)
{
    return static_cast<AVLNode<Key,Value>*>\
    (BinarySearchTree<Key,Value>::predecessor(curr));
}

template<class Key, class Value>
void AVLTree<Key, Value>::promoteChild(AVLNode<Key,Value>* curr, 
    AVLNode<Key,Value>* child)
{
    BinarySearchTree<Key,Value>::promoteChild(curr,child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, 
    AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
