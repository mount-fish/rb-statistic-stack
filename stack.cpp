#include "stack.h"

statistic_stack::statistic_stack(const key_value_t * array, sz_t sz)
{
	rb_stack = new rb_tree();
	sz_t i = 0;
	for(; i < sz; i++)
	{
		key_value_t kv = key_value(array[i].key, array[i].value);
		push(kv);
	}
}

void
statistic_stack::push(const key_value_t &kv)
{
	element_t* elem	= rb_tree::create_node();
	elem->key		= kv.key;
	elem->data		= kv.value;
	elem->data_destructor 
					= kv.release;
	element_t* &top	= rb_stack->get_header_next();

	elem->next = top;
	top = elem; // insert the element after the asistant header of rb_tree.

	rb_stack->insert_equal(elem);
}

void
statistic_stack::pop(void)
{
	if(empty())
		return;

	element_t *&top = rb_stack->get_header_next();
	element_t *p = top;
	if(top)
		top = top->next;
	if(p)
	{
		rb_stack->erase(iterator(p));
	}
}

key_value_t	
statistic_stack::top() const
{
	key_value_t kv;
	if(empty())
	{
		kv.set_none();	
		return kv;
	}

	element_t *t = rb_stack->get_header_next();
	if(t)
	{
		kv.set(t->key, t->data, t->data_destructor);
	}

	return kv;
}

key_value_t	
statistic_stack::find_kth(sz_t k) const
{
	key_value_t res;
	if(k == 0 || k > size())
	{
		res.set_none();
		return res;
	}
	const element_t *kth_elem = rb_stack->order_select(rb_stack->root(), k);
	if(kth_elem)
	{
		res.set(kth_elem->key, kth_elem->data, kth_elem->data_destructor);
	}
	return res;
}

