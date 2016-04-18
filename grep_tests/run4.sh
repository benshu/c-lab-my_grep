#! /usr/bin/env bash

cat ~nimrodav/grep_tests/3lines | ./my_grep bla | diff ~nimrodav/grep_tests/out3 -

