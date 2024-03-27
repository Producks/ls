#!/bin/bash
# Author producks 24/03/2024

for test in 'x' 'a' 'lib';
  do
    ls $test &> realresult
    ./ft_ls $test > result
    diff -c result realresult
  done

rm -rf result realresult
