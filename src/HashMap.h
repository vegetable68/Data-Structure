/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"


/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap
{
public:
    class Entry
    {
        K key;
        V value;
    public:
        Entry(){}
        Entry(K k, V v)
        {
            key = k;
            value = v;
        }
        Entry(const Entry &x){
            key = x.key; value = x.value; 
        }
        
        Entry &operator =(const Entry &x){
            key = x.key; value = x.value;
            return *this;
        }

        K getKey() const
        {
            return key;
        }

        V getValue() const
        {
            return value;
        }
        
        V &getValue2() 
        {
            return value;
        }

    };
    class Node{
    public:
        Entry data;
        Node *nxt;
        Node(){}
        Node(Entry _d, Node *_n = NULL):data(_d), nxt(_n){}
    };
    const static int D_cap = 11;
    const static double Factor = 0.75;
    int cap, thereshold;
    Node** buckets;
    int sz;

    int hash(const K &key) const{
        int res = (H::hashCode(key) % cap + cap) % cap;
       // std::cout<<H::hashCode(key)<std::endl;
        return res;
    }

    void rehash(){
        Node** tmp = buckets;
        int l_cap = cap;
        cap *= 2;
        buckets = new Node* [cap];
        thereshold = cap * Factor;
        for (int i = 0; i < cap; i ++) buckets[i] = NULL; 
        for (int i = 0; i < l_cap; i ++){
            Node *e = tmp[i];
            for (;e != NULL; e = e->nxt){
                int idx = hash(e->data.getKey());
                Node *cur = new Node(e->data, buckets[idx]);
                buckets[idx] = cur;
            }
        }
        for (int i = 0; i < l_cap; i ++) delete tmp[i];
        delete [] tmp;
    }

    class Iterator
    {
        Node** buckets;
        int idx, cap;
        Node* cur; 
        Entry res;
    public:
        void init(Node** _B, int _C){buckets = _B; cap = _C; idx = -1; cur = NULL;}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            while (cur == NULL){
                idx ++; if (idx >= cap) return 0;
                cur = buckets[idx];
            }
            return 1;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if (!hasNext()) throw ElementNotExist("\nNo Such Element.\n");
            while (cur == NULL){
                idx ++; cur = buckets[idx];
            }
            res = cur->data;
            cur = cur->nxt;
            return res; 
        }
    };

    /**
     * TODO Constructs an empty hash map.
     */
    HashMap() {
        cap = D_cap;
        thereshold = cap * Factor;
        sz = 0;
        buckets = new Node* [cap];
        for (int i = 0; i < cap; i ++) buckets[i] = NULL;
    }

    /**
     * TODO Destructor
     */
    ~HashMap() { 
        for (int i = 0; i < cap; i ++){
            Node *e = buckets[i];
            while (e != NULL){
                Node *tmp = e->nxt;
                delete e; e = tmp;
            }
        } 
        delete [] buckets;
    }

    /**
     * TODO Assignment operator
     */
    HashMap &operator=(const HashMap &x) {
        if (x == *this) return *this;
        for (int i = 0; i < cap; i ++) delete buckets[i];
        delete [] buckets;
        cap = x.cap;
        thereshold = x.thereshold;
        sz = x.sz;
        buckets = new Node* [cap];
        for (int i = 0; i < cap; i ++){
            buckets[i] = NULL;
            Node *cur = buckets[i], *e = x.buckets[i];
            while (e != NULL){
                cur = new Node(e->data, buckets[i]);
                buckets[i] = cur; e = e->nxt;            
            } 
        }
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    HashMap(const HashMap &x) { 
        cap = x.cap;
        thereshold = x.thereshold;
        sz = x.sz;
        buckets = new Node* [cap];
        for (int i = 0; i < cap; i ++){
            buckets[i] = NULL;
            Node *cur = NULL, *e = x.buckets[i];
            while (e != NULL){
                cur = new Node(e->data, buckets[i]);
                buckets[i] = cur; e = e->nxt;            
            } 
        }
     }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const {
        Iterator iter;
        iter.init(buckets, cap);
        return iter;
    }

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {
        for (int i = 0; i < cap; i ++) delete buckets[i];
        delete [] buckets;
        cap = D_cap;
        thereshold = cap * Factor;
        buckets = new Node* [cap];
        for (int i = 0; i < cap; i ++) buckets[i] = NULL;
    }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
        int idx = hash(key);
        Node *e = buckets[idx];
        for (;e != NULL; e = e->nxt){
            if (e->data.getKey() == key) return true; 
        }
        return false;
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
        for (int i = 0; i < cap; i ++){
            Node *e = buckets[i];
            for (;e != NULL; e = e->nxt){
                if (e->data.getValue() == value) return true; 
            }
        }
        return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
        if (!containsKey(key)) throw ElementNotExist("\nNo Such Element.\n");
        int idx = hash(key);
        Node *e = buckets[idx];
        for (;e != NULL; e = e->nxt){
            if (e->data.getKey() == key) return e->data.getValue2(); 
        }
        return e->data.getValue2();
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {
        for (int i = 0; i < cap; i ++) if (buckets[i] != NULL) return 0;
        return 1;
    }

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
        int idx = hash(key);
        Node *e = buckets[idx];
       // std::cout<<"tt"<<" "<<idx<<" "<<cap<<std::endl;
        
        for (;e != NULL;e = e->nxt){
            if (e->data.getKey() == key) {e->data = Entry(key, value); return ;} 
        }
        //std::cout<<"tt"<<std::endl;
        e = new Node(Entry(key, value), buckets[idx]);
        buckets[idx] = e;
        if (++sz > thereshold) rehash();
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        int idx = hash(key);
        if (!containsKey(key)) throw ElementNotExist("\nNo Such Element.\n");
        Node *e = buckets[idx], *last = NULL;
        for (;e != NULL;last = e, e = e->nxt){
            if (e->data.getKey() == key) {
                if (last != NULL) last->nxt = e->nxt;
                    else buckets[idx] = e->nxt;
                sz --;
                delete e;
                return ;
            }
        }
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const { return sz;}
};

#endif
