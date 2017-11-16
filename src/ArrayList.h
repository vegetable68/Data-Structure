/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, where it means
 * the length of the array of your internal implemention
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class T>
class ArrayList
{
    T *elem;
    int rear, Size;

    void doubleSpace() {
        T *tmp = elem;
        elem = new T[2 * Size];
        for (int i = 0; i < Size; i ++) elem[i] = tmp[i];
        Size *= 2; delete [] tmp;
    }
public:
    class Iterator
    {
        int pos, rear, Size, last;
        ArrayList *arr;
    public:
        void init(int _p, ArrayList<T> *_a, int _r, int _S, int L){
            pos = _p; rear = _r; Size = _S; last = L; arr = _a;
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {if (pos >= rear) return false; else return true;}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if (!hasNext()) throw ElementNotExist("\nNo Such Element\n");
            last = pos; pos ++; return arr->get(last);
        }

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * @throw ElementNotExist
         */
        void remove() {
            if (last < 0 || last >= rear) throw ElementNotExist("\nNo Such Element\n");
            arr->removeIndex(last);
            pos --; rear --; last = -1;
        }
    };

    /**
     * TODO Constructs an empty array list.
     */
    ArrayList() {Size = 1; elem = new T[Size]; rear = 0;}

    /**
     * TODO Destructor
     */
    ~ArrayList() {delete [] elem;}

    /**
     * TODO Assignment operator
     */
    ArrayList& operator=(const ArrayList& x) { //checked
        if (x == *this) return *this;
        delete [] elem;
        Size = x.Size; rear = x.rear;
        elem = new T[Size];
        for (int i = 0; i < rear; i ++) elem[i] = x.elem[i];
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    ArrayList(const ArrayList& x) {//checked
        rear = x.rear; Size = x.Size;
        elem = new T[Size];
        for (int i = 0; i < rear; i ++) elem[i] = x.elem[i];
    }

    /**
     * TODO Appends the specified element to the end of this list.
     */
    bool add(const T& e) { //checked
        if (rear == Size) doubleSpace();
        elem[rear++] = e;
        return true;
    }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) { //checked
        if (index >= rear || rear < 0) {throw IndexOutOfBound("\nIllegal Segment\n"); return ;}
        if (rear == Size) doubleSpace();
        for (int i = rear; i > index; i --) elem[i] = elem[i - 1]; 
        elem[index] = element; rear ++;
    }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {//checked
        delete [] elem; Size = 1; rear = 0;
        elem = new T[Size];
    }

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {//checked
        for (int i = 0; i < rear; i ++) if (elem[i] == e) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {//checked
        if (index >= rear || rear < 0) throw IndexOutOfBound("\nIllegal Segment\n");
        return elem[index];
    }

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const {return (rear == 0);}//checked

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {//checked
        if (index >= rear || index < 0) throw IndexOutOfBound("\nIllegal Segment\n");
        for (int i = index; i < rear; i ++){
            elem[i] = elem[i + 1]; 
        } 
        rear --;
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it is present in the list, otherwise false.
     */
    bool remove(const T &e) {//checked
        for (int i = 0; i < rear; i ++)
            if (elem[i] == e) {removeIndex(i); return true;}
        return false;
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {//checked
        if (index < 0 || index >= rear) throw IndexOutOfBound("\nIllegal Segment\n");
        elem[index] = element;
    }

    /**
     * TODO Returns the number of elements in this list.
     */
    int size() const {return rear;}//checked

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator() {
        Iterator itr;
        itr.init(0, this, rear, Size, -1);
        return itr; 
    }
};

#endif
