#ifndef _FIB_H_
#define _FIB_H_


#include <vector>
#include <memory>
#include <type_traits>


template <typename T>
class ReturnSelf
{
public:
    T& operator() (T& self) const { return self; }
};


template < typename T > struct pheap_el;
template <typename T>
using pheap_handle = std::shared_ptr< pheap_el<T> >;


template < typename T >
struct pheap_el {
	pheap_handle<T> lchild;
	pheap_handle<T> left;
	pheap_handle<T> right;
	T	data;

    pheap_el(const T& d = T()) : lchild(nullptr), left(nullptr), right(nullptr), data(d) {}
};


/*
 * GetKey:
 * Unary function that accepts an element with type reference to T as argument
 * and returns a non-const reference of a variable with type Key.
 */
template < typename T, typename GetKey=ReturnSelf<T> >
class pheap
{
public:
    typedef T value_type;
    typedef pheap_el<T> node_type;
    typedef pheap_handle<T> handle_type;
    typedef typename std::remove_reference<typename std::result_of<GetKey(T&)>::type>::type key_type;

    pheap(GetKey key = GetKey());
	void clear();
	void insert(handle_type);
    // return the newly created handle containing given data
    handle_type emplace(const value_type& data);
    handle_type extract();
	void decreaseKey(handle_type, key_type);
    size_t size() const;

private:
    GetKey getkey;
	handle_type root;
	size_t heap_size;

	handle_type cmpAndlink(handle_type, handle_type);
	handle_type combineSiblings(handle_type);
};


#include "pheap.hpp"


#endif /* _FIB_H_ */
