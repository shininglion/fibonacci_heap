#include <cstdio>
#include <cstdlib>
#include "pheap.h"


using namespace std;


template <typename T, typename GetKey, typename Compare>
pheap<T, GetKey, Compare>::pheap(GetKey key, Compare cmp) : getkey(key), compare(cmp), root(nullptr), heap_size(0) {}


template <typename T, typename GetKey, typename Compare>
void pheap<T, GetKey, Compare>::clear()
{
	root = nullptr;
	heap_size = 0;
}


template <typename T, typename GetKey, typename Compare>
size_t pheap<T, GetKey, Compare>::size() const
{ return this->heap_size; }


template <typename T, typename GetKey, typename Compare>
auto pheap<T, GetKey, Compare>::emplace(const value_type& data) -> handle_type
{
    handle_type new_handle(new node_type(data));
    this->insert(new_handle);

    return new_handle;
}


template <typename T, typename GetKey, typename Compare>
void pheap<T, GetKey, Compare>::insert(handle_type el)
{
	heap_size++;
	el->lchild = el->left = el->right = nullptr;
	if(root == nullptr) {
		root = el;
    }
	else {
		root = cmpAndlink(root, el);
	}
}


template <typename T, typename GetKey, typename Compare>
auto pheap<T, GetKey, Compare>::extract() -> handle_type
{
	if(heap_size == 0) {
		printf("ERROR %s %d\n", __FILE__, __LINE__);
		exit(1);
	}
	heap_size--;

	auto oldRoot = root;
	if(root->lchild == nullptr) {
		root = nullptr;
    }
	else {
		root = combineSiblings(root->lchild);
    }

	oldRoot->lchild = oldRoot->left = oldRoot->right = nullptr;
	return oldRoot;

}


template <typename T, typename GetKey, typename Compare>
void pheap<T, GetKey, Compare>::decreaseKey(handle_type el, key_type newValue)
{
    if (compare(getkey(el->data), newValue)) {
		printf("ERROR %s %d\n", __FILE__, __LINE__);
		exit(1);
	}

	getkey(el->data) = newValue;
	if(el != root) {
		if(el->right != nullptr) {
			el->right->left = el->left;
        }
		if(el->left->lchild == el) {
			el->left->lchild = el->right;
        }
		else {
			el->left->right= el->right;
        }
		el->left = el->right = nullptr;
		root = cmpAndlink(root, el);
	}

}


template <typename T, typename GetKey, typename Compare>
auto pheap<T, GetKey, Compare>::cmpAndlink(handle_type e1, handle_type e2) -> handle_type
{
	if(compare(getkey(e1->data), getkey(e2->data))) {
		e2->left = e1;
		e2->right = e1->lchild;
		if(e2->right != nullptr) {
			e2->right->left = e2;
        }
		e1->lchild = e2;
		return e1;
	}
	else
	{
		e1->left = e2;
		e1->right = e2->lchild;
		if(e1->right != nullptr) {
			e1->right->left = e1;
        }
		e2->lchild = e1;
		return e2;
	}
}


template <typename T, typename GetKey, typename Compare>
auto pheap<T, GetKey, Compare>::combineSiblings(handle_type left) -> handle_type
{
    std::vector<handle_type> tmp(heap_size);
	size_t num = 0;

	for(; left != nullptr; ++num) {
        if (tmp.size() <= num) { tmp.resize(tmp.size() << 1); }

		if(left->right != nullptr) {
			auto tmpEl1 = left;
			auto tmpEl2 = left->right;
			left = left->right->right;
			tmpEl1->left = tmpEl1->right = tmpEl2->left = tmpEl2->right = nullptr;
			tmp[num] = cmpAndlink(tmpEl1, tmpEl2);
		}
		else {
			left->left = nullptr;
			tmp[num] = left;
			left = nullptr;
		}
	}
	for(num -= 1; num >= 1; num--) {
		tmp[num-1] = cmpAndlink(tmp[num-1], tmp[num]);
    }

	return tmp[0];
}
