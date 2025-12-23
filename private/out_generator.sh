#!/bin/bash

list="small medium large"

for type in $list
do
    for t in $PATH_TESTS/$type/*.in
    do
        $EXE < $t > ${t%in}out
        echo "$t"
    done
done