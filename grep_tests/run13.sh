#! /usr/bin/env bash

./my_grep -E "r(i|A).[y-z]\." ~nimrodav/grep_tests/2013.html | diff ~nimrodav/grep_tests/out13 -

