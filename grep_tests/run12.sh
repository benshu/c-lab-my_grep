#! /usr/bin/env bash

./my_grep -E "(bla|li)" ~nimrodav/grep_tests/3lines | diff ~nimrodav/grep_tests/out11 -

