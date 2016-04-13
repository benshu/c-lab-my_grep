#! /usr/bin/env bash

make
echo "Preforming Valgrind memory check"
valgrind --leak-check=yes ./my_grep a ./grep_tests/emptyfile
echo "Starting tests..."
echo "----------------------------------------"
./my_grep a ./grep_tests/emptyfile | diff ./grep_tests/emptyfile -
./my_grep bla ./grep_tests/bla | diff ./grep_tests/bla -
./my_grep bla ./grep_tests/3lines | diff ./grep_tests/out3 -
cat ./grep_tests/3lines | ./my_grep bla | diff ./grep_tests/out3 -
./my_grep -n pdf ./grep_tests/2013.html | diff ./grep_tests/myout5 -
./my_grep -n -i -E "o\.pdf" ./grep_tests/2013.html | diff ./grep_tests/out5 -
./my_grep -A 2 -n -i io grep_tests/2013.html | diff grep_tests/out6 -
./my_grep -b -i -E "o\.pdf" grep_tests/2013.html | diff grep_tests/out7 -
./my_grep -c -i -E "o\.pdf" grep_tests/2013.html | diff grep_tests/out8 -
./my_grep -v -i -E "o\.pdf" grep_tests/2013.html | diff grep_tests/out9 -
echo "----------------------------------------"
echo "Tests finished!"
