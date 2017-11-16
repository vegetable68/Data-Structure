/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"
#include <iostream>

/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T>
class LinkedList
{
    struct Node{
        T data;
        Node *nxt, *pre;
        Node(T _d = 0, Node *_n = NULL, Node *_p = NULL): data(_d), nxt(_n), pre(_p){}
    };
    Node *front, *rear;
    int sze;

public:
    //checked
    class Iterator
    {
    public:
        LinkedList *lnk;
        Node *cur, *last;
        void init(LinkedList *_l, Node *_c, Node *_last){
            lnk = _l; cur = _c; last = _last;
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {return (cur != NULL);}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if (!hasNext()) throw ElementNotExist("\nNo Such Element\n");
            last = cur; cur = cur->nxt;
            return last->data;
        }

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * @throw ElementNotExist
         */
        void remove() {
            if (last == NULL) throw ElementNotExist("\nNo Such Element\n");
            lnk->removeEntry(last);
            last = NULL;
        }
    };

    /**
     * TODO Constructs an empty linked list
     */
    LinkedList() {front = rear = NULL; sze = 0;}

    /**
     * TODO Copy constructor
     */
    //checked
    LinkedList(const LinkedList<T> &c) {
        Node *last = NULL, *tmp = front;
        tmp = NULL; front = NULL;
        for (Node *cur = c.front; cur != NULL; cur = cur->nxt){
            tmp = new Node;
            tmp->pre = last; tmp->data = cur->data;
            if (tmp->pre != NULL) tmp->pre->nxt = tmp; else front = tmp;
            last = tmp;
        } 
        rear = tmp;
        sze = c.sze;
    }

    /**
     * TODO Assignment operator
     */
    //checked
    LinkedList<T>& operator=(const LinkedList<T> &c) {
        if (c == *this) return *this;
        Node *last = NULL, *tmp = front;
        for (;tmp != NULL; ){
            Node *cur = tmp;
            tmp = tmp->nxt;
            delete cur;
        }
        tmp = NULL; front = NULL;
        for (Node *cur = c.front; cur != NULL; cur = cur->nxt){
            tmp = new Node;
            tmp->pre = last; tmp->data = cur->data;
            if (tmp->pre != NULL) tmp->pre->nxt = tmp; else front = tmp;
            last = tmp;
        } 
        rear = tmp;
        sze = c.sze;
        return *this;
    }

    /**
     * TODO Desturctor
     */
    ~LinkedList() {
        Node *tmp = front;
        for (;tmp != NULL;){
            Node *cur = tmp;
            tmp = tmp->nxt; delete cur;
        }
    }

    /**
     * TODO Appends the specified element to the end of this list.
     */
    bool add(const T& e) {//checked
        if (front == NULL) front = rear = new Node(e, NULL, NULL); 
            else {rear->nxt = new Node(e, NULL, rear); rear = rear->nxt;} 
        sze ++;
        return true;
    }

    /**
     * TODO Inserts the specified element to the beginning of this list.
     */
    void addFirst(const T& elem) {//checked
        front = new Node(elem, front, NULL);
        if (rear == NULL) rear = front;
        sze ++;
    }

    /**
     * TODO Insert the specified element to the end of this list.
     * Equivalent to add.
     */
    void addLast(const T &elem) {//checked
        if (rear == NULL) {
            sze ++;
            rear = new Node(elem, NULL, rear); front = rear; return;
        }
        rear->nxt = new Node(elem, NULL, rear); rear = rear->nxt;
        if (front == NULL) front = rear; 
        sze ++;
    }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {//checked
        int cur = 0; 
        for (Node *tmp = front; tmp != NULL; tmp = tmp->nxt, cur ++){
            if (cur == index){
                Node *New = new Node(element, tmp, tmp->pre);
                if (tmp->pre == NULL) front = New;
                    else tmp->pre->nxt = New;
                tmp->pre = New;
                sze ++;
                return ;
            }
        }
        if (cur == index){
            Node *New = new Node(element, rear, NULL);
            rear->nxt = New;
            sze ++;
            rear = New; return ;
        }
        throw IndexOutOfBound("\nIllegal Segment\n");
    }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {
        Node *tmp = front;
        for (;tmp != NULL;){
            Node *cur = tmp;
            tmp = tmp->nxt; delete cur;
        }
        front = rear = NULL;
        sze = 0;
    }

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {//checked
        Node *tmp = front;
        for (;tmp != NULL; tmp = tmp->nxt) if (tmp->data == e) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {//checked
        Node *tmp = front;
        int cur = 0;
        for (;tmp != NULL; cur ++, tmp = tmp->nxt){
            if (cur == index) return tmp->data;
        }
        throw IndexOutOfBound("\nIllegal Segment\n");
    }

    /**
     * TODO Returns a const reference to the first element.
     * @throw ElementNotExist
     */
    //checked
    const T& getFirst() const {if (front == NULL) throw ElementNotExist("\nNo Such Element\n"); else return front->data;}

    /**
     * TODO Returns a const reference to the last element.
     * @throw ElementNotExist
     */
    //checked
    const T& getLast() const {if (rear == NULL) throw ElementNotExist("\nNo Such Element\n"); else return rear->data;}

    /**
     * TODO Returns true if this list contains no elements.
     */
    //checked
    bool isEmpty() const {if (front == NULL) return true; else return false;}

    void removeEntry(Node *cur){
        Node *P = cur->pre, *N = cur->nxt;
        if (P != NULL) P->nxt = N; else front = N;
        if (N != NULL) N->pre = P; else rear = P;
        sze --;
        delete cur;
    }

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {//checked
        int cur = 0;
        Node *tmp = front;
        for (;tmp != NULL; cur ++, tmp = tmp ->nxt){
            if (cur == index){
                if (tmp->pre == NULL) front = tmp->nxt;
                    else tmp->pre->nxt = tmp->nxt;
                if (tmp->nxt == NULL) rear = tmp->pre;
                    else tmp->nxt->pre = tmp->pre;
                sze --;
                return ;
            }
        }
        throw IndexOutOfBound("\nIllegal Segment\n");
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it is present in the list, otherwise false.
     */
    //checked
    bool remove(const T &e) {
        Node *tmp = front; 
        for (;tmp != NULL; tmp = tmp->nxt){
            if (tmp->data == e){
                if (tmp->pre == NULL) front = tmp->nxt;
                    else tmp->pre->nxt = tmp->nxt;
                if (tmp->nxt == NULL) rear = tmp->pre;
                    else tmp->nxt->pre = tmp->pre;
                sze --;
                return true;
            }
        }
        return false;
    }

    /**
     * TODO Removes the first element from this list.
     * @throw ElementNotExist
     */
    //checked
    void removeFirst() {
        if (front == NULL) throw ElementNotExist("\nNo Such Element\n"); 
        Node *tmp = front;
        sze --;
        front = front->nxt;if (front != NULL) front->pre = NULL; else rear = front;
        delete tmp;
    }

    /**
     * TODO Removes the last element from this list.
     * @throw ElementNotExist
     */
    //checked
    void removeLast() {
        if (rear == NULL) throw ElementNotExist("\nNo Such Element\n");
        Node *tmp = rear;
        sze --;
        rear = rear->pre; if (rear != NULL) rear->nxt = NULL; else front = rear;
        delete tmp;
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    //checked
    void set(int index, const T &element) {
        Node *tmp = front; int i = 0;
        for (;i < index; i ++){
            if (tmp == NULL) throw IndexOutOfBound("\nIllegal Segment\n");
            tmp = tmp->nxt;
        }
        tmp->data = element;
    }

    /**
     * TODO Returns the number of elements in this list.
     */
    //checked
    int size() const {
        return sze;
    }

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator() {
        Iterator iter;
        iter.init(this, front, NULL);
        return iter;
    }
};

#endif
