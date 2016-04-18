#! /usr/bin/env bash

cat ~nimrodav/grep_tests/my_grep | ./my_grep my_grep | diff ~nimrodav/grep_tests/my_grep_out -

