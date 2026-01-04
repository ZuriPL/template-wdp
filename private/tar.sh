#!/bin/bash

name=$1

tar --exclude='./.git' --exclude='.gitignore' --exclude='./private' --exclude='./src/*' --exclude='a.out' -zcvf $PATH_ROOT/private/$name.tar .