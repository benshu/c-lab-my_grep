#! /usr/bin/env bash

./my_grep -A 2 -n -i io ~nimrodav/grep_tests/2013.html | diff ~nimrodav/grep_tests/out6 -

