#include "rbtree.h"
#include <iostream>

template<class K, class V>
class KVContainer
{
public:
    virtual bool Put(const K k, const V v)  = 0;
    virtual bool Get(const K k, V &v) = 0;
    virtual bool Remove(const K k)    = 0;
};

template<class K, class V>
class Map : public KVContainer<K,V>
{
private:
    class Schema
    {
    public:
        K key;
        V value;
    public:
        Schema(){}
        Schema(K key, V value):key(key),value(value){}
    };
    
private:
    static int fn(void* a, void* b)
    {
        return *(K*)a - *(K*)b;
    }
private:
    template<typename T>
    void* dup(const T key)
    {
        return (void*)( new (malloc(sizeof(key))) T(key));
    }
public:
    Map():
    mytree(NULL)
    {
        mytree = rbtree_init(fn);
    }
    ~Map()
    {
        
    }
    virtual bool Put(const K key, const V value)
    {
        V* pHit = (V*)rbtree_lookup(mytree, dup(key));
        if (pHit) {
            *pHit = value;
            return true;
        }
        
       
        // Create New - Insert
        int ret = rbtree_insert(mytree, dup(key), dup(value));
        return ret == 0;
    }
    
    virtual bool Get(const K key, V& value)
    {
        V* pHit = (V*)rbtree_lookup(mytree, dup(key));
        if (pHit) {
            value = *pHit;
            return true;
        }
        
        if (pHit) {
            value = *pHit; // Copy Value to Output Parameter
            return true;
        }
        else
        {
            return false;
        }
    }
    
    virtual bool Remove(const K key)
    {
        int ret = rbtree_remove(mytree, (void*)&key);
        
        if ( ret == 0)
        {
            return true;
        }

        return false;
    }
    
private:
    struct rbtree* mytree;
};

int develop_generic_typed_map()
{
    Map<int, int> dict;
    
    for(int j = 0; j < 1000000; j++)
    for (int i = 0; i < 10000; i++) {
        if(!dict.Put(i, i + 50))
        {
            std::cout << "if(!dict.Put(i, i + 50)) failed. " << "i = " << i << std::endl;
        }
//        std::cout << "Put returns " << (bRet?"true":"false") << std::endl;
        int value = i + 50;
        if (dict.Get(i, value)) {
            std::cout << "Loop#:" << j << " " << "dict[" << i << "] = " << value << std::endl;
        }
        else
        {
            std::cout << "dict.Get Failed" << std::endl;
        }
    }
    return 0;
}

int main()
{
    return develop_generic_typed_map();
}

