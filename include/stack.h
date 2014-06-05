#ifndef SUNJIAN_RB_STACK_H_
#define SUNJIAN_RB_STACK_H
#include "rbtree.h"

typedef struct key_value key_value_t;

struct key_value 
{
	typedef unsigned long	key_t;
	typedef void*			value_t;
	typedef void (*gc_func_t)(void*);

	key_value() 
		: key(LONGMAX), value(0), release(0) {}
	key_value(key_t k, value_t v, gc_func_t release=0) 
		: key(k), value(v), release(release) {}

	key_t		key;
	value_t		value;
	gc_func_t	release; // Gallery collection callback function for value.

	void set(key_t k, value_t v, gc_func_t r)
	{
		key		= k;
		value	= v;
		release	= r;
	}

	bool is_none() const
	{
		if((sz_t)-1 == key && 0 == value)
			return true;
		else
			return false;
	}

	void set_none()
	{
		set(LONGMAX, 0, 0); //LONGMAX key means an invalid key.
	}
};

class statistic_stack 
{
public:
	typedef unsigned long		key_t;
	typedef unsigned long		sz_t;
	typedef void*				value_t;
	typedef rb_tree_node		element_t;
	typedef rb_tree_iterator	iterator;

	explicit statistic_stack(void) 
		: rb_stack(0) 
	{
		rb_stack = new rb_tree();
	}
	explicit statistic_stack(const key_value_t*, sz_t);
	virtual ~statistic_stack(void)
	{
		if(rb_stack)
			delete rb_stack;
	}

	void		push(const key_value_t &kv);
	void		pop(void);
	key_value_t	top(void) const;
	key_value_t	find_kth(sz_t) const;

	void show_stack(void) const
	{
		if(rb_stack)
			rb_stack->dump_tree();
	}

	bool minimum(key_value_t &ret_kv) const
	{
		if(empty())
		{
			ret_kv.set_none();
			return false;
		}
		else 
		{
			element_t *pnode = rb_stack->leftmost();
			ret_kv.set(pnode->key, pnode->data, pnode->data_destructor);
			return true;
		}
	}
	bool maximum(key_value_t &ret_kv) const
	{
		if(empty())
		{
			ret_kv.set_none();
			return false;
		}
		else
		{
			element_t *pnode = rb_stack->rightmost();
			ret_kv.set(pnode->key, pnode->data, pnode->data_destructor);
			return true;
		}
	}

	sz_t size(void) const
	{
		if(rb_stack)
			return rb_stack->size();
		else
			return (sz_t)0;
	}

	bool empty(void) const 
	{
		if(!rb_stack || rb_stack->empty())
			return true;
		else
			return false;
	}

	void clear(void) 
	{
		if(rb_stack)
			rb_stack->clear();
	}

private:
	rb_tree	*rb_stack;
};

#endif
