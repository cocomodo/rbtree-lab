.PHONY: test

CFLAGS=-I ../src -Wall -g -DSENTINEL #센티널을 사용하기 위해 -DSENTINEL을 붙여줌.

test: test-rbtree
	./test-rbtree
	valgrind ./test-rbtree

test-rbtree: test-rbtree.o ../src/rbtree.o

../src/rbtree.o:
	$(MAKE) -C ../src rbtree.o

clean:
	rm -f test-rbtree *.o
