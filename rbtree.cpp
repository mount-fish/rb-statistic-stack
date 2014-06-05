#include "rbtree.h"
#include <cstdio>

/*START================Implementations of class rb_tree_base_iterator===================*/
void rb_tree_base_iterator::increment()
{
	if(node->right != 0)
	{
		node = node->right;
		while(node->left != 0)
			node = node->left;
	}
	else
	{
		node_ptr y = node->parent;
		while(node == y->right)
		{
			node = y;
			y = y->parent;
		}
		if(node->right != y)
		{
			node = y;
		}
	}
}

void rb_tree_base_iterator::decrement()
{
	if(node->color == RED && node->parent->parent == node)
	{
		node = node->right;
	}
	else if(node->left != 0)
	{
		node_ptr y = node->left;
		while(y->right != 0)
			y = y->right;
		node = y;
	}
	else
	{
		node_ptr y = node->parent;
		while(node == y->left)
		{
			node = y;
			y = y->parent;
		}
		node = y;
	}
}
/*END==================Implementations of class rb_tree_base_iterator===================*/

/*START================Implementations of class rb_tree=====================*/
typedef rb_tree_node* node_ptr;
typedef rb_tree_iterator iterator;

node_ptr
rb_tree::create_node(void)
{
	node_ptr node_tmp 	= new rb_tree_node();
	node_tmp->color 	= 0;
	node_tmp->parent 	= 0;
	node_tmp->left 		= 0;
	node_tmp->right 	= 0;
	node_tmp->key		= 0;
	node_tmp->nchildren	= 1;
	node_tmp->data		= 0;
	return node_tmp;
}

node_ptr
rb_tree::clone_node(const node_ptr& x)
{
	node_ptr node_tmp	= create_node();
	node_tmp->color		= x->color;
	node_tmp->data		= x->data; // todo
	node_tmp->nchildren = x->nchildren;
	return node_tmp;
}

void
rb_tree::empty_init(void)
{
	root() 			= 0;
	leftmost() 		= header;
	rightmost() 	= header;
	header->color 	= RED; //used to distinguish header from root	
}

iterator
rb_tree::find(k_t key) const
{
	node_ptr y = header;
	node_ptr x = root();
	while(x != 0)
	{
		if(!(x->key < key))
			y = x, x = x->left;
		else
			x = x->right;
	}
	iterator j(y);
	return (j == end() || key < j.node->key) ? end() : j; 
}

iterator
rb_tree::insert_equal(node_ptr z)
{
/*	node_ptr node = create_node();
	set_key(key, node);
	set_data(data, node);
*/	
	k_t key = z->key;
	node_ptr y = header;
	node_ptr x = root();

	while(x != 0)
	{
		y = x;
		x = (key < x->key) ? x->left : x->right;
	}
	return insert(x, y, z);
}

couple
rb_tree::insert_unique(node_ptr z)
{
	node_ptr y	= header;
	node_ptr x	= root();
	k_t key		= z->key;
	bool comp	= true;
	while(x != 0)
	{
		y = x;
		comp = (key < x->key);
		x = (comp ? x->left : x->right);
	}
	
	iterator j(y);
	if(comp)
		if(j == begin())
			return couple(insert(x, y, z), true);
		else
			--j;
	if(j.node->key < key)
		return couple(insert(x, y, z), true);
	return couple(j.node, false);
}

iterator
rb_tree::insert(node_ptr x, node_ptr y, node_ptr z)
{
//	node_ptr z = create_node();
//	set_key(key, z);
//	set_data(data, z);

	if(y == header || x != 0 || z->key < y->key)
	{
		y->left = z;
		if(y == header)
		{
			root() = z;
			rightmost() = z;
		}
		else if(y == leftmost())
		{
			leftmost() = z;
		}
	}
	else // z.key >= y.key
	{
		y->right = z;
		if(y == rightmost())
		{
			rightmost() = z;
		}
	}
	
	z->parent = y;

	node_ptr zz = z->parent;
	while(zz != header) //root's parent is NULL. 
	{
		zz->nchildren++;
		zz = zz->parent;
	}

	rebalance(z, root());
	++count;
	return z;
}

void 
rb_tree::rebalance(node_ptr x, node_ptr& root)
{
	x->color = RED;
	while(x != root && x->parent->color == RED)
	{
		// x's parent is the left child of x's grandparent
		if(x->parent == x->parent->parent->left)
		{
			node_ptr y = (node_ptr)(x->parent->parent->right);
			if(y && y->color == RED)//encounter a 4-node, split it
			{
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = (node_ptr)(x->parent->parent);
			}
			else// non 4-node, no need to split it
			{
				if(x == x->parent->right)
				{
					x = x->parent;
					rotate_left(x, root);
				}
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotate_right(x->parent->parent, root);
			}
		}
		else//symmetrical cases 
		{
			node_ptr y = (node_ptr)(x->parent->parent->left);
			if(y && y->color == RED)
			{
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = (node_ptr)(x->parent->parent);
			}
			else
			{
				if(x == x->parent->left)
				{
					x = x->parent;
					rotate_right(x, root);
				}
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = BLACK;
}

void
rb_tree::rotate_left(node_ptr x, node_ptr& root)
{
	node_ptr y = x->right;
	x->right = y->left;
	if(y->left != 0)
		y->left->parent = x;
	y->parent = x->parent;

	if(x == root)
		root = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else 
		x->parent->right = y;
	y->left = x;
	x->parent = y;

	y->nchildren = x->nchildren;
	x->nchildren = (x->left ? x->left->nchildren : 0) + (x->right ? x->right->nchildren : 0) + 1;
}

void
rb_tree::rotate_right(node_ptr x, node_ptr& root)
{
	node_ptr y = x->left;
	x->left = y->right;
	if(y->right != 0)
		y->right->parent = x;
	y->parent = x->parent;

	if(x == root)
		root = y;
	else if(x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;

	//Update x and y's number of children.
	y->nchildren = x->nchildren;
	x->nchildren = (x->left ? x->left->nchildren : 0) + (x->right ? x->right->nchildren : 0) + 1;
}

iterator
rb_tree::upper_bound(k_t key) const
{
	node_ptr y = header;
	node_ptr x = root();

	while(x != 0)
	{
		if(key < x->key)
			y = x, x = x->left;
		else 
			x = x->right;
	}
	return iterator(y);
}

iterator
rb_tree::lower_bound(k_t key) const
{
	node_ptr y = header;
	node_ptr x = root();

	while(x != 0)
	{
		if(!(x->key < key))
			y = x, x = x->left;
		else
			x = x->right;
	}
	return iterator(y);
}

void
rb_tree::destroy_node(node_ptr x)
{
	if(!x)
		return;
	if(x->data_destructor)
		x->data_destructor(x->data);

	printf("node %lu is destroyed\n", x->key);
	delete x;
}

void 
rb_tree::erase_no_rebalance(node_ptr x)
{
	while(x != 0)
	{
		erase_no_rebalance(x->right);
		node_ptr y = x->left;
		destroy_node(x);
		x = y;
	}
}

node_ptr
rb_tree::rebalance_for_erase(node_ptr z)
{
	node_ptr y = z;
	node_ptr x = 0;
	node_ptr x_parent = 0;

	if(y->left == 0)
		x = y->right;
	else
	{
		if(y->right == 0)
			x = y->left;
		else
		{
			y = y->right;
			while(y->left != 0)
				y = y->left;
			x = y->right;
		}
	}

	if(y != z)// y is z's successor
	{
		z->left->parent = y;
		y->left = z->left;
		if(y != z->right) // y is not z's right child
		{
			x_parent = y->parent;
			if(x) x->parent = y->parent;
			y->parent->left = x;
			y->right = z->right;
			z->right->parent = y;
		}
		else
			x_parent = y;

		if(root() == z)
			root() = y;
		else if(z == z->parent->left)
			z->parent->left = y;
		else
			z->parent->right = y;
		y->parent = z->parent;
		y->color ^= z->color;//swap y's color with z'color without third variable.
		z->color ^= y->color;
		y->color ^= z->color;
		y = z;// y to be deleted actually
	}
	else // y == z
	{
		x_parent = y->parent;
		if(x) x->parent = y->parent;
		if(root() == z) 
			root() = x;
		else
		{
			if(z->parent->left == z)
				z->parent->left = x;
			else
				z->parent->right = x;
		}

		if(leftmost() == z)
		{
			if(z->right == 0)// z->left must be 0
				leftmost() = z->parent;
			else
				leftmost() = rb_tree_node::s_minimum(x);
		}
		if(rightmost() == z)
		{
			if(z->left == 0)// z->right must be 0
				rightmost() = z->parent;
			else
				rightmost() = rb_tree_node::s_maximum(x);
		}
	}

	node_ptr xx = x_parent; // xx is used to update nchildren field of tree node up util to root.
	while(xx != header)
	{
		xx->nchildren = (xx->left ? xx->left->nchildren : 0) + (xx->right ? xx->right->nchildren : 0) + 1;
		xx = xx->parent;
	}

	//start adjustment to rebalance
	if(y->color != RED)
	{
		while(x != root() && (x == 0 || x->color == BLACK))
		{
			if(x == x_parent->left)
			{
				node_ptr w = x_parent->right;
				if(w->color == RED) //case 1
				{
					w->color = BLACK;
					x_parent->color = RED;
					rotate_left(x_parent, root());
					w = x_parent->right;
				}
				if((w->left == 0 || w->left->color == BLACK) &&
						(w->right == 0 || w->right->color == BLACK))//case 2
				{
					w->color = RED;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else
				{
					if(w->right == 0 || w->right->color == BLACK)
					{
						if(w->left)
							w->left->color = BLACK;
						w->color = RED;
						rotate_right(w, root());
						w = x_parent->right;
					}
					w->color = x_parent->color;
					x_parent->color = BLACK;
					if(w->right)
						w->right->color = BLACK;
					rotate_left(x_parent, root());
					break;
				}
			}
			else //symmetric cases
			{
				node_ptr w = x_parent->left;
				if(w->color == RED)
				{
					w->color = BLACK;
					x_parent->color = RED;
					rotate_right(x_parent, root());
					w = x_parent->left;
				}
				if((w->right == 0 || w->right->color == BLACK) &&
						(w->left == 0 || w->left->color == BLACK))
				{
					w->color = RED;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else
				{
					if(w->left == 0 || w->left->color == BLACK)
					{
						if(w->right)
							w->right->color = BLACK;
						w->color = RED;
						rotate_left(w, root());
						w = x_parent->left;
					}
					w->color = x_parent->color;
					x_parent->color = BLACK;
					if(w->left) 
						w->left->color = BLACK;
					rotate_right(x_parent, root());
					break;
				}
			}
		}// end of while
		if(x)
			x->color = BLACK;
	}
	return y;
}

void
rb_tree::erase(iterator it)
{
	node_ptr to_remove = rebalance_for_erase(it.node);	
	destroy_node(to_remove);
	--count;
}

const node_ptr 
rb_tree::order_select(const node_ptr x, sz_t kth) 
{
	if(!x)
		return 0;

	sz_t r = (x->left ? x->left->nchildren + 1 : (sz_t)1);
	
	if(kth == r)
		return x;

	if(kth < r)
		return order_select(x->left, kth);
	else
		return order_select(x->right, kth -r);
}

sz_t 
rb_tree::order_rank(node_ptr root, node_ptr x)
{
	if(!root || !x)
		return LONGMAX;

	sz_t r = (x->left ? x->left->nchildren +1 : 1);
	node_ptr y = x;
	while(y != root)
	{
		if(y == y->parent->right)
			r += (y->parent->left ? y->parent->left->nchildren + 1 : 1);
		y = y->parent;
	}
	return r;
}

void 
rb_tree::dump_tree(void) const
{
	iterator it = begin();
	while(it != end())
	{
		k_t pk, lk, rk;
		pk = (it->parent	? it->parent->key	: LONGMAX);
		lk = (it->left		? it->left->key		: LONGMAX);
		rk = (it->right		? it->right->key	: LONGMAX);
		printf("%p key %4lu; color %s; nchildren %4lu; parent 0x%08x(%4ld); left 0x%08x(%4ld); right 0x%08x(%4ld);\n", 
				it.node, it->key, it->color ? "BLACK" : " RED ", it->nchildren, it->parent, pk, it->left, lk, it->right, rk);
		++it;
	}
}

void
rb_tree::clear(void)
{
	erase_no_rebalance(root());
	empty_init();
	header->next = 0;
	count = 0UL;
}

/*END==================Implementations of class rb_tree=====================*/
