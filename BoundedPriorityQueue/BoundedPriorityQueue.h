//
// Created by Vikrant More on 27/12/15.
//

#ifndef DS_BOUNDEDPRIORITYQUEUE_H
#define DS_BOUNDEDPRIORITYQUEUE_H

#include <map>
#include <algorithm>
#include <limits>

using namespace std;

template <typename T>
class BoundedPQueue{
public:
    explicit BoundedPQueue(size_t maxSize);
    void enqueue(const T& value, double priority);
    T dequeueMin();
    size_t size() const;
    bool empty() const;
    size_t maxSize() const;
    double best() const;
    double worst() const;

private:
    multimap<double, T> elems;
    size_t maximumSize;
};


template <typename T>
BoundedPQueue<T>::BoundedPQueue(size_t maxSize) {
    maximumSize = maxSize;
}

template <typename T>
void BoundedPQueue<T>::enqueue(const T &value, double priority) {
    elems.insert(make_pair(priority,value));

    if(size() >  maxSize()){
        typename multimap<double, T>::iterator last = elems.end();
        --last;
        elems.erase(last);
    }
}

template <typename T>
T BoundedPQueue<T>::dequeueMin() {
    T result = elems.begin()->second;
    elems.erase(elems.begin());
    return result;
}

template <typename T>
size_t BoundedPQueue<T>::size() const {
    return elems.size();
}

template <typename T>
bool BoundedPQueue<T>::empty() const {
    return elems.empty();
}

template <typename T>
size_t BoundedPQueue<T>::maxSize() const {
    return maximumSize;
}

template <typename T>
double BoundedPQueue<T>::best() const {
    return empty() ? numeric_limits<double>::infinity() : elems.begin()->first;
}

template <typename T>
double BoundedPQueue<T>::worst() const {
    return empty() ? numeric_limits<double>::infinity() : elems.rbegin()->first;
}

#endif //DS_BOUNDEDQUEUE_H
