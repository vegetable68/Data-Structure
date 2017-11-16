/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H

#include "ElementNotExist.h"

/**
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V>
class TreeMap
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
        Entry(const Entry& x){
            key = x.key; value = x.value; 
        }
        Entry& operator=(const Entry& x){
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
    class treap
    {
        public:
        struct Node{
            int l, r, fix, sum;
            bool tag;
            Node(){tag = 1;}
            Entry key;
        };
        int Size, root, cur_size, tot;
        const static int cap = 1000000; 
        Node *p;
        void doubleSpace(){
            Node *tmp = p; 
            p = new Node[2 * Size];
            for (int i = 0; i < Size; i ++) p[i] = tmp[i];
            Size *= 2;delete [] tmp;
        }
        void update(int x){
            p[x].sum = 1;
            p[x].sum += (p[x].l != -1)? p[p[x].l].sum:0;
            p[x].sum += (p[x].r != -1)? p[p[x].r].sum:0;
        }
        void rot_l(int &x){
            int y = p[x].r;
            p[x].r = p[y].l;
            p[y].l = x;
            update(x); update(y); 
            x = y;
        }
        void rot_r(int &x){
            int y = p[x].l;
            p[x].l = p[y].r;
            p[y].r = x;
            update(x); update(y);
            x = y;
        }
        void ins(int &k, Entry cur){
            if (k == -1){
                k = ++cur_size; 
                p[k].l = p[k].r = -1;
                update(k);
                p[k].key = cur;
                p[k].fix = rand();
            }else if (cur.getKey() < p[k].key.getKey()){
                        ins(p[k].l, cur); update(k);
                        if (p[ p[k].l ].fix > p[k].fix) rot_r(k);
                    }else{
                        if (cur.getKey() == p[k].key.getKey()){p[k].key = cur; return;}
                        ins(p[k].r, cur); update(k);
                        if (p[ p[k].r ].fix > p[k].fix) rot_l(k);
                    }
        }
        void del(int &k, Entry cur){
            if (k == -1) return;
            if (cur.getKey() < p[k].key.getKey()) {del(p[k].l, cur); update(k); return; }
            if (cur.getKey() > p[k].key.getKey()) {del(p[k].r, cur); update(k); return; }
                else {
                            if (p[k].l == -1 && p[k].r == -1) {tot ++; p[k].tag = 0; k = -1;}
                                else if (p[k].l == -1) {tot ++; p[k].tag = 0; k = p[k].r;}
                                        else if (p[k].r == -1) k = p[k].l;
                                                else if (p[ p[k].l ].fix < p[ p[k].r ].fix){
                                                        rot_l(k); del(p[k].l, cur); update(k);
                                                     }else{rot_r(k); del(p[k].r, cur); update(k);}
                     }
        }
        bool K_existed(int k, K key)const{
            if (k == -1) return 0;
            if (p[k].key.getKey() == key) return 1;
            if (key < p[k].key.getKey()) return K_existed(p[k].l, key); else return K_existed(p[k].r, key);
        }
        bool V_existed(int k, V val)const{
            if (k == -1) return 0;
            if (p[k].key.getValue() == val) return 1;
            return V_existed(p[k].l, val) | V_existed(p[k].r, val);
        }
        V &K_Val(int k, K key) const{
            if (p[k].key.getKey() == key) return p[k].key.getValue2();
            if (key < p[k].key.getKey()) return K_Val(p[k].l, key); else return K_Val(p[k].r, key);
        }
        Entry &Get(int k, int x){
            int tmpx = x;
            if (p[k].l != -1) x -= p[ p[k].l ].sum;
            if (x == 1) return p[k].key;
            if (x <= 0) return Get(p[k].l, tmpx); else return Get(p[k].r, x - 1);
        }
        treap(){
            srand(time(0));
            cur_size = -1;
            root = -1;
            tot = 0;
            Size = 1; p = new Node[Size];
        }
        treap(const treap &x){
            Size = x.Size; root = x.root; cur_size = x.cur_size;
            p = new Node[Size];
            for (int i = 0; i < Size; i ++) p[i] = x.p[i];
        }

        void clear(){
            srand(time(0));
            cur_size = -1;
            root = -1;
            tot = 0;
            Size = 1; p = new Node[Size];
        }
        ~treap(){delete [] p;}
        treap &operator=(const treap &x){
            delete [] p;
            Size = x.Size; root = x.root; cur_size = x.cur_size;
            p = new Node[Size];
            for (int i = 0; i < Size; i ++) p[i] = x.p[i];
            return *this;
        }
        void insert(Entry cur){
            if (cur_size + 1 >= Size) doubleSpace();
            ins(root, cur);
        } 
        void refresh(){
            Node *tmp = p;
            int preS = cur_size;
            Size = 1; root = -1; cur_size = -1; tot = 0;
            p = new Node[Size]; 
            for (int i = 0; i <= preS; i ++) 
                if (tmp[i].tag) {
                   if (cur_size + 1 >= Size) doubleSpace();ins(root, tmp[i].key); 
                }
            delete [] tmp;
        }
        void remove(Entry cur){
            del(root, cur); 
            if (tot >= cap) {tot = 0; refresh();}
        }
        int size() const{if (root == -1) return 0; else return p[root].sum;}
        bool FindKey(K key) const{ return K_existed(root, key);}
        bool FindValue(V val) const{ return V_existed(root, val);}
        V &KeyValue(K key) const {return K_Val(root, key);} 
        Entry &pos(int x){return Get(root, x);}
        V TT(K key){}
    }T;
    V res;

    class Iterator
    {
        treap T;
        int cur;
        Entry res;
    public:
        Iterator(){}
        void init(treap _T){
            T = _T; cur = 0; 
        }
        /*T.*
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            if (cur >= T.size()) return 0; else return 1;
        }

        /*alue*
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if (!hasNext()) throw ElementNotExist("\nNo Such Element\n");
            cur ++;
            return T.pos(cur); 
        }
    };

    /**
     * TODO Constructs an empty tree map.
     */
    TreeMap() {
        T.clear();
    }

    /**
     * TODO Destructor
     */
    ~TreeMap() {} 

    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x) {
        if (x == *this) return *this;
        T = x.T;
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x) {
        T = x.T;
    }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const {
        Iterator itr;
        itr.init(T);
        return itr;
    }

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {T.clear();}

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {return T.FindKey(key);}

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
        return T.FindValue(value); 
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
       if (!containsKey(key)) throw ElementNotExist("\nNo Such Element\n");
       return T.KeyValue(key);
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {return !T.size();}

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
        Entry cur = Entry(key, value);
        T.insert(cur);
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        if (!T.FindKey(key)) throw ElementNotExist("\nNo Such Element\n");
        T.remove(Entry(key, 0));
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {return T.size();}
};

#endif
