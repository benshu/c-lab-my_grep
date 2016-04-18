#! /usr/bin/env bash

./my_grep -c -i -E "o\.pdf" ~nimrodav/grep_tests/2013.html | diff ~nimrodav/grep_tests/out8 -

