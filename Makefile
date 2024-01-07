PROJECT=cpp-language-tests
PWD=$(shell pwd)
INCLUDE=-I/usr/local/include/ -I/opt/local/include/
LIBPATH=-L/usr/local/lib/ -L/opt/local/lib/
LIBRARY=-lgtest

CC=g++ -std=c++17

all: test

test: clean_test
	cd Testing;\
	$(CC) $(INCLUDE) -c main.cc ;\
	$(CC) -o $@ main.o $(LIBPATH) $(LIBRARY);\
	./$@
	
clean: clean_test clean_mac
clean_test:
	rm -rf Testing/*.o
	rm -rf Testing/test
clean_mac:
	find .. -name .DS_Store -exec rm {} \;
	
leaks: clean test
	cd Testing;\
	leaks -atExit -- ./test
	
style:
	sh ./format.sh .