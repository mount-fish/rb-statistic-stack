#include <iostream>
#include "stack.h"
using namespace std;

static void description(void);
static void usage(void);

int main(int argc, char * argv[])
{

//	k_t keys[5] = {2, 1, 3, 8, 4};
//	key_value_t kvs[5];
//	sz_t i = 0;
//	for(; i < sizeof(keys)/sizeof(keys[0]); i++)
//	{
//		kvs[i].set(keys[i], 0, 0);
//	}

//	statistic_stack *sst = new statistic_stack(kvs, sizeof(kvs)/sizeof(kvs[0]));
	statistic_stack *sst = new statistic_stack();
	char c;

	description();
	cout << endl;
	usage();
	cout << endl;
	
	cout << "COMMAND:" ;
	while((c = cin.get()) != EOF)
	{
		if(c == 'q')
			break;

		switch(c)
		{
		case 'p': //push
			{
				k_t k;
				cout	<< "Input the a sequence of keys to be pushed into the statistical stack," << endl 
						<< "press CTRL+D to indicate the end of the sqeuence." << endl;
				while(cin >> k)
				{
					sst->push(key_value(k, 0));
				}
			}
			break;
		case 'o': //pop
			{
				key_value_t x = sst->top();
				sst->pop();
				if(!x.is_none())
					cout << x.key << " is poped" << endl;
				else
					cout << "stack empty" << endl;
			}
			break;
		case 't': //top
			{
				key_value_t x = sst->top();
				if(!x.is_none())
					cout << x.key << " is at top of stack" << endl;
				else
					cout << "stack empty" << endl;
			}
			break;
		case 'k': //search kth element
			{
				sz_t k;
				cout << "Input a number between 1 and " << sst->size() << ":";
				cin >> k;
				key_value_t res = sst->find_kth(k);
				if(res.is_none())
					cout << "no " << k << "th element" << endl;
				else
					cout << k << "th element with key " << res.key << endl;
			}
			break;

		case 'd':// dump the stack
			{
				if(sst->empty())
					cout << "empty stack" << endl;
				sst->show_stack();
			}
			break;

		case 'c':// clear the whole stack
			{
				sst->clear();
			}
			break;

		case 'z': // current stack size
			{
				if(sst->empty())
					cout << "empty stack" << endl;
				else
					cout << "current stack size is " << sst->size() << endl;
			}
			break;

		case 'm': // get maximum.
			{
				key_value_t max;
				if(sst->maximum(max))
					cout << "maximum is " << max.key << endl;
				else
					cout << "stack is empty" << endl;
			}
			break;

		case 'n': // get minimum
			{
				key_value_t min;
				if(sst->minimum(min))
					cout << "minimum is " << min.key << endl;
				else
					cout << "stack is empty" << endl;
			}
			break;

		case 'u': // show usage
			usage();
			break;
		case 'x': // show description.
			description();
			break;
		default:
			continue;
		}
		cin.clear();
		cin.sync();
		cout << "COMMAND:";
	}

	if(sst)
		delete sst;
	return 0;
}

void description()
{
	cout << "RBSTACK-TEST  - A simple demonstration to show how to use librbstack-sj." << endl;
	cout << endl;
	cout << "LIBRBSTACK-SJ - A statistic stack implemented based on an augmented red-black tree, in which each element " << endl;
	cout <<	"                contains an unsigned integer as its key currently and maintens a member to indicate how   " << endl;
	cout << "                many nodes there are in a subtree. Users can issue push, pop, top and getting minimum,    " << endl;
	cout << "                maximum, kth small element and current size of the stack randomly. Operations of pop, push" << endl;
	cout << "                and find kth element run with time complexity of O(logn) and others all run with time com-" << endl;
	cout << "                plexity of O(1). So librbstack-sj is efficient and proper to be used for dynamical statis-" << endl;
	cout << "                tic of stack with large amount of elements. " << endl;
	cout << "                This library is implemented by SUNJIAN on June 1st 2014. Please contact jacksun328@163.com" << endl;
	cout << "                if you'd like to have a discussion." << endl;
}

void usage()
{
	cout << "USAGE:" << endl;
	cout << "\tp\tpush a element or several elements into statistic stack." << endl;
	cout << "\to\tpop top element out of stack." << endl;
	cout << "\tt\tpeek the top element of current stack." << endl;
	cout << "\tm\tget the maximum element of current stack." << endl;
	cout << "\tk\tget the kth small element of current stack." << endl;
	cout << "\tn\tget the minimum element of current stack." << endl;
	cout << "\td\tdump the red-black tree (for debugging)." << endl;
	cout << "\tc\tclear the whole stack." << endl;
	cout << "\tu\tshow this usage." << endl;
	cout << "\tx\tshow description about this program." << endl;
	cout << "\tz\tshow the size of current stack." << endl;
	cout << "\tq\tquit the process." << endl;
}
