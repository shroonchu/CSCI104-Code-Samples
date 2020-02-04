#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, 
    Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers 
* inside of a node are only used as references to existing nodes. 
* The nodes pointed to by parent/left/right are freed within the 
* deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    static Node<Key, Value>* successor(Node<Key, Value>* current);
    static bool checkBalanced(Node<Key, Value>* n);
    static int getHeight(Node<Key, Value>* n, int h);
    static void postOrder(Node<Key, Value>* curr);
    static void promoteChild(Node<Key, Value>* curr, Node<Key, Value>* child);

protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    // TODO
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() : current_(NULL)
{
    // TODO
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    //check for null values
    if(current_==NULL && rhs.current_==NULL){return true;}
    else if(current_!=NULL && rhs.current_==NULL){return false;}
    else if(current_==NULL && rhs.current_!=NULL){return false;}
    //both are not null, then compare actual contents
    else if(current_ == rhs.current_){
        return true;
    }
    else{return false;}
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    //check for null values
    if(current_==NULL && rhs.current_==NULL){return false;}
    else if(current_!=NULL && rhs.current_==NULL){return true;}
    else if(current_==NULL && rhs.current_!=NULL){return true;}
    //both are not null, then compare actual contents
    else if(current_ != rhs.current_){
        return true;
    }
    else{return false;}
}

/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    // TODO
    current_ = successor(current_);
    return *this;
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree():root_(NULL)
{
    // TODO
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    // TODO
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair\
    <const Key, Value> &keyValuePair)
{
    // TODO
    Key k = keyValuePair.first;
    Value v = keyValuePair.second;
    //if tree is empty just insert it
    if(empty()){
        root_ = new Node<Key, Value>(k,v,NULL);
        return;
    }
    //find where key fits into the tree
    Node<Key, Value>* curr = root_;
    while(curr != NULL){
        if(k < curr->getKey()){
            //if no left child, insert new node there
            if(curr->getLeft() == NULL){
                Node<Key, Value>* in = new Node<Key, Value>(k,v,curr);
                curr->setLeft(in);
                return;
            }
            curr = curr->getLeft();
        }
        else if(k > curr->getKey()){
            //if no right child, insert new node there
            if(curr->getRight() == NULL){
                Node<Key, Value>* in = new Node<Key, Value>(k,v,curr);
                curr->setRight(in);
                return;
            }
            curr = curr->getRight();
        }
        //else key already exists, overwrite it
        else{
            curr->setValue(v);
            return;
        }
    }
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    // TODO
    Node<Key, Value>* curr = internalFind(key);
    //if key doesn't exist, do nothing
    if(curr==NULL){
        return;
    }
    //if tree only has root node, delete and set root to NULL
    if(curr == root_ && curr->getLeft()==NULL && curr->getRight()==NULL){
        delete curr;
        root_=NULL;
        return;
    }
    //while curr is not a leafnode, swap it with something
    while(curr->getLeft()!=NULL || curr->getRight()!=NULL){
        bool currisroot(curr==root_);
        Node<Key,Value>* next;
        //if there are two children, swap with predecessor
        if(curr->getLeft()!=NULL && curr->getRight()!=NULL){
            next = predecessor(curr);
            nodeSwap(curr,next);
        }
        //if one child, promote child
        else if(curr->getLeft()!=NULL && curr->getRight()==NULL){
            next = curr->getLeft();
            promoteChild(curr,next);
        }
        else{
            next = curr->getRight();
            promoteChild(curr,next);
        }
        if(currisroot){
            root_ = next;
        }
    }
    //update parents
    if(curr->getParent()==NULL){;}
    else if(curr == curr->getParent()->getLeft()){
        curr->getParent()->setLeft(NULL);
    }
    else if(curr == curr->getParent()->getRight()){
        curr->getParent()->setRight(NULL);
    }
    delete curr;
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::promoteChild(Node<Key, Value>* curr, 
    Node<Key, Value>* child){
    if(curr == NULL || child == NULL){return;}
    Node<Key, Value>* par = curr->getParent();
    //update child's parent to curr's parent
    child->setParent(par);
    //udpate par's child to curr's child
    if(par==NULL){;}
    else if(curr == par->getLeft()){
        par->setLeft(child);
    }
    else{
        par->setRight(child);
    }
    curr->setLeft(NULL);
    curr->setRight(NULL);
    curr->setParent(child);
}

template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    // TODO
    //if current has a left child, find rightmost
    //node in left subtree
    Node<Key, Value>* pred = current;
    //if pred is NULL, just return it
    if(pred == NULL){return NULL;}
    else if(pred->getLeft() != NULL){
        pred = pred->getLeft();
        while(pred->getRight() != NULL){
            pred = pred->getRight();
        }
    }
    //else go up ancestor chain until traverse
    //left child pointer
    else{
        while(pred->getParent()!=NULL){
            if(pred == pred->getParent()->getRight()){
                break;
            }
            pred = pred->getParent();
        }
        pred = pred->getParent();
    }
    return pred;
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    // TODO
    postOrder(root_);
    root_ = NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key,Value>::postOrder(Node<Key, Value>* curr){
    if(curr==NULL){
        return;
    }
    if(curr->getLeft() != NULL){
        postOrder(curr->getLeft());
    }
    if(curr->getRight()!= NULL){
        postOrder(curr->getRight());
    }
    delete curr;
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    // TODO
    //if empty, return nothing
    if(empty()){
        return NULL;
    }
    Node<Key, Value>* curr = root_;
    while(curr->getLeft() != NULL){
        curr = curr->getLeft();
    }
    //curr is now leftmost node in the whole tree
    return curr;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* 
BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    // TODO
    Node<Key, Value>* curr = root_;
    //while curr is not a leafnode, keep searching
    while(curr!=NULL){
        //if curr has same key, return it
        if(key == curr->getKey()){
            break;
        }
        //if key is less than curr's key
        else if(key < curr->getKey()){
            curr = curr->getLeft();
        }
        //otherwise, key is greater than curr's key
        else{
            curr = curr->getRight();
        }
    }
    //cur is either found or it is null
    return curr;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    // TODO
    //check each node to see if it is balanced
    for(typename BinarySearchTree<Key, Value>::iterator it = this->begin(); 
        it != this->end(); ++it){
        if(!checkBalanced(it.current_)){
            return false;
        }
    }
    return true;
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key,Value>* n1, 
    Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

template<typename Key, typename Value>
Node<Key, Value>* 
BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current){
    //if current has a right child, find leftmost
    //node in right subtree
    Node<Key, Value>* succ = current;
    //if succ is NULL, just return it
    if(succ == NULL){return NULL;}
    else if(succ->getRight() != NULL){
        succ = succ->getRight();
        while(succ->getLeft() != NULL){
            succ = succ->getLeft();
        }
    }
    //else go up ancestor chain until traverse
    //left child pointer
    else{
        while(succ->getParent()!=NULL){
            if(succ == succ->getParent()->getLeft()){
                break;
            }
            succ = succ->getParent();
        }
        succ = succ->getParent();
    }
    return succ;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::checkBalanced(Node<Key, Value>* n){
    //get heights of right and left subtree
    int r_height = getHeight(n->getRight(),0);
    int l_height = getHeight(n->getLeft(),0);
    int balance = r_height - l_height;
    return (balance<=1 && balance>=-1);
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key, Value>* n, int h){
    //if n is NULL, return what h already is
    if(n==NULL){return h;}
    //try traversing down both subtrees
    else{
        int left = getHeight(n->getLeft(), h+1);
        int right = getHeight(n->getRight(), h+1);
        if(left > right){return left;}
        else{return right;}
    }
}


/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
