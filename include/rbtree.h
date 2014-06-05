#ifndef SUNJIAN_RB_TREE_H
#define SUNJIAN_RB_TREE_H

typedef bool				color_t;
typedef unsigned long		k_t;
typedef unsigned long		count_t;
typedef unsigned long		sz_t;
typedef void*				data_t;
typedef struct rb_tree_node*	
							node_ptr;

static const color_t RED	= false;
static const color_t BLACK 	= true;
static const k_t LONGMAX	= (k_t)-1;

struct rb_tree_node
{
	typedef void (*gc_func_t)(data_t);
	node_ptr	parent;
	node_ptr	left;
	node_ptr	right;
	node_ptr	next;

	color_t		color;
	k_t			key;
	count_t		nchildren;
	data_t		data;
	gc_func_t	data_destructor;
	
	static node_ptr s_minimum(node_ptr x) // find the smallest given a node pointer.
	{
		node_ptr y = x;
		while(y->left != 0)
			y = y->left;
		return y;
	}

	static node_ptr s_maximum(node_ptr x) // find the biggest given a node pointer.
	{
		node_ptr y = x;
		while(y->right != 0)
			y = y->right;
		return y;
	}
};

class rb_tree_base_iterator
{
public:
	void increment();
	void decrement();
	
	node_ptr node; // real tree-node pointer.
};

class rb_tree_iterator : public rb_tree_base_iterator
{
public:
	rb_tree_iterator() {}
	rb_tree_iterator(node_ptr x) { node = x; }
	rb_tree_iterator(const rb_tree_iterator& x) { node = x.node; }

	data_t& 			operator*() { return node->data; } // this must be re-contemplated.
	rb_tree_iterator&	operator++()
	{
		increment();
		return *this;
	}

	rb_tree_iterator	operator++(int) 
	{
		rb_tree_iterator __tmp = *this;
		increment();
		return __tmp;
	}

	rb_tree_iterator&	operator--()
	{
		decrement();
		return *this;
	}

	rb_tree_iterator	operator--(int)
	{
		rb_tree_iterator __tmp = *this;
		decrement();
		return __tmp;
	}

	bool operator==(const rb_tree_iterator& y)const
	{
		return node == y.node; 
	}

	bool operator!=(const rb_tree_iterator& y)const
	{
		return node != y.node;
	}

	node_ptr operator->() const 
	{
		return node;
	}
};

class couple // similar to STL pair.
{
public:
	typedef rb_tree_iterator iterator;
	
	couple() : first(), last(false) {}
	couple(rb_tree_node* ptr, bool b) : first(ptr), last(b) {}
	couple(const iterator& it, bool b) : first(it), last(b) {}
	iterator first;
	bool 	 last;
};

struct rb_tree_base
{
	rb_tree_base() 
		: header(0) 
	{
		header			= get_node();
		header->key		= LONGMAX; //header is an assistant node, it contains invalid key.
		header->data	= 0;
		header->next	= 0;		//from the single-linked stack's perspective, 
									//header is the empty head node of the single-linked stack.
		header->data_destructor
						= 0;
	}

	virtual	~rb_tree_base() 
	{
		put_node(header);
	}
protected:
	node_ptr header;
	node_ptr get_node()
	{
		return new rb_tree_node();
	}

	void put_node(node_ptr p) 
	{
		if(p)
		{
			if(p->data_destructor)
				p->data_destructor(p->data);
			delete p;
		}
	}
};

class rb_tree : protected rb_tree_base
{
public:
	typedef rb_tree_iterator iterator;

	rb_tree() : count(0)
	{ 
		empty_init(); 
	}

	virtual ~rb_tree(void)
	{
		erase_no_rebalance(root());
	}

public: // All rb_tree interfaces are according STL container.
	node_ptr& root(void)		const { return (node_ptr&)header->parent; }
	node_ptr& leftmost(void)	const { return (node_ptr&)header->left; }
	node_ptr& rightmost(void)	const { return (node_ptr&)header->right; }
	node_ptr& get_header_next()	const { return (node_ptr&)header->next; }
	sz_t      size(void)		const { return count; } 	

	iterator  begin()			const { return leftmost(); }
	iterator  end()				const { return header; }
	bool	  empty()			const { return 0UL == count; }

	iterator  find(k_t)			const;
	iterator  insert_equal(node_ptr);
	void	  erase(iterator);
	iterator  upper_bound(k_t)	const;
	iterator  lower_bound(k_t)	const;
	couple	  insert_unique(node_ptr);
	void	  dump_tree(void)	const;
	void	  clear(void);

	static node_ptr		create_node();
	static node_ptr		clone_node(const node_ptr&);
	static void			set_key(k_t key, node_ptr& x) { x->key = key; }
	static void			set_data(data_t data, node_ptr& x) { x->data = data; }
	static const		node_ptr order_select(const node_ptr, k_t);
	static sz_t			order_rank(node_ptr, node_ptr);

private:
	void				empty_init(void);
	iterator			insert(node_ptr, node_ptr, node_ptr);
	inline static void	rebalance(node_ptr, node_ptr&);
	inline static void 	rotate_left(node_ptr, node_ptr&);
	inline static void 	rotate_right(node_ptr, node_ptr&);
	inline static void	destroy_node(node_ptr);
		   static void	erase_no_rebalance(node_ptr);
	node_ptr rebalance_for_erase(node_ptr);

private:
	sz_t count; // store home many nodes there are in tree.
};

#endif
