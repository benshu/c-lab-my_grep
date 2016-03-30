#! /usr/bin/env bash

make
./my_grep a ./grep_tests/emptyfile | diff ./grep_tests/emptyfile -
./my_grep bla ./grep_tests/bla | diff ./grep_tests/bla -
./my_grep bla ./grep_tests/3lines | diff ./grep_tests/out3 -
cat ./grep_tests/3lines | ./my_grep bla | diff ./grep_tests/out3 -

