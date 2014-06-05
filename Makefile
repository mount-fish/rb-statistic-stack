IDIR=include
LIBDIR=lib
OBJDIR=objs
STACKH=stack.h
RBTREEH=rbtree.h
DESTLIBDIR=/usr/lib

CC=g++
CFLAGS=-I$(IDIR) -g
LIBCFLAGS=$(CFLAGS) -shared -fPIC

PROCTEST=rbstack-test-no-lib

PROCNAME=rbstack-test
PROCHX=stack.h
PROCH=$(patsubst %,$(IDIR)/%,$(PROCHX))

LIBRBSTACK=librbstack-sj.so
LIBHX=stack.h rbtree.h
LIBH=$(patsubst %,$(IDIR)/%,$(LIBHX))

PROCOBJSX=rbstack-test.o
LIBOBJSX=rbtree.o stack.o
PROCOBJS=$(patsubst %,$(OBJDIR)/%,$(PROCOBJSX))
LIBOBJS=$(patsubst %,$(OBJDIR)/%,$(LIBOBJSX))

all:$(PROCNAME) $(PROCTEST)

$(OBJDIR)/%.o:%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

$(LIBRBSTACK):$(LIBOBJS) $(LIBH)
	$(CC) -o $@ $^ $(LIBCFLAGS)

$(PROCNAME):$(PROCOBJS) $(LIBRBSTACK) $(PROCH)
	$(CC) -o $@ $^ $(CFLAGS) -L.

$(PROCTEST):$(PROCOBJS) $(LIBOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

install:all
	cp -af $(LIBRBSTACK) $(DESTLIBDIR)

.PHONY: clean

clean:
	rm -f $(PROCNAME) $(LIBRBSTACK) $(PROCOBJS) $(LIBOBJS) $(PROCTEST)
	
