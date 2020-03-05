#!/bin/sh
make -f Makefile.txt
echo -e
./test_environment
echo -e
valgrind --leak-check=full --track-origins=yes --tool=memcheck ./test_environment
