#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <iostream>
#include <iomanip>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

    /// print elements of array for debugging
    void printHeap();

private:
    /// Add whatever helper functions you need below

    // Helper function for push
    void pushHeap(size_t i);

    /// heapify's the top element of a heap
    void heapify(unsigned int i);

    /// returns reference to element at index
    T & operator[](unsigned int i);

    /// returns size of arr
    int size();

    /// returns index of element, returns -1 if doesn't exist
    int findLoc(const T& item);

    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    // make sure item doesn't already exist in heap
    if(findLoc(item) != -1){
        return;
    }
    store_.push_back(std::make_pair(priority,item));
    // add arbitrary location into keyToLocation_. pushHeap() will take care of it
    keyToLocation_.insert(std::make_pair(item,0));
    pushHeap(size()-1);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void 
Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double newpri, const T& item)
{
    // You complete
    typename std::unordered_map<T, size_t>::iterator it
         = keyToLocation_.find(item);
    if(it==keyToLocation_.end()) return;
    size_t loc = it->second;
    store_.at(loc).first = newpri;
    //use pushHeap to move item up the heap if needed
    pushHeap(loc);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // You complete
    return store_[0].second;
}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    // You complete
    // If we get here we know the heap has at least 1 item
    // Replace top element with last element, and pop last element
    typename std::vector<std::pair<double, T> >::iterator it = store_.begin();
    keyToLocation_.erase(it->second);
    *it = *(store_.begin()+(size()-1));
    store_.pop_back();
    heapify(0);
}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

// helper function for push
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pushHeap(size_t i){
    //if store_[i] at top of heap, return
    if(i<=0){
        return;
    }
    size_t p = (i-1)/m_;
    //if store_[i] is smaller than parent, swap
    //make sure to adjust locations stores in keyToLocation_ when swapping
    std::pair<double,T> curr(store_[i]);
    std::pair<double,T> par(store_[p]);
    if(c_(curr.first,par.first)){
        std::pair<double, T> temp = store_[i];
        store_[i] = store_[p];
        store_[p] = temp;
        keyToLocation_.at(curr.second) = p;
        keyToLocation_.at(par.second) = i;
        pushHeap(p);
    }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::heapify(unsigned int i){
    //if i is out of bounds, return
    if(i >= store_.size()){return;}
    //start with left child
    unsigned int left_c = m_*i + 1;
    //if left child is out of bounds, store_[i] is a leaf node
    if((int)left_c >= size()){
        return;
    }
    //find smallest child
    unsigned int c = left_c;
    //compare to any existing right side children
    for(int j=1; j<m_; j++){
        //check that it exists
        if(left_c+j >= store_.size()){break;}
        if(c_(store_[left_c+j].first,store_[c].first)){
        c = left_c+j;
        }
    }
    //if store_[i] is larger than child, swap
    //make sure to adjust locations stores in keyToLocation_ when swapping
    std::pair<double,T> curr(store_[i]);
    std::pair<double,T> child(store_[c]);
    if(c_(child.first,curr.first)){
        std::pair<double, T> temp = store_[i];
        store_[i] = store_[c];
        store_[c] = temp;
        keyToLocation_.at(curr.second) = c;
        keyToLocation_.at(child.second) = i;
        heapify(c);
    }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T & Heap<T,TComparator,PComparator,Hasher>::operator[](unsigned int i){
    if(i < 0 || i >= store_.size()){
        T garbage;
        return garbage;
    }
    return store_[i];
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
int Heap<T,TComparator,PComparator,Hasher>::size(){
    return store_.size();
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
int Heap<T,TComparator,PComparator,Hasher>::findLoc(const T& item){
    for(int i=0; i<size();i++){
        if((T)store_[i].second == (T)item){
            return i;
        }
    }
    return -1;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::printHeap(){
    int i = 1;
    for(typename std::vector<std::pair<double, T> >::iterator it = store_.begin(); 
        it != store_.end(); it++){
        std::cout << "(" << std::fixed << std::setprecision(1) << it->first;
        std::cout << "," << it->second << ")";
        if(it+1 != store_.end() && i%5 != 0){
        std::cout << " - ";
        }
        if(it+1 != store_.end() && i%5 == 0){
            std::cout << std::endl;
        }
        i++;
    }
    std::cout << std::endl << std::endl;
}

#endif

